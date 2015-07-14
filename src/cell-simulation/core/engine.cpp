#include "engine.h"

// Standard includes.
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

// SFML includes.
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Keyboard.hpp>

// Project includes.
#include "content.h"
#include "console.h"

Engine::Engine() :

    m_avgRenderTime(0.0f),
    m_avgRenderAcc(0.0f),
    m_avgRenderCounter(1.0f),

    m_avgUpdateTime(0.0f),
    m_avgUpdateAcc(0.0f),
    m_avgUpdateCounter(1.0f),

    m_fps(0),
    m_fpsTicks(0),
    m_maxDeltaTime(0.0f),

    m_entityTrackingIndex(0)
{ }

Engine::~Engine()
{ }

bool Engine::initialize()
{
    time_t t;
    time(&t);
    srand(t);

    // Initialize and load the content before we create the rest of the objects
    // Since they may need to use them.
    if(!Content::initialize()) {
        return false;
    }

    m_debugText = new sf::Text();
    m_debugText->setPosition(1.0f, 1.0f);
    m_debugText->setCharacterSize(16);
    m_debugText->setStyle(sf::Text::Bold);
    m_debugText->setFont(*Content::font);

    m_shader = Content::shader;

    if (!m_world.initialize()) {
        return false;
    }

    m_camera.trackEntity(m_world.getEntities()[0]);

    return true;
}

void Engine::destroy()
{
    //if (m_debugText)
    //   delete m_debugText;

    Console::destroy();
    m_world.destroy();
    Content::destroy();
}

void Engine::resize(const uint32 width, const uint32 height)
{
    m_textView = sf::View(sf::FloatRect(sf::Vector2f(), sf::Vector2f(width, height)));
    m_camera.resize(width, height);
}

void Engine::keyPress(sf::Event::KeyEvent e)
{
    bool nowTracking = false;

    if (e.code == sf::Keyboard::R) {
        // Make sure the index is valid after adding.
        m_entityTrackingIndex = (m_entityTrackingIndex + 1) % m_world.getEntityCount();
        nowTracking = true;
    }
    else if (e.code == sf::Keyboard::T) {
        // Make sure the index is valid after subtracting.
        m_entityTrackingIndex = (m_entityTrackingIndex - 1) % m_world.getEntityCount();
        nowTracking = true;
    }
    else if (e.code == sf::Keyboard::Space) {
        // Make sure the index is valid.
        m_entityTrackingIndex = (m_entityTrackingIndex) % m_world.getEntityCount();
        nowTracking = true;
    }
    else if (e.code == sf::Keyboard::I) {
        // Swap the debug flag.
        m_world.setDebug(!m_world.getDebug());
    }

    if (nowTracking) {

        Entity* entity = m_world.getEntity(m_entityTrackingIndex);

        uint32 counter = 0;
        while(entity->getType() != type::Cell && counter < m_world.getEntityCount()) {
           m_entityTrackingIndex = (m_entityTrackingIndex + 1) % m_world.getEntityCount();
           entity = m_world.getEntity(m_entityTrackingIndex);
           counter++;
        }

        if (entity)
            m_camera.trackEntity(entity);
    }
}

void Engine::update(const float dt)
{
    // Begin the measuring the time for this update cycle.
    m_updateTimer.restart();

    m_camera.applyKeyboardControls(dt);

    m_world.update(dt);

    m_camera.update(dt);

    // Update the average update time.
    m_avgUpdateAcc += m_updateTimer.getElapsedTime().asSeconds();
    m_avgUpdateCounter++;
    m_avgUpdateTime = m_avgUpdateAcc / m_avgUpdateCounter;

    // Reset the average update counter after so many.
    /*if (m_avgUpdateCounter > 2500) {
        m_avgUpdateCounter = 1.0f;
        m_avgUpdateAcc = 0.0f;
    }*/

    Console::update();

    if (dt > m_maxDeltaTime) {
        m_maxDeltaTime = dt;
    }
}

void Engine::updateDebugInfo()
{
    if (m_debugTextTimer.getElapsedTime().asSeconds() >= 0.2) {

        m_debugTextTimer.restart();

        std::stringstream str;
        str << std::fixed << std::setprecision(8);

        str << "avg rtime: " << m_avgRenderTime << std::endl;
        str << "avg utime: " << m_avgUpdateTime << std::endl;
        str << std::setprecision(2);
        str << "fps: " << m_fps << std::endl;
        str << "max dt: " << m_maxDeltaTime << std::endl;
        //str << "scale: " << m_worldScale << std::endl;

        vec2f cameraLocation = m_camera.getLocation();
        str << "cam offset: (x: " << cameraLocation.x << ", y: " << cameraLocation.y << ")" << std::endl;
        m_debugText->setString(str.str());
    }
}

void Engine::render(sf::RenderTarget& target)
{
    // Being the measuring the time for this render cycle.
    m_renderTimer.restart();

    // Make sure we have updated d
    updateDebugInfo();

    // Draw stuff here in the world view.
    m_world.render(target, m_camera, m_textView);

    // Update the view so we see text properly.
    target.setView(m_textView);

    target.draw(*m_debugText);

    Console::render(target);

    // Reset the average render counter after so many.
    /*if (m_avgRenderCounter > 2500) {
        m_avgRenderCounter = 1.0f;
        m_avgRenderAcc = 0.0f;
    }*/

    // Average the render time measured with the render time.
    m_avgRenderAcc += m_renderTimer.getElapsedTime().asSeconds();
    m_avgRenderCounter++;
    m_avgRenderTime = m_avgRenderAcc / m_avgRenderCounter;

    // Update the frame counter too. we don't really need it but why not.
    m_fpsTicks++;
    if (m_fpsTimer.getElapsedTime().asSeconds() >= 1.0f) {
        m_fpsTimer.restart();
        m_fps = m_fpsTicks;
        m_fpsTicks = 0;
    }
}
