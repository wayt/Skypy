#include "ConfigMgr.h"
#include "Utils.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

ConfigMgr::ConfigMgr() : _stringConfigs(),
    _valueCache()
{ }

bool ConfigMgr::loadConfig(bool reload)
{
    std::ifstream file(_configFile);
    if (!file.is_open())
        return false;

    _stringConfigs.clear();

    std::string line;
    while (std::getline(file, line))
    {
        if (line[0] == '#') // Skip comment
            continue;

        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '='))
        {
            std::string value;
            if (std::getline(is_line, value))
            {
                value = Utils::trim(value);
                Utils::trim_quote(value);
                _stringConfigs[Utils::trim(key)] = value;
            }
        }
    }

    file.close();

    std::cout << "Configuration loaded" << std::endl;
    return true;
}

void ConfigMgr::dumpConfig()
{
    std::cout << "Config :" << std::endl;
    for (auto itr = _stringConfigs.begin(); itr != _stringConfigs.end(); ++itr)
        std::cout << "\t" << itr->first << " => " << itr->second << std::endl;
}

bool ConfigMgr::getBoolDefault(std::string const& key, bool def)
{
    { // Cache handle
        auto itr = _valueCache.find(key);
        if (itr != _valueCache.end())
            return itr->second.bValue;
    }

    auto itr = _stringConfigs.find(key);
    if (itr == _stringConfigs.end())
        return def;

    std::string str = itr->second;
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    bool value = (bool)Utils::to<int>(itr->second);
    if (str == "TRUE" || str == "YES")
        value = true;
    _valueCache[key].bValue = value;
    return value;
}

int ConfigMgr::getIntDefault(std::string const& key, int def)
{
    { // Cache handle
        auto itr = _valueCache.find(key);
        if (itr != _valueCache.end())
            return itr->second.iValue;
    }

    auto itr = _stringConfigs.find(key);
    if (itr == _stringConfigs.end())
        return def;

    int value = Utils::to<int>(itr->second);
    _valueCache[key].iValue = value;
    return value;
}

std::string const& ConfigMgr::getStringDefault(std::string const& key, std::string const& def)
{
    { // Cache handle
        auto itr = _valueCache.find(key);
        if (itr != _valueCache.end())
            return itr->second.sValue;
    }

    auto itr = _stringConfigs.find(key);
    if (itr == _stringConfigs.end())
        return def;

    std::string const& value = itr->second;
    _valueCache[key].sValue = value;
    return value;
}
