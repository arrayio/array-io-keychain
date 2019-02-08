#pragma once
/**
 *  @file exception.hpp
 *  @brief Defines exception's used by fc
 */
#include <fc_light/log/logger.hpp>
#include <fc_light/optional.hpp>
#include <exception>
#include <functional>
#include <unordered_map>

namespace fc_light
{
   namespace detail { class exception_impl; }

   enum exception_code_te
   {
     /** for exceptions we threw that don't have an assigned code */
     unspecified_exception_code        = 0, ///< default type, practically unused
     json_parse_error_code             , ///< error in json format of rpc command
     rpc_command_parse_code            , ///< error while parsing structure of rpc command (invalid set of fields or invalid value type)
     command_not_implemented_code      , ///< command is not implemented
     command_depreciated_code          , ///< command is depreciates
     invalid_arg_exception_code        , ///< invalid command arguments
     privkey_not_found_code            , ///< private key not found by public key
     privkey_invalid_unlock_code       , ///< cannot unlock private key, possible wrong password
     password_input_error_code         , ///< error while getting password
     declined_by_user_code             , ///< user does not approve transaction (decline or cancel has been pressed)
     internal_error_code               , ///< some unspecified internal error
     
     //internal errors
     parse_error_exception_code        ,
     timeout_exception_code            ,
     file_not_found_exception_code     ,
     key_not_found_exception_code      ,
     bad_cast_exception_code           ,
     assert_exception_code             ,
     encryption_error_code             ,
     null_optional_code                ,
     overflow_code                     ,
     underflow_code                    ,
     divide_by_zero_code               ,
     out_of_range_exception_code       ,
     eof_exception_code                ,
     
     //3d party exceptions
     std_exception_code                , ///< for std::exceptions (3rd party)
     unhandled_exception_code            ///< for unhandled 3rd party exceptions
   };

   /**
    *  @brief Used to generate a useful error report when an exception is thrown.
    *  @ingroup serializable
    *
    *  At each level in the stack where the exception is caught and rethrown a
    *  new log_message is added to the exception.
    *
    *  exception's are designed to be serialized to a variant and
    *  deserialized from an variant.
    *
    *  @see FC_LIGHT_THROW_EXCEPTION
    *  @see FC_LIGHT_RETHROW_EXCEPTION
    *  @see FC_LIGHT_RETHROW_EXCEPTIONS
    */
   class exception
   {
      public:
         enum code_enum
         {
            code_value = unspecified_exception_code
         };

         exception( int64_t code = unspecified_exception_code,
                    const std::string& name_value = "exception",
                    const std::string& what_value = "unspecified");
         exception( log_message&&, int64_t code = unspecified_exception_code,
                                   const std::string& name_value = "exception",
                                   const std::string& what_value = "unspecified");
         exception( log_messages&&, int64_t code = unspecified_exception_code,
                                    const std::string& name_value = "exception",
                                    const std::string& what_value = "unspecified");
         exception( const log_messages&,
                    int64_t code = unspecified_exception_code,
                    const std::string& name_value = "exception",
                    const std::string& what_value = "unspecified");
         exception( const exception& e );
         exception( exception&& e );
         virtual ~exception();

         const char*          name()const throw();
         int64_t              code()const throw();
         virtual const char*  what()const throw();

         /**
          *   @return a reference to log messages that have
          *   been added to this log.
          */
         const log_messages&  get_log()const;
         void                 append_log( log_message m );

         /**
          *   Generates a detailed string including file, line, method,
          *   and other information that is generally only useful for
          *   developers.
          */
         std::string to_detail_string( log_level ll = log_level::all )const;

         /**
          *   Generates a user-friendly error report.
          */
         std::string to_string( log_level ll = log_level::info  )const;

         /**
          *  Throw this exception as its most derived type.
          *
          *  @note does not return.
          */
         virtual NO_RETURN void     dynamic_rethrow_exception()const;

         /**
          *  This is equivalent to:
          *  @code
          *   try { throwAsDynamic_exception(); }
          *   catch( ... ) { return std::current_exception(); }
          *  @endcode
          */
          virtual std::shared_ptr<exception> dynamic_copy_exception()const;

