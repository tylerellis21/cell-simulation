#include "console.h"
#include "../util/log.h"

sf::Clock Console::m_consoleClock;
std::vector<Console::ConsoleItem*> Console::m_lines;

void Console::destroy()
{
    for (auto& line : m_lines) {
        delete line;
    }

    m_lines.clear();
}

void Console::write(sf::String text, sf::Color color, uint32 size)
{
    m_lines.push_back(new ConsoleItem(2, m_consoleClock.getElapsedTime().asMilliseconds(), size, text, color));

    if (m_lines.size() > 25) {

        ConsoleItem* item = m_lines[0];
        delete item;
        m_lines.erase(m_lines.begin());
    }

    // TODO: Test on multi language.
    Log::info(text.toAnsiString());
}

void Console::update()
{
    int32 currentTime = m_consoleClock.getElapsedTime().asMilliseconds();

    for (int32 i = m_lines.size() - 1; i >= 0; i--) {

        ConsoleItem* item = m_lines[i];

        if (currentTime - item->creationTime >= item->lifeTime) {
            m_lines.erase(m_lines.begin() + i);
            delete item;
        }
    }
}

void Console::render(sf::RenderTarget& target)
{
    int32 yOffset = 100;

    for (auto& line : m_lines) {

        sf::Text* text = line->textItem;
        text->setPosition(line->x, yOffset += text->getLocalBounds().height + 10.0f);

        target.draw(*text);
    }
}
