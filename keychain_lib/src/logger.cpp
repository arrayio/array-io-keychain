#include "logger.hpp"

logger_singletone::logger_singletone()
{
    logging::add_file_log
            (
                    keywords::file_name = "sample_%N.log",
                    keywords::rotation_size = 10 * 1024 * 1024,
                    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                    keywords::format = "[%TimeStamp%]: %Message%"
            );

    boost::shared_ptr< logging::core > core = logging::core::get();

    logging::add_common_attributes();

    // Create a backend and attach a couple of streams to it
    boost::shared_ptr< sinks::text_ostream_backend > backend =
            boost::make_shared< sinks::text_ostream_backend >();
    backend->add_stream(
            boost::shared_ptr< std::ostream >(new std::ofstream("sample.log")));
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

}

const logger_singletone& logger_singletone::instance()
{
    static const logger_singletone instance;
    return instance;
}

