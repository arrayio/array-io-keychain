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

namespace bfs = boost::filesystem;

logger_singleton::logger_singleton(std::string postfix)
{
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    auto dir = bfs::path(getenv("HOME"));
    dir += "/";
    dir += bfs::path(LOG_DEFAULT_PATH);
#else
  bfs::path dir(LOG_DEFAULT_PATH);
#endif

    typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;
	std::string def_log_path = dir.c_str();
    // Create a text file sink
#ifdef _WIN32
	if (getenv("USERPROFILE") != NULL) {
		std::string user(getenv("USERPROFILE"));
		if (!user.empty()) {
			def_log_path = user.append("\\AppData\\Local\\Keychain\\").append(def_log_path);
		}
	}
	else {
		if (getenv("SystemRoot") != NULL) {
			std::string logFolder(getenv("SystemRoot"));
			if (!logFolder.empty()) {
				def_log_path = logFolder.append("\\Logs\\Keychain\\").append(def_log_path);
			}
		}
		else
		{
			def_log_path = std::string(".\\").append(def_log_path);
		}
	}
#endif //_WIN32

    boost::shared_ptr< file_sink > sink(new file_sink(
            keywords::file_name = def_log_path +  "/%Y%m%d_%H%M%S_"+postfix+".log",
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

logger_singleton& logger_singleton::instance(std::string postfix)
{
    static  logger_singleton instance(postfix);
    return instance;
}

