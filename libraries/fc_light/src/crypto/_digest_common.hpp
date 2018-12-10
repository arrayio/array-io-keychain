#pragma once

/* Common stuff for cryptographic hashes
 */
namespace fc_light { namespace detail {
    void shift_l( const char* in, char* out, std::size_t n, unsigned int i);
    void shift_r( const char* in, char* out, std::size_t n, unsigned int i);
}}
