#ifndef CONTENT_H_INCLUDE
#define CONTENT_H_INCLUDE

// SFML includes.
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>

const std::string VERTEX_SHADER_PATH = "../../data/resources/shaders/shader.vert";
const std::string FRAGMENT_SHADER_PATH = "../../data/resources/shaders/shader.frag";
const std::string FONT_PATH = "../../data/resources/font.ttf";

class Content
{
public:

    /**
     * @brief Load any needed content.
     * @return true if sucessful.
     */
    static bool initialize();

    /**
     * @brief Destroy any loaded content.
     */
    static void destroy();

    /**
     * @brief The global font used for rendering strings.
     */
    static sf::Font* font;

    /**
     * @brief Our global shader which is used for rendering everything.
     */
    static sf::Shader* shader;
};

#endif // CONTENT_H_INCLUDE
