#ifndef KEYCHAIN_LOGGER
#define KEYCHAIN_LOGGER

#include <boost/log/trivial.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

using namespace logging::trivial;


class logger_singleton
{
public:
    static const logger_singleton& instance(std::string = "keychain_common");
    src::severity_logger< severity_level > lg;
private:
    logger_singleton(std::string);
};

#endif //KEYCHAIN_LOGGER