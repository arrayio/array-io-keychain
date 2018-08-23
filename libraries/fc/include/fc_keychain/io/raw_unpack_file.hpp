#pragma once
#include <fc_keychain/io/raw.hpp>
#include <fc_keychain/interprocess/file_mapping.hpp>
#include <fc_keychain/filesystem.hpp>
#include <fc_keychain/exception/exception.hpp>

namespace fc_keychain
{
    namespace raw
    {
        template<typename T>
        void unpack_file( const fc_keychain::path& filename, T& obj )
        {
           try {
               fc_keychain::file_mapping fmap( filename.generic_string().c_str(), fc_keychain::read_only);
               fc_keychain::mapped_region mapr( fmap, fc_keychain::read_only, 0, fc_keychain::file_size(filename) );
               auto cs  = (const char*)mapr.get_address();

               fc_keychain::datastream<const char*> ds( cs, mapr.get_size() );
               fc_keychain::raw::unpack(ds,obj);
           } FC_KEYCHAIN_RETHROW_EXCEPTIONS( info, "unpacking file ${file}", ("file",filename) );
        }
   }
}
