#include "ircbot.h"

// Standard includes.
#include <fstream>

// SFML includes.
#include <SFML/System/Sleep.hpp>

// Project includes.
#include "../core/console.h"

IrcBot::IrcBot() :
    m_thread(&IrcBot::run, this),
    m_alive(true),
    m_port(6667),
    m_serverAddress("irc.twitch.tv"),
    m_authToken("NOTSET!"),
    m_username("NOTSET!")
{
}

IrcBot::~IrcBot()
{ }

bool IrcBot::initialize()
{
    tryConfigLoad();

    const sf::IpAddress serverIP(m_serverAddress);

    if (m_socket.connect(serverIP, m_port) != sf::Socket::Done) {

        std::stringstream sb;
        sb << "Failed to connect to server address: " << m_serverAddress << ", on port: " << m_port;
        Console::write(sb.str(), sf::Color::Red);

        return false;
    }
    else {

        std::stringstream sb;
        sb << "Connect to server address: " << m_serverAddress << ", on port: " << m_port;
        Console::write(sb.str(), sf::Color::Green);
    }

    m_thread.launch();

    return true;
}

void IrcBot::destroy()
{
    m_alive = false;
    tryConfigSave();
}

void IrcBot::tryConfigLoad()
{
    std::ifstream inputFile(CONFIG_FILE_PATH);

    if (!inputFile.is_open()) {
        std::stringstream sb;
        sb << "failed to open json config: " << CONFIG_FILE_PATH;
        Log::warn(sb.str());
    }
    else {
        std::string error = picojson::parse(m_config, inputFile);
        if (!error.empty()) {
            Log::error(std::string("couldn't parse config file! Error: ").append(error));
            return;
        }

        m_port = (uint16)m_config.get("server-port").get<double>();
        m_serverAddress = m_config.get("server-address").to_str();
        m_authToken = m_config.get("twitch-auth-token").to_str();
        m_username = m_config.get("twitch-username").to_str();
    }
}

void IrcBot::tryConfigSave()
{
    std::ofstream output(CONFIG_FILE_PATH.c_str());
    if (!output.is_open()) {
        return;
    }

    picojson::object config;

    config["server-port"] = picojson::value(double(m_port));
    config["server-address"] = picojson::value(m_serverAddress);
    config["twitch-auth-token"] = picojson::value(m_authToken);
    config["twitch-username"] = picojson::value(m_username);

    // Pass true to serialize in a neat readable format.
    output << picojson::value(config).serialize(true) << std::endl;

    output.flush();
    output.close();
}

void IrcBot::run()
{
    Console::write("IrcBot starting");

    while (m_alive)
    {

        char temp[1024];
        std::size_t received = 0;

        if (m_socket.receive(temp, 1024, received) != sf::Socket::Done) {
            Console::write("unexpected socket receive error!");
            m_alive = false;
            return;
        }
        else {

            std::stringstream sb;
            sb << "received bytes: " << received;
            Console::write(sb.str(), sf::Color::Blue);
        }

        // Wait so we aren't constantly using cpu time.
        sf::sleep(sf::milliseconds(25));
    }

    Console::write("IrcBot stopping");
}

/*
< PASS oauth:twitch_oauth_token
< NICK twitch_username
> :tmi.twitch.tv 001 twitch_username :Welcome, GLHF!
> :tmi.twitch.tv 002 twitch_username :Your host is tmi.twitch.tv
> :tmi.twitch.tv 003 twitch_username :This server is rather new
> :tmi.twitch.tv 004 twitch_username :-
> :tmi.twitch.tv 375 twitch_username :-
> :tmi.twitch.tv 372 twitch_username :You are in a maze of twisty passages, all alike.
> :tmi.twitch.tv 376 twitch_username :>
*/

void IrcBot::login()
{

}
