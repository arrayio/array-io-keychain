#include "logger.hpp"

src::severity_logger< severity_level > lg;

void init()
{
    logging::add_file_log
            (
                    keywords::file_name = "sample_%N.log",
                    keywords::rotation_size = 10 * 1024 * 1024,
                    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                    keywords::format = "[%TimeStamp%]: %Message%"
            );

    logging::core::get()->set_filter
            (
                    logging::trivial::severity >= logging::trivial::info
            );
}
