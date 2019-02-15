#include <fc_light/exception/exception.hpp>
#include <boost/exception/all.hpp>
#include <fc_light/io/sstream.hpp>
#include <fc_light/log/logger.hpp>
#include <fc_light/io/json.hpp>

#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>

#include <iostream>

namespace fc_light {
FC_LIGHT_REGISTER_EXCEPTIONS(
  (json_parse_exception)
  (rpc_command_parse_exception)
  (command_not_implemented_exception)
  (command_depreciated)
  (password_input_exception)
  (privkey_not_found_exception)
  (privkey_invalid_unlock)
  (internal_error_exception)
  (timeout_exception)
  (invalid_arg_exception)
  (parse_error_exception)
  (file_not_found_exception)
  (key_not_found_exception)
  (bad_cast_exception)
  (assert_exception)
  (encryption_exception)
  (null_optional)
  (overflow_exception)
  (underflow_exception)
  (divide_by_zero_exception)
  (out_of_range_exception)
  (eof_exception)
)

namespace detail {
class exception_impl {
public:
  std::string name;
  std::string what;
  int64_t code;
  log_messages elog;
};
}
}
FC_LIGHT_REFLECT(fc_light::detail::exception_impl, (name)(what)(code)(elog))

namespace fc_light {
   exception::exception( log_messages&& msgs, int64_t code,
                                    const std::string& name_value,
                                    const std::string& what_value )
   :my( new detail::exception_impl() )
   {
      my->code = code;
      my->what = what_value;
      my->name = name_value;
      my->elog = fc_light::move(msgs);
   }

   exception::exception(
      const log_messages& msgs,
      int64_t code,
      const std::string& name_value,
      const std::string& what_value )
   :my( new detail::exception_impl() )
   {
      my->code = code;
      my->what = what_value;
      my->name = name_value;
      my->elog = msgs;
   }

   unhandled_exception::unhandled_exception( log_message&& m, std::exception_ptr e )
   :exception( fc_light::move(m) )
   {
      _inner = e;
   }
   unhandled_exception::unhandled_exception( const exception& r )
   :exception(r)
   {
   }
   unhandled_exception::unhandled_exception( log_messages m )
   :exception()
   { my->elog = fc_light::move(m); }

   std::exception_ptr unhandled_exception::get_inner_exception()const { return _inner; }

   NO_RETURN void     unhandled_exception::dynamic_rethrow_exception()const
   {
      if( !(_inner == std::exception_ptr()) ) std::rethrow_exception( _inner );
      else { fc_light::exception::dynamic_rethrow_exception(); }
   }

   std::shared_ptr<exception> unhandled_exception::dynamic_copy_exception()const
   {
      auto e = std::make_shared<unhandled_exception>( *this );
      e->_inner = _inner;
      return e;
   }

   exception::exception( int64_t code,
                         const std::string& name_value,
                         const std::string& what_value )
   :my( new detail::exception_impl() )
   {
      my->code = code;
      my->what = what_value;
      my->name = name_value;
   }

   exception::exception( log_message&& msg,
                         int64_t code,
                         const std::string& name_value,
                         const std::string& what_value )
   :my( new detail::exception_impl() )
   {
      my->code = code;
      my->what = what_value;
      my->name = name_value;
      my->elog.push_back( fc_light::move( msg ) );
   }
   exception::exception( const exception& c )
   :my( new detail::exception_impl(*c.my) )
   { }
   exception::exception( exception&& c )
   :my( fc_light::move(c.my) ){}

   const char*  exception::name()const throw() { return my->name.c_str(); }
   const char*  exception::what()const throw() { return my->what.c_str(); }
   int64_t      exception::code()const throw() { return my->code;         }

   exception::~exception(){}

   void to_variant( const exception& e, variant& v )
   {
      v = mutable_variant_object( "code", e.code() )
                                ( "name", e.name() )
                                ( "message", e.what() )
                                ( "stack", e.get_log() );

   }
   void          from_variant( const variant& v, exception& ll )
   {
      auto obj = v.get_object();
      if( obj.contains( "stack" ) )
         ll.my->elog =  obj["stack"].as<log_messages>();
      if( obj.contains( "code" ) )
         ll.my->code = obj["code"].as_int64();
      if( obj.contains( "name" ) )
         ll.my->name = obj["name"].as_string();
      if( obj.contains( "message" ) )
         ll.my->what = obj["message"].as_string();
   }

