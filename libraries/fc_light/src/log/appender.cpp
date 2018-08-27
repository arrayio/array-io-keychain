#include <fc_light/log/appender.hpp>
#include <fc_light/log/logger.hpp>
#include <fc_light/thread/unique_lock.hpp>
#include <unordered_map>
#include <string>
#include <fc_light/thread/spin_lock.hpp>
#include <fc_light/thread/scoped_lock.hpp>
#include <fc_light/log/console_appender.hpp>
#include <fc_light/variant.hpp>
#include "console_defines.h"


namespace fc_light {

   std::unordered_map<std::string,appender::ptr>& get_appender_map() {
     static std::unordered_map<std::string,appender::ptr> lm;
     return lm;
   }
   std::unordered_map<std::string,appender_factory::ptr>& get_appender_factory_map() {
     static std::unordered_map<std::string,appender_factory::ptr> lm;
     return lm;
   }
   appender::ptr appender::get( const fc_light::string& s ) {
     static fc_light::spin_lock appender_spinlock;
      scoped_lock<spin_lock> lock(appender_spinlock);
      return get_appender_map()[s];
   }
   bool  appender::register_appender( const fc_light::string& type, const appender_factory::ptr& f )
   {
      get_appender_factory_map()[type] = f;
      return true;
   }
   appender::ptr appender::create( const fc_light::string& name, const fc_light::string& type, const variant& args  )
   {
      auto fact_itr = get_appender_factory_map().find(type);
      if( fact_itr == get_appender_factory_map().end() ) {
         //wlog( "Unknown appender type '%s'", type.c_str() );
         return appender::ptr();
      }
      auto ap = fact_itr->second->create( args );
      get_appender_map()[name] = ap;
      return ap;
   }
   

} // namespace fc_light
