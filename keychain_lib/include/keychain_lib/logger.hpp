#ifndef KEYCHAIN_LOGGER
#define KEYCHAIN_LOGGER

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>


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