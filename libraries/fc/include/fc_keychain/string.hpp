#pragma once
#include <fc_keychain/utility.hpp>
#include <fc_keychain/fwd.hpp>
#include <fc_keychain/optional.hpp>

#ifndef USE_FC_KEYCHAIN_STRING
#include <string>
namespace fc_keychain
{
    typedef std::string string;

  int64_t  to_int64( const fc_keychain::string& );
  uint64_t to_uint64( const fc_keychain::string& );
  double   to_double( const fc_keychain::string& );
  fc_keychain::string to_string( double );
  fc_keychain::string to_string( uint64_t );
  fc_keychain::string to_string( int64_t );
  fc_keychain::string to_string( uint16_t );
  std::string to_pretty_string( int64_t );
  inline fc_keychain::string to_string( int32_t v ) { return to_string( int64_t(v) ); }
  inline fc_keychain::string to_string( uint32_t v ){ return to_string( uint64_t(v) ); }
#ifdef __APPLE__
  inline fc_keychain::string to_string( size_t s) { return to_string(uint64_t(s)); }
#endif

  typedef fc_keychain::optional<fc_keychain::string> ostring;
  class variant_object;
  fc_keychain::string format_string( const fc_keychain::string&, const variant_object& );
  fc_keychain::string trim( const fc_keychain::string& );
  fc_keychain::string to_lower( const fc_keychain::string& );
  string trim_and_normalize_spaces( const string& s );

  uint64_t parse_size( const string& s );
}

#else

/**
 *  There is debate about whether doing this is 'standard conforming', but 
 *  it works everywhere and enables the purpose of this library which is to 
 *  accelerate compiles while maintaining compatability.
 */
namespace std {
  template<class Char>
  struct char_traits;

  template<class T>
  class allocator;

  template<class Char, class Traits, class Allocator>
  class basic_string;

  typedef basic_string<char, char_traits<char>, allocator<char> > string;
}


namespace fc_keychain {
  /**
   * @brief wrapper on std::string
   *
   *  Including <string> results in 4000 lines of code
   *  that must be included to build your header.  This
   *  class hides all of those details while maintaining
   *  compatability with std::string. Using fc_keychain::string
   *  instead of std::string can accelerate compile times
   *  10x.
   *
   *  The implementation of this class is std::string, this header simply
   *  accelerates compile times.   fc_keychain::string is automatically convertable to / from
   *  std::string.
   */
  class string {
    public:
      typedef char*       iterator;
      typedef const char* const_iterator;
      enum { npos = size_t(-1) };
      //  static const size_t npos;// = -1;

      string();
      string( const std::string& s );
      string( std::string&& s );
      string( const string& c );
      string( string&& c );
      string( const char* c );
      string( const char* c, int s );
      string( const_iterator b, const_iterator e );
      ~string();

      operator std::string&();
      operator const std::string&()const;

      iterator begin();
      iterator end();

      const_iterator begin()const;
      const_iterator end()const;

      char&       operator[](size_t idx);
      const char& operator[](size_t idx)const;

      string& operator =( const string& c );
      string& operator =( string&& c );

      void    reserve( size_t );
      size_t  size()const;
      size_t  find( char c, size_t pos = 0 )const;
      size_t  find(const fc_keychain::string& str, size_t pos = 0) const;
      size_t  find(const char* s, size_t pos = 0) const;
      size_t  rfind( char c, size_t pos = npos )const;
      size_t  rfind( const char* c, size_t pos = npos )const;
      size_t  rfind( const fc_keychain::string& c, size_t pos = npos )const;
      size_t  find_first_of (const fc_keychain::string& str, size_t pos = 0) const;
      size_t  find_first_of (const char* s, size_t pos = 0) const;
      string& replace(size_t pos,  size_t len,  const fc_keychain::string& str);
      string& replace(size_t pos,  size_t len,  const char* s);

      void    resize( size_t s );
      void    clear();

      const char* c_str()const;
      char*       data();

      bool    operator == ( const char* s )const;
      bool    operator == ( const string& s )const;
      bool    operator != ( const string& s )const;

      friend bool operator < ( const string& a, const string& b );

      string& operator+=( const string& s );
      string& operator+=( char c );

      friend string operator + ( const string&, const string&  );
      friend string operator + ( const string&, char c );

      fc_keychain::string substr( size_t start, size_t len = fc_keychain::string::npos )const;
    
    private:
       fc_keychain::fwd<std::string,32> my;
  };

  int64_t  to_int64( const fc_keychain::string& );
  uint64_t to_uint64( const fc_keychain::string& );
  double   to_double( const fc_keychain::string& );
  fc_keychain::string to_string( double );
  fc_keychain::string to_string( uint64_t );
  fc_keychain::string to_string( int64_t );

  typedef fc_keychain::optional<fc_keychain::string> ostring;
  class variant_object;
  fc_keychain::string format_string( const fc_keychain::string&, const variant_object& );

} // namespace fc_keychain

#endif
