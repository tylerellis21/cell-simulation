#ifndef CONFIG_H_INCLUDE
#define CONFIG_H_INCLUDE

//Standard includes.
#include <string>

//Common library includes.
#include "../util/log.h"
#include "../util/picojson.h"

/**
 * @brief This holds the config values which may be loaded from the config.json file.
 */
class Config
{
public:

    /**
     * @brief load the settings from the json file.
     */
    static void load(std::string configFile);

    /**
     * @brief save the settings to the json file.
     */
    static void save(std::string configFile);

    static int getAALevel() { return m_aaLevel; }
    static int getWidth() { return m_width; }
    static int getHeight() { return m_height; }
    static int getFpsLimit() { return m_fps; }
    static bool getFullscreen() { return m_fullscreen; }
    static bool getVSync() { return m_vsync; }

    static void setWidth(int width) { m_width = width; }
    static void setHeight(int height) { m_height = height; }
    static void setFPSLimit(int fps) { m_fps = fps; }
    static void setFullscreen(bool fullscreen) { m_fullscreen = fullscreen; }
    static void setVSync(bool vsync) { m_vsync = vsync; }

private:

    /**
     * @brief The level of anti-aliasing for the renderer.
     */
    static int m_aaLevel;

    /**
     * @brief The default width of the window.
     */
    static int m_width;

    /**
     * @brief The default height of the window.
     */
    static int m_height;

    /**
     * @brief The fps to run the game at, (<=0 for no limit)
     */
    static int m_fps;

    /**
     * @brief The flag to specify if we use a fullscreen window.
     */
    static bool m_fullscreen;

    /**
     * @brief The flag to specify if we use vsync.
     */
    static bool m_vsync;

}; //class Config

#endif // CONFIG_H_INCLUDE
