#pragma once
#include <fc_keychain/io/raw_variant.hpp>
#include <fc_keychain/reflect/reflect.hpp>
#include <fc_keychain/io/datastream.hpp>
#include <fc_keychain/io/varint.hpp>
#include <fc_keychain/optional.hpp>
#include <fc_keychain/fwd.hpp>
#include <fc_keychain/smart_ref_fwd.hpp>
#include <fc_keychain/array.hpp>
#include <fc_keychain/time.hpp>
#include <fc_keychain/filesystem.hpp>
#include <fc_keychain/exception/exception.hpp>
#include <fc_keychain/safe.hpp>
#include <fc_keychain/io/raw_fwd.hpp>
#include <map>
#include <deque>

namespace fc_keychain {
    namespace raw {

    template<typename Stream, typename Arg0, typename... Args>
    inline void pack( Stream& s, const Arg0& a0, Args... args ) {
       pack( s, a0 );
       pack( s, args... );
    }

    template<typename Stream>
    inline void pack( Stream& s, const fc_keychain::exception& e )
    {
       fc_keychain::raw::pack( s, e.code() );
       fc_keychain::raw::pack( s, std::string(e.name()) );
       fc_keychain::raw::pack( s, std::string(e.what()) );
       fc_keychain::raw::pack( s, e.get_log() );
    }
    template<typename Stream>
    inline void unpack( Stream& s, fc_keychain::exception& e )
    {
       int64_t code;
       std::string name, what;
       log_messages msgs;

       fc_keychain::raw::unpack( s, code );
       fc_keychain::raw::unpack( s, name );
       fc_keychain::raw::unpack( s, what );
       fc_keychain::raw::unpack( s, msgs );

       e = fc_keychain::exception( fc_keychain::move(msgs), code, name, what );
    }

    template<typename Stream>
    inline void pack( Stream& s, const fc_keychain::log_message& msg )
    {
       fc_keychain::raw::pack( s, variant(msg) );
    }
    template<typename Stream>
    inline void unpack( Stream& s, fc_keychain::log_message& msg )
    {
       fc_keychain::variant vmsg;
       fc_keychain::raw::unpack( s, vmsg );
       msg = vmsg.as<log_message>();
    }

    template<typename Stream>
    inline void pack( Stream& s, const fc_keychain::path& tp )
    {
       fc_keychain::raw::pack( s, tp.generic_string() );
    }

    template<typename Stream>
    inline void unpack( Stream& s, fc_keychain::path& tp )
    {
       std::string p;
       fc_keychain::raw::unpack( s, p );
       tp = p;
    }

    template<typename Stream>
    inline void pack( Stream& s, const fc_keychain::time_point_sec& tp )
    {
       uint32_t usec = tp.sec_since_epoch();
       s.write( (const char*)&usec, sizeof(usec) );
    }

    template<typename Stream>
    inline void unpack( Stream& s, fc_keychain::time_point_sec& tp )
    { try {
       uint32_t sec;
       s.read( (char*)&sec, sizeof(sec) );
       tp = fc_keychain::time_point() + fc_keychain::seconds(sec);
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "" ) }

    template<typename Stream>
    inline void pack( Stream& s, const fc_keychain::time_point& tp )
    {
       uint64_t usec = tp.time_since_epoch().count();
       s.write( (const char*)&usec, sizeof(usec) );
    }

    template<typename Stream>
    inline void unpack( Stream& s, fc_keychain::time_point& tp )
    { try {
       uint64_t usec;
       s.read( (char*)&usec, sizeof(usec) );
       tp = fc_keychain::time_point() + fc_keychain::microseconds(usec);
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "" ) }

    template<typename Stream>
    inline void pack( Stream& s, const fc_keychain::microseconds& usec )
    {
       uint64_t usec_as_int64 = usec.count();
       s.write( (const char*)&usec_as_int64, sizeof(usec_as_int64) );
    }

    template<typename Stream>
    inline void unpack( Stream& s, fc_keychain::microseconds& usec )
    { try {
       uint64_t usec_as_int64;
       s.read( (char*)&usec_as_int64, sizeof(usec_as_int64) );
       usec = fc_keychain::microseconds(usec_as_int64);
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "" ) }

    template<typename Stream, typename T, size_t N>
    inline void pack( Stream& s, const fc_keychain::array<T,N>& v) {
      s.write((const char*)&v.data[0],N*sizeof(T));
    }

    template<typename Stream, typename T>
    inline void pack( Stream& s, const std::shared_ptr<T>& v)
    {
      fc_keychain::raw::pack( s, *v );
    }

