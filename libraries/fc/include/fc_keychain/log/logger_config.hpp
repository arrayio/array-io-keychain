#pragma once
#include <fc_keychain/log/logger.hpp>

namespace fc_keychain {
   class path;
   struct appender_config {
      appender_config(const string& name = "",
                      const string& type = "", 
                      variant args = variant()) :
        name(name),
        type(type),
        args(fc_keychain::move(args)),
        enabled(true)
      {}
      string   name;
      string   type;
      variant  args;
      bool     enabled;
   };

   struct logger_config {
      logger_config(const fc_keychain::string& name = ""):name(name),enabled(true),additivity(false){}
      string                           name;
      ostring                          parent;
      /// if not set, then parents level is used.
      fc_keychain::optional<log_level>          level;
      bool                             enabled;
      /// if any appenders are sepecified, then parent's appenders are not set.
      bool                             additivity;
      std::vector<string>               appenders;

      logger_config& add_appender( const string& s );
   };

   struct logging_config {
      static logging_config default_config();
      std::vector<string>          includes;
      std::vector<appender_config> appenders;
      std::vector<logger_config>   loggers;
   };

   void configure_logging( const fc_keychain::path& log_config );
   bool configure_logging( const logging_config& l );
}

#include <fc_keychain/reflect/reflect.hpp>
FC_KEYCHAIN_REFLECT( fc_keychain::appender_config, (name)(type)(args)(enabled) )
FC_KEYCHAIN_REFLECT( fc_keychain::logger_config, (name)(parent)(level)(enabled)(additivity)(appenders) )
FC_KEYCHAIN_REFLECT( fc_keychain::logging_config, (includes)(appenders)(loggers) )
