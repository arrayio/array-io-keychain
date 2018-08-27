#pragma once
#include <fc_light/io/raw.hpp>
#include <fc_light/interprocess/file_mapping.hpp>
#include <fc_light/filesystem.hpp>
#include <fc_light/exception/exception.hpp>

namespace fc_light
{
    namespace raw
    {
        template<typename T>
        void unpack_file( const fc_light::path& filename, T& obj )
        {
           try {
               fc_light::file_mapping fmap( filename.generic_string().c_str(), fc_light::read_only);
               fc_light::mapped_region mapr( fmap, fc_light::read_only, 0, fc_light::file_size(filename) );
               auto cs  = (const char*)mapr.get_address();

               fc_light::datastream<const char*> ds( cs, mapr.get_size() );
               fc_light::raw::unpack(ds,obj);
           } FC_LIGHT_RETHROW_EXCEPTIONS( info, "unpacking file ${file}", ("file",filename) );
        }
   }
}
