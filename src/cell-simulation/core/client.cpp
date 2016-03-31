#include "client.h"
#include <stdio.h>
#include <util/log.h>
#include "config.h"

#include <SFML/OpenGL.hpp>

const std::string LOG_FILE_PATH = "../../data/log.txt";
const std::string CFG_FILE_PATH = "../../data/config.json";

const std::string WINDOW_TITLE = "Cell Simulation";

Client::Client() :
    m_running(true)
{ }

Client::~Client()
{ }

bool Client::initialize()
{
    Log::initialize(LOG_FILE_PATH);
    Config::load(CFG_FILE_PATH);

    if (!applyDisplaySettings()) {
        return false;
    }

    return m_engine.initialize();
}

void Client::destroy()
{
    m_engine.destroy();

    Config::save(CFG_FILE_PATH);
    Log::destroy();
}

bool Client::applyDisplaySettings()
{
    //The context settings apply to the opengl context that we create.
    sf::ContextSettings contextSettings;

    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;

    contextSettings.antialiasingLevel = Config::getAALevel();
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 4;
    // We want to create the window after the settings were loaded in case
    // anything client setting is different that the defaults.
    sf::VideoMode videoMode(Config::getWidth(), Config::getHeight(), 32);

    // Don't warn in fullscreen since anything works really.
    if (!Config::getFullscreen()) {

        //Just warn the user if they have a non optimal video mode selected.
        if (!videoMode.isValid()) {
            Log::error(std::string("non optimal video mode selected in the config file!"));
            //return false;
        }
    }

    //Here we check to go full screen
    //We just get the first full screen mode.
    //TODO (Tyler): Figure out a way to properly handle this stuff.
    if (Config::getFullscreen()) {
        m_window.create(sf::VideoMode::getFullscreenModes()[0],
            WINDOW_TITLE, sf::Style::Fullscreen, contextSettings);
    }
    else {
         m_window.create(videoMode, WINDOW_TITLE, sf::Style::Default, contextSettings);
    }

    m_window.setVerticalSyncEnabled(Config::getVSync());

    //Make sure we don't set the framerate limit to zero.
    if (Config::getFpsLimit() > 0) {
        m_window.setFramerateLimit(Config::getFpsLimit());
    }


    //Here we are just getting the settings after we have created the window.
    //And simply writing some debug about the current context settings.
    sf::ContextSettings windowSettings = m_window.getSettings();

    std::stringstream sb;
    sb << "depthBits: " << windowSettings.depthBits << ", ";
    sb << "stencilBits: " << windowSettings.stencilBits << ", ";
    sb << "antialiasingLevel: " << windowSettings.antialiasingLevel << ", ";
    sb << "majorVersion: " << windowSettings.majorVersion << ", ";
    sb << "minorVersion: " << windowSettings.minorVersion << ", ";

    //The renderer is the device that is actually getting the data from opengl.
    sb << "renderer: " << std::string((char*)glGetString(GL_RENDERER));

    Log::debug(sb.str());

    //Return true since the display settings were applied properly.
    return true;
}

int Client::start()
{
    // Make sure we load and initalize correctly.
    if (!initialize()) {
        return -1;
    }

    // Run the main loop.
    run();

    // Destroy content since we are now closing.
    destroy();

    m_window.close();

    return 0;
}

void Client::run()
{
    sf::Clock deltaTime;

    while (m_running) {

        sf::Event e;
        while (m_window.pollEvent(e)) {

            if (e.type == sf::Event::Closed) {
                m_running = false;
            }
            else if (e.type == sf::Event::Resized) {
                m_engine.resize(e.size.width, e.size.height);
            }
            else if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Escape) {
                    m_running = false;
                }
                else {
                    m_engine.keyPress(e.key);
                }
            }

        }

        const float dt = deltaTime.restart().asSeconds();

        //std::cout << dt << std::endl;

        m_engine.update(dt);

        m_window.clear();
        m_engine.render(m_window);
        m_window.display();
    }
}
