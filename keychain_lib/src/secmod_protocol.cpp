//
// Created by roman on 4/20/18.
//

#include "sec_mod_protocol.hpp"

namespace keychain_app
{

namespace secmod_commands
{

virtual std::string secmod_command_base::to_pretty_string() const
{
  return fc_light::json::to_pretty_string(fc_light::variant(this));
}

}

}


