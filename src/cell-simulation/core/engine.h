#ifndef ENGINE_H_INCLUDE
#define ENGINE_H_INCLUDE

// SFML includes.
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>

// Standard includes.
#include <string>

// Project includes.
#include "../simulation/world.h"
#include "camera.h"

//#include "../irc/ircbot.h"

const r32 ZOOM_MIN = 0.0001f;

/**
 * @brief The engine is responsible for the camera, world and events.
 */
class Engine
{
public:

    /**
     * @brief Default engine constructor.
     */
    Engine();

    /**
     * @brief Default engine destructor.
     */
    ~Engine();

    /**
     * @brief Initialize the engine and load any needed content.
     * @return true if sucessful.
     */
    bool initialize();

    /**
     * @brief Destroy any loaded content.
     */
    void destroy();

    /**
     * @brief Occurs when the window is resized.
     * @param width = The view width.
     * @param height = The view height.
     */
    void resize(const u32 width, const u32 height);

    /**
     * @brief Occurs when a key is pressed.
     * @param e = They key event data.
     */
    void keyPress(sf::Event::KeyEvent e);

    /**
     * @brief Update the engine.
     * @param dt = Delta time.
     */
    void update(const r32 dt);

    /**
     * @brief Render the engine.
     * @param target = Target to render to.
     */
    void render(sf::RenderTarget& target);

private:

    /**
     * @brief The average time it takes to render one frame.
     */
    r32 m_avgRenderTime;

    /**
     * @brief The counter used to keep track of the number of values in the render time accumulator.
     */
    r32 m_avgRenderCounter;

    /**
     * @brief The accumulator used to keep track of the average render time.
     */
    r32 m_avgRenderAcc;

    /**
     * @brief The average time it takes to update one frame.
     */
    r32 m_avgUpdateTime;

    /**
     * @brief The counter used to keep track of the number of the values in the update time accumulator.
     */
    r32 m_avgUpdateCounter;

    /**
     * @brief The accumulator used to keep track of the average update time.
     */
    r32 m_avgUpdateAcc;

    /**
     * @brief The last fps value measured.
     */
    i32 m_fps;

    /**
     * @brief The number of frames rendered, used to track the fps.
     */
    i32 m_fpsTicks;

    /**
     * @brief The timer used to track the fps.
     */
    sf::Clock m_fpsTimer;

    /**
     * @brief The last index that we used for entity tracking.
     */
    u32 m_entityTrackingIndex;

    /**
     * @brief The max delta time the engine has encountered so far.
     */
    r32 m_maxDeltaTime;

    /**
     * @brief Used to display debug information.
     */
    sf::Text* m_debugText;

    /**
     * @brief The camera used to control the view of the world.
     */
    Camera m_camera;

    /**
     * @brief The view used to render text.
     */
    sf::View m_textView;

    /**
     * @brief The shader used for all rendering.
     */
    sf::Shader* m_shader;

    /**
     * @brief The approximate render time.
     */
    sf::Clock m_renderTimer;

    /**
     * @brief The approximate update time.
     */
    sf::Clock m_updateTimer;

    /**
     * @brief This clock limits the amout of times we update the debug clock.
     */
    sf::Clock m_debugTextTimer;

    /**
     * @brief The world which the entities live in.
     */
    World m_world;

    /**
     * @brief The instance of the irc bot.
     */
    //IrcBot m_ircBot;

    /**
     * @brief Apply any keyboard movement needed.
     */
    void applyKeyboardMovement(const r32 dt);

    /**
     * @brief Update the debug info.
     */
    void updateDebugInfo();
};

#endif // ENGINE_H_INCLUDE
