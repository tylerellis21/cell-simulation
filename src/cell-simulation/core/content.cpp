#include "content.h"

sf::Font* Content::font = 0;
sf::Shader* Content::shader = 0;

bool Content::initialize()
{
    shader = new sf::Shader();
    if (!shader->loadFromFile(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)) {
        return false;
    }

    font = new sf::Font();
    if (!font->loadFromFile(FONT_PATH)) {
        return false;
    }
    return true;
}

void Content::destroy()
{
    if (shader)
        delete shader;

    if (font)
        delete font;

    shader = 0;
    font = 0;
}