         friend void to_variant( const exception& e, variant& v );
         friend void from_variant( const variant& e, exception& ll );

         exception& operator=( const exception& copy );
         exception& operator=( exception&& copy );
      protected:
         std::unique_ptr<detail::exception_impl> my;
   };

   void to_variant( const exception& e, variant& v );
   void from_variant( const variant& e, exception& ll );
   typedef std::shared_ptr<exception> exception_ptr;

   typedef optional<exception> oexception;


   /**
    *  @brief re-thrown whenever an unhandled exception is caught.
    *  @ingroup serializable
    *  Any exceptions thrown by 3rd party libraries that are not
    *  caught get wrapped in an unhandled_exception exception.
    *
    *  The original exception is captured as a std::exception_ptr
    *  which may be rethrown.  The std::exception_ptr does not
    *  propgate across process boundaries.
    */
   class unhandled_exception : public exception
   {
      public:
       enum code_enum {
          code_value = unhandled_exception_code,
       };
       unhandled_exception( log_message&& m, std::exception_ptr e = std::current_exception() );
       unhandled_exception( log_messages );
       unhandled_exception( const exception&  );

       std::exception_ptr get_inner_exception()const;

       virtual NO_RETURN void               dynamic_rethrow_exception()const;
       virtual std::shared_ptr<exception>   dynamic_copy_exception()const;
      private:
       std::exception_ptr _inner;
   };

   template<typename T>
   fc_light::exception_ptr copy_exception( T&& e )
   {
#if defined(_MSC_VER) && (_MSC_VER < 1700)
     return std::make_shared<unhandled_exception>( log_message(),
                                                   std::copy_exception(fc_light::forward<T>(e)) );
#else
     return std::make_shared<unhandled_exception>( log_message(),
                                                   std::make_exception_ptr(fc_light::forward<T>(e)) );
#endif
   }


   class exception_factory
   {
      public:
        struct base_exception_builder
        {
           virtual NO_RETURN void rethrow( const exception& e )const = 0;
        };

        template<typename T>
        struct exception_builder : public base_exception_builder
        {
           virtual NO_RETURN void rethrow( const exception& e )const override
           {
              throw T( e );
           }
        };

        template<typename T>
        void register_exception()
        {
           static exception_builder<T> builder;
           auto itr = _registered_exceptions.find( T::code_value );
           assert( itr == _registered_exceptions.end() );
           (void)itr; // in release builds this hides warnings
           _registered_exceptions[T::code_value] = &builder;
        }

        void NO_RETURN rethrow( const exception& e )const;

        static exception_factory& instance()
        {
           static exception_factory once;
           return once;
        }

      private:
        std::unordered_map<int64_t,base_exception_builder*> _registered_exceptions;
   };
#define FC_LIGHT_REGISTER_EXCEPTION(r, unused, base) \
   fc_light::exception_factory::instance().register_exception<base>();

#define FC_LIGHT_REGISTER_EXCEPTIONS( SEQ )\
     \
   static bool exception_init = []()->bool{ \
    BOOST_PP_SEQ_FOR_EACH( FC_LIGHT_REGISTER_EXCEPTION, v, SEQ )  \
      return true; \
   }();  \


#define FC_LIGHT_DECLARE_DERIVED_EXCEPTION( TYPE, BASE, CODE, WHAT ) \
   class TYPE : public BASE  \
   { \
      public: \
       enum code_enum { \
          code_value = CODE, \
       }; \
       explicit TYPE( int64_t code, const std::string& name_value, const std::string& what_value ) \
       :BASE( code, name_value, what_value ){} \
       explicit TYPE( fc_light::log_message&& m, int64_t code, const std::string& name_value, const std::string& what_value ) \
       :BASE( std::move(m), code, name_value, what_value ){} \
       explicit TYPE( fc_light::log_messages&& m, int64_t code, const std::string& name_value, const std::string& what_value )\
       :BASE( std::move(m), code, name_value, what_value ){}\
       explicit TYPE( const fc_light::log_messages& m, int64_t code, const std::string& name_value, const std::string& what_value )\
       :BASE( m, code, name_value, what_value ){}\
       TYPE( const std::string& what_value, const fc_light::log_messages& m ) \
       :BASE( m, CODE, BOOST_PP_STRINGIZE(TYPE), what_value ){} \
       TYPE( fc_light::log_message&& m ) \
       :BASE( fc_light::move(m), CODE, BOOST_PP_STRINGIZE(TYPE), WHAT ){}\
       TYPE( fc_light::log_messages msgs ) \
       :BASE( fc_light::move( msgs ), CODE, BOOST_PP_STRINGIZE(TYPE), WHAT ) {} \
       TYPE( const TYPE& c ) \
       :BASE(c){} \
       TYPE( const BASE& c ) \
       :BASE(c){} \
       TYPE():BASE(CODE, BOOST_PP_STRINGIZE(TYPE), WHAT){}\
       \
       virtual std::shared_ptr<fc_light::exception> dynamic_copy_exception()const\
       { return std::make_shared<TYPE>( *this ); } \
       virtual NO_RETURN void     dynamic_rethrow_exception()const \
       { if( code() == CODE ) throw *this;\
         else fc_light::exception::dynamic_rethrow_exception(); \
       } \
   };

