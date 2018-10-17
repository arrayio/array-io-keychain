#pragma once
#include <fc_light/io/iostream.hpp>
#include <fc_light/fwd.hpp>

namespace fc_light {

  class stringstream : virtual public iostream {
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
