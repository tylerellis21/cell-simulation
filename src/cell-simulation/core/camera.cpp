#include "camera.h"

#include <SFML/Window/Keyboard.hpp>

Camera::Camera() :
    m_mode(mode::Free),
    m_speed(250.0f),
    m_zoomSpeed(0.01f),
    m_location(0, 0),
    m_trackingEntity(0)
{ }

void Camera::resize(const uint32 width, const uint32 height)
{
    m_view.setSize(width, height);
}

void Camera::trackEntity(Entity* entity)
{
    m_mode = mode::Track;
    m_trackingEntity = entity;
}

void Camera::update(const float dt)
{
    // If we are in the tracking mode,
    // Update our position to be the same as the entities position as long as it is valid.
    if (m_mode == mode::Track) {

        // Make sure we have a valid entity to track
        // Otherwise we want to pop back out of the track mode.
        if (m_trackingEntity) {
            m_location = m_trackingEntity->getLocation();
            m_view.setCenter(m_location.x, m_location.y);
        }
        else {
            m_mode = mode::Free;
        }
    }
}

void Camera::applyKeyboardControls(const float dt)
{
    bool swapMode = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        m_view.move(0.0f, -m_speed * dt);
        swapMode = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        m_view.move(0.0f, m_speed * dt);
        swapMode = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_view.move(-m_speed * dt, 0.0f);
        swapMode = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_view.move(m_speed * dt, 0.0f);
        swapMode = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        m_view.zoom(1.0f + m_zoomSpeed);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        m_view.zoom(1.0f - m_zoomSpeed);

    // Switch back to free mode if they used the control keys.
    if (swapMode) {
        m_mode = mode::Free;
    }
}

void Camera::render(sf::RenderTarget& target)
{
    target.setView(m_view);
}
