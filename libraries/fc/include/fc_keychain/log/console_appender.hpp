#pragma once
#include <fc_keychain/log/appender.hpp>
#include <fc_keychain/log/logger.hpp>
#include <vector>

namespace fc_keychain
{
   class console_appender : public appender 
   {
       public:
            struct color 
            {
                enum type {
                   red,
                   green,
                   brown,
                   blue,
                   magenta,
                   cyan,
                   white,
                   console_default,
                };
            };

            struct stream { enum type { std_out, std_error }; };

            struct level_color 
            {
               level_color( log_level l=log_level::all, 
                            color::type c=color::console_default )
               :level(l),color(c){}

               log_level                         level;
               console_appender::color::type     color;
            };

            struct config 
            {
               config()
               :format( "${timestamp} ${thread_name} ${context} ${file}:${line} ${method} ${level}]  ${message}" ),
                stream(console_appender::stream::std_error),flush(true){}

               fc_keychain::string                         format;
               console_appender::stream::type     stream;
               std::vector<level_color>           level_colors;
               bool                               flush;
            };


            console_appender( const variant& args );
            console_appender( const config& cfg );
            console_appender();

            ~console_appender();
            virtual void log( const log_message& m );
            
            void print( const std::string& text_to_print, 
                        color::type text_color = color::console_default );

            void configure( const config& cfg );

       private:
            class impl;
            std::unique_ptr<impl> my;
   };
} // namespace fc_keychain

#include <fc_keychain/reflect/reflect.hpp>
FC_KEYCHAIN_REFLECT_ENUM( fc_keychain::console_appender::stream::type, (std_out)(std_error) )
FC_KEYCHAIN_REFLECT_ENUM( fc_keychain::console_appender::color::type, (red)(green)(brown)(blue)(magenta)(cyan)(white)(console_default) )
FC_KEYCHAIN_REFLECT( fc_keychain::console_appender::level_color, (level)(color) )
FC_KEYCHAIN_REFLECT( fc_keychain::console_appender::config, (format)(stream)(level_colors)(flush) )