  #define FC_LIGHT_DECLARE_EXCEPTION( TYPE, CODE, WHAT ) \
      FC_LIGHT_DECLARE_DERIVED_EXCEPTION( TYPE, fc_light::exception, CODE, WHAT )

  /**
   * @brief command not implemented
   */
  FC_LIGHT_DECLARE_EXCEPTION( command_not_implemented_exception, command_not_implemented_code, "Command not implemented" );

  /**
   * @brief cannot parse rpc command
   */
  FC_LIGHT_DECLARE_EXCEPTION( rpc_command_parse_exception, rpc_command_parse_code, "Command cannot be parsed" );

  /**
   * @brief cannot parse json format of rpc command
   */
  FC_LIGHT_DECLARE_EXCEPTION(json_parse_exception, json_parse_error_code, "Ivalid json format in input data");

  /**
   * @brief command not implemented
   */
  FC_LIGHT_DECLARE_EXCEPTION( command_depreciated, command_depreciated_code, "Command depreciated" );
  
  /**
   * @brief get password error
   */
  FC_LIGHT_DECLARE_EXCEPTION( password_input_exception, password_input_error_code, "Password input error");

  /**
   * @brief get password error
   */
  FC_LIGHT_DECLARE_EXCEPTION( privkey_not_found_exception, privkey_not_found_code, "Private key not found");

/**
 * @brief wrong private key password error
 */
  FC_LIGHT_DECLARE_EXCEPTION( privkey_invalid_unlock, privkey_invalid_unlock_code, "Cannot unlock private key");

  FC_LIGHT_DECLARE_EXCEPTION( timeout_exception, timeout_exception_code, "Timeout" );
  FC_LIGHT_DECLARE_EXCEPTION( file_not_found_exception, file_not_found_exception_code, "File not found" );
  /**
   * @brief report's parse errors
   */
  FC_LIGHT_DECLARE_EXCEPTION( parse_error_exception, parse_error_exception_code, "Parse error" );
  FC_LIGHT_DECLARE_EXCEPTION( invalid_arg_exception, invalid_arg_exception_code, "Invalid argument" );
  /**
   * @brief reports when a key, guid, or other item is not found.
   */
  FC_LIGHT_DECLARE_EXCEPTION( key_not_found_exception, key_not_found_exception_code, "Key not found" );
  FC_LIGHT_DECLARE_EXCEPTION( bad_cast_exception, bad_cast_exception_code, "Bad cast" );
  FC_LIGHT_DECLARE_EXCEPTION( out_of_range_exception, out_of_range_exception_code, "Out of range" );
  
