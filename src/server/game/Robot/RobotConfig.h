#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H


#include "Define.h"
#include <string>
#include <vector>

class TC_COMMON_API RobotConfig
{
    RobotConfig() = default;
    RobotConfig(RobotConfig const&) = delete;
    RobotConfig& operator=(RobotConfig const&) = delete;
    ~RobotConfig() = default;

public:
    /// Method used only for loading main configuration files (authserver.conf and worldserver.conf)
    bool LoadInitial(std::string file, std::vector<std::string> args, std::string& error);
    bool LoadAdditionalFile(std::string file, bool keepOnReload, std::string& error);

    static RobotConfig* instance();

    bool Reload(std::vector<std::string>& errors);

    std::string GetStringDefault(std::string const& name, const std::string& def, bool quiet = false) const;
    bool GetBoolDefault(std::string const& name, bool def, bool quiet = false) const;
    int GetIntDefault(std::string const& name, int def, bool quiet = false) const;
    float GetFloatDefault(std::string const& name, float def, bool quiet = false) const;

    std::string const& GetFilename();
    std::vector<std::string> const& GetArguments() const;
    std::vector<std::string> GetKeysByString(std::string const& name);

private:
    template<class T>
    T GetValueDefault(std::string const& name, T def, bool quiet) const;

public:
    bool StartRobotSystem();
    uint32 Enable;
    std::string AccountNamePrefix;
    uint32 DPSDelay;
};

#define sRobotConfig RobotConfig::instance()	

#endif
