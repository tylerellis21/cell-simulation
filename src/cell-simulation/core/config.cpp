#include "config.h"

int Config::m_width = 1024;
int Config::m_height = 768;
int Config::m_fps = 60;
int Config::m_aaLevel = 0;
bool Config::m_vsync = false;
bool Config::m_fullscreen = false;

void Config::load(std::string configFile)
{
    std::ifstream input(configFile.c_str());

    if (!input.is_open()) {
        Log::error(std::string("couldn't open config file! path: ").append(configFile));
        return;
    }

    picojson::value config;
    std::string error = picojson::parse(config, input);
    if (!error.empty()) {
        Log::error(std::string("couldn't parse config file! Error: ").append(error));
        return;
    }

    m_width = (int) config.get("window_width").get<double>();
    m_height = (int) config.get("window_height").get<double>();
    m_aaLevel = (int) config.get("antialiasing").get<double>();
    m_fps = (int) config.get("fps").get<double>();
    m_fullscreen = config.get("fullscreen").get<bool>();
    m_vsync = config.get("vsync").get<bool>();

    input.close();
}

void Config::save(std::string configFile)
{
    std::ofstream output(configFile.c_str());

    if (!output.is_open()) {
        return;
    }

    picojson::object config;

    config["window_width"] = picojson::value((double) m_width);
    config["window_height"] = picojson::value((double) m_height);
    config["fps"] = picojson::value((double) m_fps);
    config["vsync"] = picojson::value(m_vsync);
    config["fullscreen"] = picojson::value(m_fullscreen);
    config["antialiasing"] = picojson::value((double)m_aaLevel);
    //pass true to serialize in a neat readable format.
    output << picojson::value(config).serialize(true) << std::endl;

    output.flush();
    output.close();
}