    template<typename Stream, typename T, size_t N>
    inline void unpack( Stream& s, fc_keychain::array<T,N>& v)
    { try {
      s.read((char*)&v.data[0],N*sizeof(T));
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "fc_keychain::array<type,length>", ("type",fc_keychain::get_typename<T>::name())("length",N) ) }

    template<typename Stream, typename T>
    inline void unpack( Stream& s, std::shared_ptr<T>& v)
    { try {
      v = std::make_shared<T>();
      fc_keychain::raw::unpack( s, *v );
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "std::shared_ptr<T>", ("type",fc_keychain::get_typename<T>::name()) ) }

    template<typename Stream> inline void pack( Stream& s, const signed_int& v ) {
      uint32_t val = (v.value<<1) ^ (v.value>>31);
      do {
        uint8_t b = uint8_t(val) & 0x7f;
        val >>= 7;
        b |= ((val > 0) << 7);
        s.write((char*)&b,1);//.put(b);
      } while( val );
    }

    template<typename Stream> inline void pack( Stream& s, const unsigned_int& v ) {
      uint64_t val = v.value;
      do {
        uint8_t b = uint8_t(val) & 0x7f;
        val >>= 7;
        b |= ((val > 0) << 7);
        s.write((char*)&b,1);//.put(b);
      }while( val );
    }

    template<typename Stream> inline void unpack( Stream& s, signed_int& vi ) {
      uint32_t v = 0; char b = 0; int by = 0;
      do {
        s.get(b);
        v |= uint32_t(uint8_t(b) & 0x7f) << by;
        by += 7;
      } while( uint8_t(b) & 0x80 );
      vi.value = ((v>>1) ^ (v>>31)) + (v&0x01);
      vi.value = v&0x01 ? vi.value : -vi.value;
      vi.value = -vi.value;
    }
    template<typename Stream> inline void unpack( Stream& s, unsigned_int& vi ) {
      uint64_t v = 0; char b = 0; uint8_t by = 0;
      do {
          s.get(b);
          v |= uint32_t(uint8_t(b) & 0x7f) << by;
          by += 7;
      } while( uint8_t(b) & 0x80 );
      vi.value = static_cast<uint32_t>(v);
    }

    template<typename Stream, typename T> inline void unpack( Stream& s, const T& vi )
    {
       T tmp;
       fc_keychain::raw::unpack( s, tmp );
       FC_KEYCHAIN_ASSERT( vi == tmp );
    }

    template<typename Stream> inline void pack( Stream& s, const char* v ) { fc_keychain::raw::pack( s, fc_keychain::string(v) ); }

    template<typename Stream, typename T>
    void pack( Stream& s, const safe<T>& v ) { fc_keychain::raw::pack( s, v.value ); }

    template<typename Stream, typename T>
    void unpack( Stream& s, fc_keychain::safe<T>& v ) { fc_keychain::raw::unpack( s, v.value ); }

    template<typename Stream, typename T, unsigned int S, typename Align>
    void pack( Stream& s, const fc_keychain::fwd<T,S,Align>& v ) {
       fc_keychain::raw::pack( *v );
    }

    template<typename Stream, typename T, unsigned int S, typename Align>
    void unpack( Stream& s, fc_keychain::fwd<T,S,Align>& v ) {
       fc_keychain::raw::unpack( *v );
    }
    template<typename Stream, typename T>
    void pack( Stream& s, const fc_keychain::smart_ref<T>& v ) { fc_keychain::raw::pack( s, *v ); }

    template<typename Stream, typename T>
    void unpack( Stream& s, fc_keychain::smart_ref<T>& v ) { fc_keychain::raw::unpack( s, *v ); }

    // optional
    template<typename Stream, typename T>
    void pack( Stream& s, const fc_keychain::optional<T>& v ) {
      fc_keychain::raw::pack( s, bool(!!v) );
      if( !!v ) fc_keychain::raw::pack( s, *v );
    }

    template<typename Stream, typename T>
    void unpack( Stream& s, fc_keychain::optional<T>& v )
    { try {
      bool b; fc_keychain::raw::unpack( s, b );
      if( b ) { v = T(); fc_keychain::raw::unpack( s, *v ); }
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "optional<${type}>", ("type",fc_keychain::get_typename<T>::name() ) ) }

