#pragma once
#include <fc_light/shared_ptr.hpp>
#include <fc_light/string.hpp>

namespace fc_light {
   class appender;
   class log_message;
   class variant;

   class appender_factory : public fc_light::retainable {
      public:
       typedef fc_light::shared_ptr<appender_factory> ptr;

       virtual ~appender_factory(){};
       virtual fc_light::shared_ptr<appender> create( const variant& args ) = 0;
   };

   namespace detail {
      template<typename T>
      class appender_factory_impl : public appender_factory {
        public:
           virtual fc_light::shared_ptr<appender> create( const variant& args ) {
              return fc_light::shared_ptr<appender>(new T(args));
           }
      };
   }

   class appender : public fc_light::retainable {
      public:
         typedef fc_light::shared_ptr<appender> ptr;

         template<typename T>
         static bool register_appender(const fc_light::string& type) {
            return register_appender( type, new detail::appender_factory_impl<T>() );
         }

         static appender::ptr create( const fc_light::string& name, const fc_light::string& type, const variant& args  );
         static appender::ptr get( const fc_light::string& name );
         static bool          register_appender( const fc_light::string& type, const appender_factory::ptr& f );

         virtual void log( const log_message& m ) = 0;
   };
}
