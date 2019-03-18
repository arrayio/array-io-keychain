//
// Created by roman on 9/1/19.
//

#include <fc_light/exception/exception.hpp>
#include "private_keymap.hpp"

namespace keychain_app
{

private_key_item::private_key_item(const dev::Secret& secret_, int unlock_duration_, const dev::Secret& chain_code_)
  : secret(secret_)
  , chain_code(chain_code_)
  , unlock_time_point (std::chrono::system_clock::now())
  , unlock_duration(unlock_duration_)
{
}

dev::Public private_key_item::public_key() const
{
  return dev::toPublic(secret);
}

}