  /**
   *  @brief used inplace of assert() to report violations of pre conditions.
   */
  FC_LIGHT_DECLARE_EXCEPTION( assert_exception, assert_exception_code, "Assert exception" );
  FC_LIGHT_DECLARE_EXCEPTION( eof_exception, eof_exception_code, "End of file" );
  FC_LIGHT_DECLARE_EXCEPTION( declined_by_user_exception, declined_by_user_code, "Operation canceled or declined by user")
  FC_LIGHT_DECLARE_EXCEPTION( null_optional, null_optional_code, "Null optional" );
  FC_LIGHT_DECLARE_EXCEPTION( encryption_exception, encryption_error_code, "Encryption error" );
  FC_LIGHT_DECLARE_EXCEPTION( internal_error_exception, internal_error_code, "Internal error" );
  FC_LIGHT_DECLARE_EXCEPTION( overflow_exception, overflow_code, "Integer overflow" );
  FC_LIGHT_DECLARE_EXCEPTION( underflow_exception, underflow_code, "Integer underflow" );
  FC_LIGHT_DECLARE_EXCEPTION( divide_by_zero_exception, divide_by_zero_code, "Integer divide by zero" );
  //  /* defined near assert_evaluator */ FC_LIGHT_DECLARE_EXCEPTION( transaction_dependency_exception, transaction_dependency_code, "Missing Transaction Dependency" );

  std::string except_str();

  void record_assert_trip(
     const char* filename,
     uint32_t lineno,
     const char* expr
     );

  extern bool enable_record_assert_trip;
} // namespace fc_light

#if __APPLE__
    #define LIKELY(x)    __builtin_expect((long)!!(x), 1L)
    #define UNLIKELY(x)  __builtin_expect((long)!!(x), 0L)
#else
    #define LIKELY(x)   (x)
    #define UNLIKELY(x) (x)
#endif

/**
 *@brief: Workaround for varying preprocessing behavior between MSVC and gcc
 */
#define FC_LIGHT_EXPAND_MACRO( x ) x
/**
 *  @brief Checks a condition and throws an assert_exception if the test is FALSE
 */
