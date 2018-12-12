#include "keychain_logger.hpp"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <fstream>

#include <keychain.hpp>


logger_singletone::logger_singletone(std::string postfix)
{

    typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

    // Create a text file sink
    boost::shared_ptr< file_sink > sink(new file_sink(
            keywords::file_name = std::string(LOG_DEFAULT_PATH)+  "/%Y%m%d_%H%M%S"+postfix+".log",
            keywords::rotation_size = 16384,
            keywords::auto_flush = true
    ));

    sink->set_formatter
            (
                    expr::format("%1% %2% %3%")
                    % expr::attr< boost::posix_time::ptime >("TimeStamp")
                    % expr::attr< severity_level >("Severity")
                    % expr::message
            );

    // Add the sink to the core
    logging::add_common_attributes();

    logging::core::get()->add_sink(sink);

}

const logger_singletone& logger_singletone::instance(std::string postfix)
{
    static const logger_singletone instance(postfix);
    return instance;
}

