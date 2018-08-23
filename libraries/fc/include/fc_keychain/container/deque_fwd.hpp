#pragma once

#include <deque>

namespace fc_keychain {

   namespace raw {
       template<typename Stream, typename T>
       void pack( Stream& s, const std::deque<T>& value );
       template<typename Stream, typename T>
       void unpack( Stream& s, std::deque<T>& value );
   }
} // namespace fc_keychain
