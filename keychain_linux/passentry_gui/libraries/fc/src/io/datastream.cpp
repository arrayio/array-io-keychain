#include <fc/io/datastream.hpp>
#include <fc/exception/exception.hpp>

NO_RETURN void fc_keychain::detail::throw_datastream_range_error(char const* method, size_t len, int64_t over)
{
  FC_THROW_EXCEPTION( out_of_range_exception, "${method} datastream of length ${len} over by ${over}", ("method",fc_keychain::string(method))("len",len)("over",over) );
}
