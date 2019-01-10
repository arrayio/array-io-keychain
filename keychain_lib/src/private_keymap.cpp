//
// Created by roman on 9/1/19.
//

#include <fc_light/exception/exception.hpp>
#include "private_keymap.hpp"

namespace keychain_app
{

private_key_item::private_key_item(const std::string& keyname_, const dev::Secret& secret_, int unlock_duration_)
  : keyname(keyname_)
  , secret(secret_)
  , unlock_time_point (std::chrono::system_clock::now())
  , unlock_duration(unlock_duration_)
{
}

}

