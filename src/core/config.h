#ifndef CONFIG_H_INCLUDE
#define CONFIG_H_INCLUDE

//Standard includes.
#include <string>

//Common library includes.
#include "../util/log.h"
#include "../util/picojson.h"

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

    // Antialiasing level
    static int m_aaLevel;

    // Width of the window.
    static int m_width;

    // Height of the window.
    static int m_height;

    // FPS we want to run at, -1 for no limit.
    static int m_fps;

    // True for full-screen, false for window.
    static bool m_fullscreen;

    // Whether we use vsync or not.
    static bool m_vsync;

}; //class Config

#endif // CONFIG_H_INCLUDE
