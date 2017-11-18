#include <core/configparser.h>

#include <fstream>
#include <sstream>

namespace phyr {

// RenderConfig definitions
const std::unordered_map<std::string, std::vector<ParamType>>
RenderConfig::SupportedConfigMap = RenderConfig::createSupportedConfigMap();

template <>
void RenderConfig::addConfigParam(const std::string& key, const int& pval) {
    addConfigParam(key, Param<int>(pval, ParamType::INT));
}

template <>
void RenderConfig::addConfigParam(const std::string& key, const Real& pval) {
    addConfigParam(key, Param<Real>(pval, ParamType::REAL));
}

template <>
void RenderConfig::addConfigParam(const std::string& key, const std::string& pval) {
    addConfigParam(key, Param<std::string>(pval, ParamType::STRING));
}

bool RenderConfig::parseConfig(const std::string& filename) {
    // Clear previously stored configurations if any
    configMap.clear();

    // Try opening the config file
    std::ifstream stream(filename);
    std::string line;

    if (!stream.is_open()) {
        LOG_ERR_FMT("Unable to open file \"%s\".", filename.c_str());
        return false;
    }

#define PARAM_MISMATCH_THROW(stream, key) \
    if ((stream).fail()) \
        throw MalformedConfigException((key));

    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        std::stringstream lineStream(line);

        std::string key; lineStream >> key;
        if (SupportedConfigMap.count(key) == 0)
            throw UnsupportedConfigException(key);

        // Get supported parameters for {key}
        const std::vector<ParamType>& typeList = SupportedConfigMap.at(key);
        for (ParamType type : typeList) {
            switch (type) {
                case ParamType::INT: {
                    int val; lineStream >> val;
                    PARAM_MISMATCH_THROW(lineStream, key);
                    addConfigParam(key, val);
                } break;

                case ParamType::REAL: {
                    Real val; lineStream >> val;
                    PARAM_MISMATCH_THROW(lineStream, key);
                    addConfigParam(key, val);
                } break;

                case ParamType::STRING: {
                    std::string str; lineStream >> str;
                    PARAM_MISMATCH_THROW(lineStream, key);
                    addConfigParam(key, str);
                } break;
            }
        }
    }

#undef PARAM_MISMATCH_THROW

    if (stream.bad()) {
        LOG_ERR_FMT("Error while reading file \"%s\".", filename.c_str());
        return false;
    }

    return true;
}

}  // namespace phyr
