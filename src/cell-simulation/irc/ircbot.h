#ifndef IRCBOT_H_INCLUDE
#define IRCBOT_H_INCLUDE

// Standard includes.
#include <string>

// SFML includes.
#include <SFML/System/Thread.hpp>
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

// Common includes.
#include <util/log.h>
#include <util/picojson.h>

// Project includes.
#include "../typedefs.h"

const std::string CONFIG_FILE_PATH = "../../data/irc/config.json";
const std::string TWITCH_EOL_SUFFIX = "\r\n";

/**
 * @brief This class handles the irc commands for the specified channels.
 */
class IrcBot
{
public:

    /**
     * @brief The default irc bot constructor.
     */
    IrcBot();

    /**
     * @brief The default irc bot destructor.
     */
    ~IrcBot();

    /**
     * @brief Setup and initialize the irc bot.
     * @return True if successful.
     */
    bool initialize();

    /**
     * @brief Destroy and save any needed content.
     */
    void destroy();

private:

    /**
     * @brief This flag keeps the server thread alive.
     */
    bool m_alive;

    /**
     * @brief The port of the irc server.
     */
    uint16 m_port;

    /**
     * @brief The address to the server.
     */
    std::string m_serverAddress;

    /**
     * @brief The username of the twitch bot.
     */
    std::string m_username;

    /**
     * @brief The auth token used to log into the twitch irc servers.
     */
    std::string m_authToken;

    /**
     * @brief Holds the config values used for the irc bot.
     */
    picojson::value m_config;

    /**
     * @brief The thread used for the tcp communication with the irc server.
     */
    sf::Thread m_thread;

    /**
     * @brief The tcp socket used to connect to the irc server.
     */
    sf::TcpSocket m_socket;

    /**
     * @brief Load the config info.
     * @return True if successful.
     */
    void tryConfigLoad();

    /**
     * @brief Try to save the config file info.
     */
    void tryConfigSave();

    /**
     * @brief This is the main loop for the server thread.
     */
    void run();

    /**
     * @brief Send the inital login details.
     */
    void login();
};

#endif // IRCBOT_H_INCLUDE