   const log_messages&   exception::get_log()const { return my->elog; }
   void                  exception::append_log( log_message m )
   {
      my->elog.emplace_back( fc_light::move(m) );
   }

   /**
    *   Generates a detailed string including file, line, method,
    *   and other information that is generally only useful for
    *   developers.
    */
   string exception::to_detail_string( log_level ll )const
   {
      fc_light::stringstream ss;
      ss << variant(my->code).as_string() <<" " << my->name << ": " <<my->what<<"\n";
      for( auto itr = my->elog.begin(); itr != my->elog.end();  )
      {
         ss << itr->get_message() <<"\n"; //fc_light::format_string( itr->get_format(), itr->get_data() ) <<"\n";
         ss << "    " << json::to_string( itr->get_data() )<<"\n";
         ss << "    " << itr->get_context().to_string();
         ++itr;
         if( itr != my->elog.end() ) ss<<"\n";
      }
      return ss.str();
   }

   std::string exception::to_detail_json_string( log_level ll )const
   {
     return fc_light::json::to_pretty_string(my);
   }

   /**
    *   Generates a user-friendly error report.
    */
   string exception::to_string( log_level ll )const
   {
      fc_light::stringstream ss;
      ss << exception::what() << ":";
      bool first_occur = true;
      for( auto itr = my->elog.begin(); itr != my->elog.end(); ++itr )
      {
        if (first_occur)
        {
          first_occur = false;
          ss << " ";
        }
        else
          ss << " => ";
        if( itr->get_format().size() )
          ss << fc_light::format_string( itr->get_format(), itr->get_data() );
      }
      return ss.str();
   }

   void NO_RETURN exception_factory::rethrow( const exception& e )const
   {
      auto itr = _registered_exceptions.find( e.code() );
      if( itr != _registered_exceptions.end() )
         itr->second->rethrow( e );
      throw e;
   }
   /**
    * Rethrows the exception restoring the proper type based upon
    * the error code.  This is used to propagate exception types
    * across conversions to/from JSON
    */
   NO_RETURN void  exception::dynamic_rethrow_exception()const
   {
      exception_factory::instance().rethrow( *this );
   }

   exception_ptr exception::dynamic_copy_exception()const
   {
       return std::make_shared<exception>(*this);
   }

   fc_light::string except_str()
   {
       return boost::current_exception_diagnostic_information();
   }

   void throw_bad_enum_cast( int64_t i, const char* e )
   {
      FC_LIGHT_THROW_EXCEPTION( bad_cast_exception,
                          "invalid index '${key}' in enum '${enum}'",
                          ("key",i)("enum",e) );
   }
   void throw_bad_enum_cast( const char* k, const char* e )
   {
      FC_LIGHT_THROW_EXCEPTION( bad_cast_exception,
                          "invalid name '${key}' in enum '${enum}'",
                          ("key",k)("enum",e) );
   }

   bool assert_optional(bool is_valid )
   {
      if( !is_valid )
         throw null_optional();
      return true;
   }
   exception& exception::operator=( const exception& copy )
   {
      *my = *copy.my;
      return *this;
   }

   exception& exception::operator=( exception&& copy )
   {
      my = std::move(copy.my);
      return *this;
   }

   void record_assert_trip(
      const char* filename,
      uint32_t lineno,
      const char* expr
      )
   {
      fc_light::mutable_variant_object assert_trip_info =
         fc_light::mutable_variant_object()
         ("source_file", filename)
         ("source_lineno", lineno)
         ("expr", expr)
         ;
      std::cout
         << "FC_LIGHT_ASSERT triggered:  "
         << fc_light::json::to_string( assert_trip_info ) << "\n";
      return;
   }

   bool enable_record_assert_trip = false;

} // fc


