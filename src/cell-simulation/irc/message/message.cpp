#include "message.h"

Message::Message(utf8str string) :
    m_string(string)
{ }

bool Message::send(sf::TcpSocket& socket) const
{
    return socket.send(m_string.c_str(), m_string.size()) == sf::Socket::Done;
}
