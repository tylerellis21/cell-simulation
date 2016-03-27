#ifndef CONSOLE_H_INCLUDE
#define CONSOLE_H_INCLUDE

// Standard include.
#include <vector>
#include <string>

// SFML includes.
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <scl/types.h>

// Project includes.
#include "content.h"

/**
 * @brief This class is used to display debug text in the window.
 */
class Console
{
public:

    /**
     * @brief Containes all of the data to define one console line.
     */
    struct ConsoleItem
    {
        /**
         * @brief x offset of the text item.
         */
        i32 x;

        /**
         * @brief The time the console item was created.
         */
        i32 creationTime;

        /**
         * @brief The amount of time the console item stays alive.
         */
        i32 lifeTime;

        /**
         * @brief The actual text item used to render the text string.
         */
        sf::Text* textItem;

        /**
         * @brief Default console item destructor.
         * @param x = The x offset of the text time.
         * @param creationTime = The time the text item was created.
         * @param size = The size of the text item.
         * @param content = The text in the string.
         * @param color = The color of the text.
         */
        ConsoleItem(i32 x, i32 creationTime, i32 size, sf::String content, sf::Color color) :
            x(x),
            creationTime(creationTime),
            lifeTime(50000)
        {
            textItem = new sf::Text();
            textItem->setCharacterSize(size);
            textItem->setColor(color);
            textItem->setString(content);
            textItem->setFont(*Content::font);
        }

        ~ConsoleItem()
        {
            if (textItem) {
                delete textItem;
                textItem = 0;
            }
        }
    };

    /**
     * @brief Called when the console needs to be destroyed,
     */
    static void destroy();

    /**
     * @brief Write a message to the console.
     * @param text = The text to write.
     * @param color = The color of the text (optional)
     * @param size = The size of the text (optional)
     */
    static void write(sf::String text, sf::Color color = sf::Color::White, u32 size = 14);

    /**
     * @brief Update the console.
     */
    static void update();

    /**
     * @brief Render the console.
     * @param target = The target to render to.
     */
    static void render(sf::RenderTarget& target);

private:

    /**
     * @brief Used to clear items from the console based on their live time.
     */
    static sf::Clock m_consoleClock;

    /**
     * @brief Add of the current console lines.
     */
    static std::vector<ConsoleItem*> m_lines;
};

#endif // CONSOLE_H_INCLUDE
