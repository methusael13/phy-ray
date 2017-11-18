#ifndef PHYRAY_CORE_CONFIGPARSER_H
#define PHYRAY_CORE_CONFIGPARSER_H

#include <core/phyr.h>

#include <unordered_map>

namespace phyr {

enum ParamType { INT, REAL, STRING };

class ParamAbstract {
  public:
    ParamAbstract(ParamType type) : type(type) {}
    ParamType type;
};

template <typename T>
class Param : public ParamAbstract {
  public:
    Param(T value, ParamType type) :
        ParamAbstract(type), value(value) {}
    T value;
};

class ConfigArgsList {
  public:
    ConfigArgsList() {}

    int getNextExpectedIndex() const { return argList.size(); }

    template <typename U>
    void addParam(const Param<U>& p) {
        ASSERT(idx < nArgs);
        argList.push_back(std::make_shared<Param<U>>(p.value, p.type));
    }

    template <typename U>
    Param<U> getParam(int idx) const {
        ASSERT(id < argList.size());
        return *static_cast<Param<U>*>(argList[idx].get());
    }

  private:
    std::vector<std::shared_ptr<ParamAbstract>> argList;
};

// Exception declarations
class UnsupportedConfigException : public std::exception {
  public:
    UnsupportedConfigException(const std::string& key) :
        key(key),
        msg("Unsupported configuration symbol [" + key + "]") {}

    const char* what() const throw() { return msg.c_str(); }

  private:
    const std::string key, msg;
};

class MalformedConfigException : public std::exception {
  public:
    MalformedConfigException(const std::string& key) :
        key(key),
        msg("Illegal parameters passed for [" + key + "]") {}

    const char* what() const throw() { return msg.c_str(); }

  private:
    const std::string key, msg;
};

class RenderConfig {
  public:
    // Interface
    bool parseConfig(const std::string& filename);

    bool getConfigArgs(const std::string& key, ConfigArgsList* args = nullptr) {
        if (SupportedConfigMap.count(key) == 0)
            throw UnsupportedConfigException(key);

        if (configMap.count(key) == 0)
            return false;
        else if (args)
            *args = configMap[key];
        return true;
    }

  private:
    std::unordered_map<std::string, ConfigArgsList> configMap;

    static const std::unordered_map<std::string, std::vector<ParamType>> SupportedConfigMap;

    static std::unordered_map<std::string, std::vector<ParamType>> createSupportedConfigMap() {
        std::unordered_map<std::string, std::vector<ParamType>> config;
        // Image resolution
        config["resolution"].push_back(ParamType::INT);
        config["resolution"].push_back(ParamType::INT);

        // Samples per pixel
        config["samples"].push_back(ParamType::INT);

        // Max-bounces
        config["bounces"].push_back(ParamType::INT);
        return config;
    }

    template <typename U>
    void addConfigParam(const std::string& key, const U& param) {}

    template <typename U>
    void addConfigParam(const std::string& key, const Param<U>& param) {
        int idx = configMap[key].getNextExpectedIndex();
        bool shouldAdd = idx < SupportedConfigMap.at(key).size();

        if (shouldAdd) configMap[key].addParam(param);
    }
};

}  // namespace phyr

#endif