#define FC_LIGHT_ASSERT( TEST, ... ) \
  FC_LIGHT_EXPAND_MACRO( \
    FC_LIGHT_MULTILINE_MACRO_BEGIN \
      if( UNLIKELY(!(TEST)) ) \
      {                                                                      \
        if( fc_light::enable_record_assert_trip )                                  \
           fc_light::record_assert_trip( __FILE__, __LINE__, #TEST );              \
        FC_LIGHT_THROW_EXCEPTION( fc_light::assert_exception, #TEST ": "  __VA_ARGS__ ); \
      }                                                                      \
    FC_LIGHT_MULTILINE_MACRO_END \
  )

#define FC_LIGHT_CAPTURE_AND_THROW( EXCEPTION_TYPE, ... ) \
  FC_LIGHT_MULTILINE_MACRO_BEGIN \
    throw EXCEPTION_TYPE( FC_LIGHT_LOG_MESSAGE( error, "", FC_LIGHT_FORMAT_ARG_PARAMS(__VA_ARGS__) ) ); \
  FC_LIGHT_MULTILINE_MACRO_END

//#define FC_LIGHT_THROW( FORMAT, ... )
// FC_LIGHT_INDIRECT_EXPAND workas around a bug in Visual C++ variadic macro processing that prevents it
// from separating __VA_ARGS__ into separate tokens
#define FC_LIGHT_INDIRECT_EXPAND(MACRO, ARGS) MACRO ARGS
#define FC_LIGHT_THROW(  ... ) \
  FC_LIGHT_MULTILINE_MACRO_BEGIN \
    throw fc_light::exception( FC_LIGHT_INDIRECT_EXPAND(FC_LIGHT_LOG_MESSAGE, ( error, __VA_ARGS__ )) );  \
  FC_LIGHT_MULTILINE_MACRO_END

#define FC_LIGHT_EXCEPTION( EXCEPTION_TYPE, FORMAT, ... ) \
    EXCEPTION_TYPE( FC_LIGHT_LOG_MESSAGE( error, FORMAT, __VA_ARGS__ ) )
/**
 *  @def FC_LIGHT_THROW_EXCEPTION( EXCEPTION, FORMAT, ... )
 *  @param EXCEPTION a class in the Phoenix::Athena::API namespace that inherits
 *  @param format - a const char* string with "${keys}"
 */
#define FC_LIGHT_THROW_EXCEPTION( EXCEPTION, FORMAT, ... ) \
  FC_LIGHT_MULTILINE_MACRO_BEGIN \
    throw EXCEPTION( FC_LIGHT_LOG_MESSAGE( error, FORMAT, __VA_ARGS__ ) ); \
  FC_LIGHT_MULTILINE_MACRO_END


/**
 *  @def FC_LIGHT_RETHROW_EXCEPTION(ER,LOG_LEVEL,FORMAT,...)
 *  @brief Appends a log_message to the exception ER and rethrows it.
 */
#define FC_LIGHT_RETHROW_EXCEPTION( ER, LOG_LEVEL, FORMAT, ... ) \
  FC_LIGHT_MULTILINE_MACRO_BEGIN \
    ER.append_log( FC_LIGHT_LOG_MESSAGE( LOG_LEVEL, FORMAT, __VA_ARGS__ ) ); \
    throw; \
  FC_LIGHT_MULTILINE_MACRO_END

#define FC_LIGHT_LOG_AND_RETHROW( )  \
   catch( fc_light::exception& er ) { \
      wlog( "${details}", ("details",er.to_detail_string()) ); \
      FC_LIGHT_RETHROW_EXCEPTION( er, warn, "rethrow" ); \
   } catch( const std::exception& e ) {  \
      fc_light::exception fce( \
                FC_LIGHT_LOG_MESSAGE( warn, "rethrow ${what}: ", ("what",e.what())), \
                fc_light::std_exception_code,\
                typeid(e).name(), \
                e.what() ) ; \
      wlog( "${details}", ("details",fce.to_detail_string()) ); \
      throw fce;\
   } catch( ... ) {  \
      fc_light::unhandled_exception e( \
                FC_LIGHT_LOG_MESSAGE( warn, "rethrow"), \
                std::current_exception() ); \
      wlog( "${details}", ("details",e.to_detail_string()) ); \
      throw e; \
   }

#define FC_LIGHT_CAPTURE_LOG_AND_RETHROW( ... )  \
   catch( fc_light::exception& er ) { \
      wlog( "${details}", ("details",er.to_detail_string()) ); \
      wdump( __VA_ARGS__ ); \
      FC_LIGHT_RETHROW_EXCEPTION( er, warn, "rethrow", FC_LIGHT_FORMAT_ARG_PARAMS(__VA_ARGS__) ); \
   } catch( const std::exception& e ) {  \
      fc_light::exception fce( \
                FC_LIGHT_LOG_MESSAGE( warn, "rethrow ${what}: ", FC_LIGHT_FORMAT_ARG_PARAMS( __VA_ARGS__ )("what",e.what())), \
                fc_light::std_exception_code,\
                typeid(e).name(), \
                e.what() ) ; \
      wlog( "${details}", ("details",fce.to_detail_string()) ); \
      wdump( __VA_ARGS__ ); \
      throw fce;\
   } catch( ... ) {  \
      fc_light::unhandled_exception e( \
                FC_LIGHT_LOG_MESSAGE( warn, "rethrow", FC_LIGHT_FORMAT_ARG_PARAMS( __VA_ARGS__) ), \
                std::current_exception() ); \
      wlog( "${details}", ("details",e.to_detail_string()) ); \
      wdump( __VA_ARGS__ ); \
      throw e; \
   }

#define FC_LIGHT_CAPTURE_AND_LOG( ... )  \
   catch( fc_light::exception& er ) { \
      wlog( "${details}", ("details",er.to_detail_string()) ); \
      wdump( __VA_ARGS__ ); \
   } catch( const std::exception& e ) {  \
      fc_light::exception fce( \
                FC_LIGHT_LOG_MESSAGE( warn, "rethrow ${what}: ",FC_LIGHT_FORMAT_ARG_PARAMS( __VA_ARGS__  )("what",e.what()) ), \
                fc_light::std_exception_code,\
                typeid(e).name(), \
                e.what() ) ; \
      wlog( "${details}", ("details",fce.to_detail_string()) ); \
      wdump( __VA_ARGS__ ); \
   } catch( ... ) {  \
      fc_light::unhandled_exception e( \
                FC_LIGHT_LOG_MESSAGE( warn, "rethrow", FC_LIGHT_FORMAT_ARG_PARAMS( __VA_ARGS__) ), \
                std::current_exception() ); \
      wlog( "${details}", ("details",e.to_detail_string()) ); \
      wdump( __VA_ARGS__ ); \
   }


/**
 *  @def FC_LIGHT_RETHROW_EXCEPTIONS(LOG_LEVEL,FORMAT,...)
 *  @brief  Catchs all exception's, std::exceptions, and ... and rethrows them after
 *   appending the provided log message.
 */
#define FC_LIGHT_RETHROW_EXCEPTIONS( LOG_LEVEL, FORMAT, ... ) \
   catch( fc_light::exception& er ) { \
      FC_LIGHT_RETHROW_EXCEPTION( er, LOG_LEVEL, FORMAT, __VA_ARGS__ ); \
   } catch( const std::exception& e ) {  \
      fc_light::exception fce( \
                FC_LIGHT_LOG_MESSAGE( LOG_LEVEL, "${what}: " FORMAT,__VA_ARGS__("what",e.what())), \
                fc_light::std_exception_code,\
                typeid(e).name(), \
                e.what() ) ; throw fce;\
   } catch( ... ) {  \
      throw fc_light::unhandled_exception( \
                FC_LIGHT_LOG_MESSAGE( LOG_LEVEL, FORMAT,__VA_ARGS__), \
                std::current_exception() ); \
   }

#define FC_LIGHT_CAPTURE_AND_RETHROW( ... ) \
   catch( fc_light::exception& er ) { \
      FC_LIGHT_RETHROW_EXCEPTION( er, warn, "", FC_LIGHT_FORMAT_ARG_PARAMS(__VA_ARGS__) ); \
   } catch( const std::exception& e ) {  \
      fc_light::exception fce( \
                FC_LIGHT_LOG_MESSAGE( warn, "${what}: ",FC_LIGHT_FORMAT_ARG_PARAMS(__VA_ARGS__)("what",e.what())), \
                fc_light::std_exception_code,\
                typeid(e).name(), \
                e.what() ) ; throw fce;\
   } catch( ... ) {  \
      throw fc_light::unhandled_exception( \
                FC_LIGHT_LOG_MESSAGE( warn, "",FC_LIGHT_FORMAT_ARG_PARAMS(__VA_ARGS__)), \
                std::current_exception() ); \
   }

#define FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW( EXCEPTION, LOG_LEVEL, FORMAT, ... ) \
   catch( fc_light::exception& er ) { \
      EXCEPTION er_(er.get_log()); \
      er_.append_log( FC_LIGHT_LOG_MESSAGE( LOG_LEVEL, FORMAT, __VA_ARGS__ ) ); \
      throw er_; \
   } catch( const std::exception& e ) {  \
      fc_light::exception fce( \
                FC_LIGHT_LOG_MESSAGE( warn, "${what}: ",FC_LIGHT_FORMAT_ARG_PARAMS(__VA_ARGS__)("what",e.what())), \
                fc_light::std_exception_code,\
                typeid(e).name(), \
                e.what() ) ; throw fce;\
   } catch( ... ) {  \
      throw fc_light::unhandled_exception( \
                FC_LIGHT_LOG_MESSAGE( warn, "",FC_LIGHT_FORMAT_ARG_PARAMS(__VA_ARGS__)), \
                std::current_exception() ); \
   }

#include <fc_light/reflect/reflect.hpp>

FC_LIGHT_REFLECT_ENUM(
  fc_light::exception_code_te,
  (unspecified_exception_code)
  (json_parse_error_code)
  (rpc_command_parse_code)
  (command_not_implemented_code)
  (command_depreciated_code)
  (invalid_arg_exception_code)
  (privkey_not_found_code)
  (privkey_invalid_unlock_code)
  (password_input_error_code)
  (declined_by_user_code)
  (internal_error_code)

  (parse_error_exception_code)
  (timeout_exception_code)
  (file_not_found_exception_code)
  (key_not_found_exception_code)
  (bad_cast_exception_code)
  (assert_exception_code)
  (encryption_error_code)
  (null_optional_code)
  (overflow_code)
  (underflow_code)
  (divide_by_zero_code)
  (out_of_range_exception_code)
  (eof_exception_code)

  (std_exception_code)
  (unhandled_exception_code))