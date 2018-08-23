#pragma once
//#include <fc_keychain/io/iostream.hpp>
#include <iostream>
#include <fc_keychain/fwd.hpp>
#include <fc_keychain/string.hpp>
#include <memory>

namespace fc_keychain {

class stringstream : virtual public std::iostream {
    public:
      stringstream();
      stringstream( fc_keychain::string& s);
      stringstream( const fc_keychain::string& s);
      ~stringstream();

      fc_keychain::string str();
      void str(const fc_keychain::string& s);

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
