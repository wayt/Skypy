#ifndef CONFIGMGR_H_
# define CONFIGMGR_H_

#include "Singleton.hpp"

#include <map>
#include <string>

#define DEFAULT_CONFIG_FILE "../etc/skypy-server.conf"

class ConfigMgr : public Singleton<ConfigMgr>
{
public:
    explicit ConfigMgr();

    void setConfigFile(std::string const& configFile) { _configFile = configFile; }
    bool loadConfig(bool reload = false);
    void dumpConfig();
    std::string const& getConfigFile() const { return _configFile; }

    bool getBoolDefault(std::string const& key, bool def);
    int getIntDefault(std::string const& key, int def);
    std::string const& getStringDefault(std::string const& key, std::string const& def);

private:
    std::string _configFile;
    std::map<std::string, std::string> _stringConfigs;

    struct ValueCache
    {
        ValueCache() : bValue(false), iValue(0), sValue() {}
        bool bValue;
        int iValue;
        std::string sValue;
    };
    std::map<std::string, ValueCache> _valueCache;

};

#define sConfig ConfigMgr::instance()

#endif
