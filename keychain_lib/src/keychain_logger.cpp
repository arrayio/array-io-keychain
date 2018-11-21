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


logger_singletone::logger_singletone()
{
/*    logging::add_file_log
            (
                    keywords::file_name = "sample_%N.log",
                    keywords::rotation_size = 10 * 1024 * 1024,
                    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                    keywords::format = "[%TimeStamp%]: %Message%"
            );
*/

/*
    boost::shared_ptr< logging::core > core = logging::core::get();

    logging::add_common_attributes();

    // Create a backend and attach a couple of streams to it
    boost::shared_ptr< sinks::text_ostream_backend > backend =
            boost::make_shared< sinks::text_ostream_backend >();

    backend->add_stream( boost::shared_ptr< std::ostream >(new std::ofstream("sample.log")));
    // Enable auto-flushing after each log record written

    backend->auto_flush(true);

    typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
    boost::shared_ptr< sink_t > sink(new sink_t(backend));


    sink->set_formatter(
            expr::format("%1% %2% %3%")
            % expr::attr< boost::posix_time::ptime >("TimeStamp")
            % expr::attr< severity_level >("Severity")
            % expr::message
    );

    core->add_sink(sink);

*/
    typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

    // Create a text file sink
    boost::shared_ptr< file_sink > sink(new file_sink(
            keywords::file_name = std::string(LOG_DEFAULT_PATH)+  "/%Y%m%d_%H%M%S.log",
            keywords::rotation_size = 16384,
            keywords::auto_flush = true
    ));

/*
    // Set up where the rotated files will be stored
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
            keywords::target = "logs"
    ));

    // Upon restart, scan the directory for files matching the file_name pattern
    sink->locked_backend()->scan_for_files();
*/
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

const logger_singletone& logger_singletone::instance()
{
    static const logger_singletone instance;
    return instance;
}

