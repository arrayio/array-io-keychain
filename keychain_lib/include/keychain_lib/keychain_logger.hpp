#ifndef KEYCHAIN_LOGGER
#define KEYCHAIN_LOGGER

#include <boost/log/trivial.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

using namespace logging::trivial;


class logger_singletone
{
public:
    static const logger_singletone& instance(std::string = "_keychain");
    src::severity_logger< severity_level > lg;
private:
    logger_singletone(std::string);
};

#endif //KEYCHAIN_LOGGER