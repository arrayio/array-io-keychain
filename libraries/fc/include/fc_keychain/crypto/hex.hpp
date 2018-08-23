#pragma once
#include <fc_keychain/string.hpp>
#include <fc_keychain/utility.hpp>
#include <vector>

namespace fc_keychain {
    uint8_t from_hex( char c );
    fc_keychain::string to_hex( const char* d, uint32_t s );
    std::string to_hex( const std::vector<char>& data );

    /**
     *  @return the number of bytes decoded
     */
    size_t from_hex( const fc_keychain::string& hex_str, char* out_data, size_t out_data_len );
} 
