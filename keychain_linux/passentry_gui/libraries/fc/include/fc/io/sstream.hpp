#pragma once
//#include <fc/io/iostream.hpp>
#include <iostream>
#include <fc/fwd.hpp>
#include <fc/string.hpp>
#include <memory>

namespace fc {

class stringstream : virtual public std::iostream {
    public:
      stringstream();
      stringstream( fc_light::string& s);
      stringstream( const fc_light::string& s);
      ~stringstream();

      fc_light::string str();
      void str(const fc_light::string& s);

      void clear();

      virtual bool     eof()const;
      virtual size_t   writesome( const char* buf, size_t len );
      virtual size_t   writesome( const std::shared_ptr<const char>& buf, size_t len, size_t offset );
      virtual size_t   readsome( char* buf, size_t len );
      virtual size_t   readsome( const std::shared_ptr<char>& buf, size_t len, size_t offset );
      virtual void     close();
      virtual void     flush();
              char     peek();

    private:
      class impl;
      fwd<impl, 392> my;
  };

}
