#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE

// SFML includes.
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

// Nex includes.
#include <nex/math/vec2.h>
using nx::vec2f;

// Project includes.
#include "../typedefs.h"
#include "../simulation/entity.h"

namespace mode
{

/**
 * @brief Defines the usable modes for the camera.
 */
enum CameraMode {

    Free = 0,
    Track = 1
};

}

/**
 * @brief This class controls the view of the simulation.
 */
class Camera
{
public:

    /**
     * @brief The default camera constructor.
     */
    Camera();

    /**
     * @brief Occurs when the main window is resized.
     * @param width = The new window width.
     * @param height = The new window height.
     */
    void resize(const uint32 width, const uint32 height);

    /**
     * @brief Occurs when the camera is updated.
     * @param dt = Delta time.
     */
    void update(const float dt);

    /**
     * @brief Occurs when the camera is rendered.
     * @param target = The target to setup the camera view for.
     */
    void render(sf::RenderTarget& target);

    /**
     * @brief Track an entity until it dies or the camera mode was switched.
     * @param entity = The entity to track.
     */
    void trackEntity(Entity* entity);

    /**
     * @brief Get the view for the camera.
     * @return The cameras view.
     */
    sf::View getView() const { return m_view; }

    /**
     * @brief Get the current mode of the camera.
     * @return The current camera mode.
     */
    mode::CameraMode getMode() const { return m_mode; }

    /**
     * @brief Set the mode of the camera.
     * @param mode = The new camera mode.
     */
    void setMode(mode::CameraMode mode) { m_mode = mode; }

    /**
     * @brief Get the current location of the camera.
     * @return The location of the camera.
     */
    vec2f getLocation() const { return m_location; }

    /**
     * @brief Apply the keyboard controls to the cameras view.
     * @param dt = Delta time.
     */
    void applyKeyboardControls(const float dt);

private:

    /**
     * @brief The current camera control mode.
     */
    mode::CameraMode m_mode;

    /**
     * @brief The speed that the camera moves at.
     */
    real32 m_speed;

    /**
     * @brief The speed the camera zooms at.
     */
    real32 m_zoomSpeed;

    /**
     * @brief The location of the camera in free mode.
     */
    vec2f m_location;

    /**
     * @brief The current view of the camera.
     */
    sf::View m_view;

    /**
     * @brief The entity that the camera is currently tracking if any.
     */
    Entity* m_trackingEntity;
};

#endif // CAMERA_H_INCLUDE
