#include <fc_keychain/log/appender.hpp>
#include <fc_keychain/log/logger.hpp>
#include <fc_keychain/thread/unique_lock.hpp>
#include <unordered_map>
#include <string>
#include <fc_keychain/thread/spin_lock.hpp>
#include <fc_keychain/thread/scoped_lock.hpp>
#include <fc_keychain/log/console_appender.hpp>
//#include <fc_keychain/log/file_appender.hpp>
//#include <fc_keychain/log/gelf_appender.hpp>
#include <fc_keychain/variant.hpp>
#include "console_defines.h"


namespace fc_keychain {

   std::unordered_map<std::string,appender::ptr>& get_appender_map() {
     static std::unordered_map<std::string,appender::ptr> lm;
     return lm;
   }
   std::unordered_map<std::string,appender_factory::ptr>& get_appender_factory_map() {
     static std::unordered_map<std::string,appender_factory::ptr> lm;
     return lm;
   }
   appender::ptr appender::get( const fc_keychain::string& s ) {
     static fc_keychain::spin_lock appender_spinlock;
      scoped_lock<spin_lock> lock(appender_spinlock);
      return get_appender_map()[s];
   }
   bool  appender::register_appender( const fc_keychain::string& type, const appender_factory::ptr& f )
   {
      get_appender_factory_map()[type] = f;
      return true;
   }
   appender::ptr appender::create( const fc_keychain::string& name, const fc_keychain::string& type, const variant& args  )
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
   
/*   static bool reg_console_appender = appender::register_appender<console_appender>( "console" );
   static bool reg_file_appender = appender::register_appender<file_appender>( "file" );
   static bool reg_gelf_appender = appender::register_appender<gelf_appender>( "gelf" );
*/
} // namespace fc_keychain