    // std::vector<char>
    template<typename Stream> inline void pack( Stream& s, const std::vector<char>& value ) {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)value.size()) );
      if( value.size() )
        s.write( &value.front(), (uint32_t)value.size() );
    }
    template<typename Stream> inline void unpack( Stream& s, std::vector<char>& value ) {
      unsigned_int size; fc_keychain::raw::unpack( s, size );
      FC_KEYCHAIN_ASSERT( size.value < MAX_ARRAY_ALLOC_SIZE );
      value.resize(size.value);
      if( value.size() )
        s.read( value.data(), value.size() );
    }

    // fc_keychain::string
    template<typename Stream> inline void pack( Stream& s, const fc_keychain::string& v )  {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)v.size()));
      if( v.size() ) s.write( v.c_str(), v.size() );
    }

    template<typename Stream> inline void unpack( Stream& s, fc_keychain::string& v )  {
      std::vector<char> tmp; fc_keychain::raw::unpack(s,tmp);
      if( tmp.size() )
         v = fc_keychain::string(tmp.data(),tmp.data()+tmp.size());
      else v = fc_keychain::string();
    }

    // bool
    template<typename Stream> inline void pack( Stream& s, const bool& v ) { fc_keychain::raw::pack( s, uint8_t(v) );             }
    template<typename Stream> inline void unpack( Stream& s, bool& v )
    {
       uint8_t b;
       fc_keychain::raw::unpack( s, b );
       FC_KEYCHAIN_ASSERT( (b & ~1) == 0 );
       v=(b!=0);
    }

    namespace detail {

      template<typename Stream, typename Class>
      struct pack_object_visitor {
        pack_object_visitor(const Class& _c, Stream& _s)
        :c(_c),s(_s){}

        template<typename T, typename C, T(C::*p)>
        void operator()( const char* name )const {
          fc_keychain::raw::pack( s, c.*p );
        }
        private:
          const Class& c;
          Stream&      s;
      };

      template<typename Stream, typename Class>
      struct unpack_object_visitor {
        unpack_object_visitor(Class& _c, Stream& _s)
        :c(_c),s(_s){}

        template<typename T, typename C, T(C::*p)>
        inline void operator()( const char* name )const
        { try {
          fc_keychain::raw::unpack( s, c.*p );
        } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "Error unpacking field ${field}", ("field",name) ) }
        private:
          Class&  c;
          Stream& s;
      };

      template<typename IsClass=fc_keychain::true_type>
      struct if_class{
        template<typename Stream, typename T>
        static inline void pack( Stream& s, const T& v ) { s << v; }
        template<typename Stream, typename T>
        static inline void unpack( Stream& s, T& v ) { s >> v; }
      };

      template<>
      struct if_class<fc_keychain::false_type> {
        template<typename Stream, typename T>
        static inline void pack( Stream& s, const T& v ) {
          s.write( (char*)&v, sizeof(v) );
        }
        template<typename Stream, typename T>
        static inline void unpack( Stream& s, T& v ) {
          s.read( (char*)&v, sizeof(v) );
        }
      };

      template<typename IsEnum=fc_keychain::false_type>
      struct if_enum {
        template<typename Stream, typename T>
        static inline void pack( Stream& s, const T& v ) {
          fc_keychain::reflector<T>::visit( pack_object_visitor<Stream,T>( v, s ) );
        }
        template<typename Stream, typename T>
        static inline void unpack( Stream& s, T& v ) {
          fc_keychain::reflector<T>::visit( unpack_object_visitor<Stream,T>( v, s ) );
        }
      };
      template<>
      struct if_enum<fc_keychain::true_type> {
        template<typename Stream, typename T>
        static inline void pack( Stream& s, const T& v ) {
          fc_keychain::raw::pack(s, (int64_t)v);
        }
        template<typename Stream, typename T>
        static inline void unpack( Stream& s, T& v ) {
          int64_t temp;
          fc_keychain::raw::unpack(s, temp);
          v = (T)temp;
        }
      };

      template<typename IsReflected=fc_keychain::false_type>
      struct if_reflected {
        template<typename Stream, typename T>
        static inline void pack( Stream& s, const T& v ) {
          if_class<typename fc_keychain::is_class<T>::type>::pack(s,v);
        }
        template<typename Stream, typename T>
        static inline void unpack( Stream& s, T& v ) {
          if_class<typename fc_keychain::is_class<T>::type>::unpack(s,v);
        }
      };
      template<>
      struct if_reflected<fc_keychain::true_type> {
        template<typename Stream, typename T>
        static inline void pack( Stream& s, const T& v ) {
          if_enum< typename fc_keychain::reflector<T>::is_enum >::pack(s,v);
        }
        template<typename Stream, typename T>
        static inline void unpack( Stream& s, T& v ) {
          if_enum< typename fc_keychain::reflector<T>::is_enum >::unpack(s,v);
        }
      };

    } // namesapce detail

    template<typename Stream, typename T>
    inline void pack( Stream& s, const std::unordered_set<T>& value ) {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)value.size()) );
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::pack( s, *itr );
        ++itr;
      }
    }
    template<typename Stream, typename T>
    inline void unpack( Stream& s, std::unordered_set<T>& value ) {
      unsigned_int size; fc_keychain::raw::unpack( s, size );
      value.clear();
      FC_KEYCHAIN_ASSERT( size.value*sizeof(T) < MAX_ARRAY_ALLOC_SIZE );
      value.reserve(size.value);
      for( uint32_t i = 0; i < size.value; ++i )
      {
          T tmp;
          fc_keychain::raw::unpack( s, tmp );
          value.insert( std::move(tmp) );
      }
    }


    template<typename Stream, typename K, typename V>
    inline void pack( Stream& s, const std::pair<K,V>& value ) {
       fc_keychain::raw::pack( s, value.first );
       fc_keychain::raw::pack( s, value.second );
    }
    template<typename Stream, typename K, typename V>
    inline void unpack( Stream& s, std::pair<K,V>& value )
    {
       fc_keychain::raw::unpack( s, value.first );
       fc_keychain::raw::unpack( s, value.second );
    }

   template<typename Stream, typename K, typename V>
    inline void pack( Stream& s, const std::unordered_map<K,V>& value ) {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)value.size()) );
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::pack( s, *itr );
        ++itr;
      }
    }
    template<typename Stream, typename K, typename V>
    inline void unpack( Stream& s, std::unordered_map<K,V>& value )
    {
      unsigned_int size; fc_keychain::raw::unpack( s, size );
      value.clear();
      FC_KEYCHAIN_ASSERT( size.value*(sizeof(K)+sizeof(V)) < MAX_ARRAY_ALLOC_SIZE );
      value.reserve(size.value);
      for( uint32_t i = 0; i < size.value; ++i )
      {
          std::pair<K,V> tmp;
          fc_keychain::raw::unpack( s, tmp );
          value.insert( std::move(tmp) );
      }
    }
    template<typename Stream, typename K, typename V>
    inline void pack( Stream& s, const std::map<K,V>& value ) {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)value.size()) );
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::pack( s, *itr );
        ++itr;
      }
    }
    template<typename Stream, typename K, typename V>
    inline void unpack( Stream& s, std::map<K,V>& value )
    {
      unsigned_int size; fc_keychain::raw::unpack( s, size );
      value.clear();
      FC_KEYCHAIN_ASSERT( size.value*(sizeof(K)+sizeof(V)) < MAX_ARRAY_ALLOC_SIZE );
      for( uint32_t i = 0; i < size.value; ++i )
      {
          std::pair<K,V> tmp;
          fc_keychain::raw::unpack( s, tmp );
          value.insert( std::move(tmp) );
      }
    }

    template<typename Stream, typename T>
    inline void pack( Stream& s, const std::deque<T>& value ) {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)value.size()) );
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::pack( s, *itr );
        ++itr;
      }
    }

    template<typename Stream, typename T>
    inline void unpack( Stream& s, std::deque<T>& value ) {
      unsigned_int size; fc_keychain::raw::unpack( s, size );
      FC_KEYCHAIN_ASSERT( size.value*sizeof(T) < MAX_ARRAY_ALLOC_SIZE );
      value.resize(size.value);
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::unpack( s, *itr );
        ++itr;
      }
    }

    template<typename Stream, typename T>
    inline void pack( Stream& s, const std::vector<T>& value ) {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)value.size()) );
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::pack( s, *itr );
        ++itr;
      }
    }

    template<typename Stream, typename T>
    inline void unpack( Stream& s, std::vector<T>& value ) {
      unsigned_int size; fc_keychain::raw::unpack( s, size );
      FC_KEYCHAIN_ASSERT( size.value*sizeof(T) < MAX_ARRAY_ALLOC_SIZE );
      value.resize(size.value);
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::unpack( s, *itr );
        ++itr;
      }
    }

    template<typename Stream, typename T>
    inline void pack( Stream& s, const std::set<T>& value ) {
      fc_keychain::raw::pack( s, unsigned_int((uint32_t)value.size()) );
      auto itr = value.begin();
      auto end = value.end();
      while( itr != end ) {
        fc_keychain::raw::pack( s, *itr );
        ++itr;
      }
    }

    template<typename Stream, typename T>
    inline void unpack( Stream& s, std::set<T>& value ) {
      unsigned_int size; fc_keychain::raw::unpack( s, size );
      for( uint64_t i = 0; i < size.value; ++i )
      {
        T tmp;
        fc_keychain::raw::unpack( s, tmp );
        value.insert( std::move(tmp) );
      }
    }



    template<typename Stream, typename T>
    inline void pack( Stream& s, const T& v ) {
      fc_keychain::raw::detail::if_reflected< typename fc_keychain::reflector<T>::is_defined >::pack(s,v);
    }
    template<typename Stream, typename T>
    inline void unpack( Stream& s, T& v )
    { try {
      fc_keychain::raw::detail::if_reflected< typename fc_keychain::reflector<T>::is_defined >::unpack(s,v);
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "error unpacking ${type}", ("type",fc_keychain::get_typename<T>::name() ) ) }

    template<typename T>
    inline size_t pack_size(  const T& v )
    {
      datastream<size_t> ps;
      fc_keychain::raw::pack(ps,v );
      return ps.tellp();
    }

    template<typename T>
    inline std::vector<char> pack(  const T& v ) {
      datastream<size_t> ps;
      fc_keychain::raw::pack(ps,v );
      std::vector<char> vec(ps.tellp());

      if( vec.size() ) {
        datastream<char*>  ds( vec.data(), size_t(vec.size()) );
        fc_keychain::raw::pack(ds,v);
      }
      return vec;
    }

    template<typename T, typename... Next>
    inline std::vector<char> pack(  const T& v, Next... next ) {
      datastream<size_t> ps;
      fc_keychain::raw::pack(ps,v,next...);
      std::vector<char> vec(ps.tellp());

      if( vec.size() ) {
        datastream<char*>  ds( vec.data(), size_t(vec.size()) );
        fc_keychain::raw::pack(ds,v,next...);
      }
      return vec;
    }


    template<typename T>
    inline T unpack( const std::vector<char>& s )
    { try  {
      T tmp;
      if( s.size() ) {
        datastream<const char*>  ds( s.data(), size_t(s.size()) );
        fc_keychain::raw::unpack(ds,tmp);
      }
      return tmp;
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "error unpacking ${type}", ("type",fc_keychain::get_typename<T>::name() ) ) }

    template<typename T>
    inline void unpack( const std::vector<char>& s, T& tmp )
    { try  {
      if( s.size() ) {
        datastream<const char*>  ds( s.data(), size_t(s.size()) );
        fc_keychain::raw::unpack(ds,tmp);
      }
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "error unpacking ${type}", ("type",fc_keychain::get_typename<T>::name() ) ) }

    template<typename T>
    inline void pack( char* d, uint32_t s, const T& v ) {
      datastream<char*> ds(d,s);
      fc_keychain::raw::pack(ds,v );
    }

    template<typename T>
    inline T unpack( const char* d, uint32_t s )
    { try {
      T v;
      datastream<const char*>  ds( d, s );
      fc_keychain::raw::unpack(ds,v);
      return v;
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "error unpacking ${type}", ("type",fc_keychain::get_typename<T>::name() ) ) }

    template<typename T>
    inline void unpack( const char* d, uint32_t s, T& v )
    { try {
      datastream<const char*>  ds( d, s );
      fc_keychain::raw::unpack(ds,v);
      return v;
    } FC_KEYCHAIN_RETHROW_EXCEPTIONS( warn, "error unpacking ${type}", ("type",fc_keychain::get_typename<T>::name() ) ) }

   template<typename Stream>
   struct pack_static_variant
   {
      Stream& stream;
      pack_static_variant( Stream& s ):stream(s){}

      typedef void result_type;
      template<typename T> void operator()( const T& v )const
      {
         fc_keychain::raw::pack( stream, v );
      }
   };

   template<typename Stream>
   struct unpack_static_variant
   {
      Stream& stream;
      unpack_static_variant( Stream& s ):stream(s){}

      typedef void result_type;
      template<typename T> void operator()( T& v )const
      {
         fc_keychain::raw::unpack( stream, v );
      }
   };


    template<typename Stream, typename... T>
    void pack( Stream& s, const static_variant<T...>& sv )
    {
       fc_keychain::raw::pack( s, unsigned_int(sv.which()) );
       sv.visit( pack_static_variant<Stream>(s) );
    }

    template<typename Stream, typename... T> void unpack( Stream& s, static_variant<T...>& sv )
    {
       unsigned_int w;
       fc_keychain::raw::unpack( s, w );
       sv.set_which(w.value);
       sv.visit( unpack_static_variant<Stream>(s) );
    }

} } // namespace fc_keychain_keychain::raw

