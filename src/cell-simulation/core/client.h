#ifndef CLIENT_H_INCLUDE
#define CLIENT_H_INCLUDE

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "engine.h"

/**
 * @brief The client class does all of the inital setup, running and destruction.
 */
class Client
{
public:

    /**
     * @brief Default client constructor.
     */
    Client();

    /**
     * @brief Default destructor.
     */
    ~Client();

    /**
     * @brief Start the client.
     */
    int start();

private:

    /**
     * @brief This flag keeps the main loop alive.
     */
    bool m_running;

    /**
     * @brief The main window which is used for rendering.
     */
    sf::RenderWindow m_window;

    /**
     * @brief Our instance of the engine.
     */
    Engine m_engine;

    /**
     * @brief Initialize the client and load any needed content.
     * @return True if sucessful.
     */
    bool initialize();

    /**
     * @brief Destroy any loaded content.
     */
    void destroy();

    /**
     * @brief Apply the display settings from the config class.
     * @return True if sucessful.
     */
    bool applyDisplaySettings();

    /**
     * @brief This is the main loop.
     */
    void run();
};

#endif // CLIENT_H_INCLUDE
