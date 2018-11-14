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
    logger_singletone();
    static const logger_singletone& instance();
    src::severity_logger< severity_level > lg;
};

#endif //KEYCHAIN_LOGGER