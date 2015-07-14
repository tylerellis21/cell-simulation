#ifndef MESSAGE_H_INCLUDE
#define MESSAGE_H_INCLUDE

// SFML include
#include <SFML/System/Utf.hpp>
#include <SFML/Network/TcpSocket.hpp>

// Standard includes.
#include <string>

// Project includes.
#include "../../typedefs.h"

// TODO: Make sure this is correct for utf8 strings.
typedef std::basic_string<uint8> utf8str;

/**
 * @brief Represents a utf8 string message.
 */
class Message
{
public:

    /**
     * @brief Default message constructor.
     * @param string = The string value for the message.
     */
    Message(utf8str string);

    /**
     * @brief Send a message over a tcp socket.
     * @param socket = The socket to send the data over.
     * @return True if the message was sent correctly.
     */
    bool send(sf::TcpSocket& socket) const;

protected:

    /**
     * @brief Our message string is stored in utf8 format.
     */
    utf8str m_string;
};

#endif // MESSAGE_H_INCLUDE
