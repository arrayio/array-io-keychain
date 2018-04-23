/*
 * pd_addr.hpp
 *
 *  Created on: Feb 7, 2018
 *      (c) 2018 Array.io
 */

#pragma once

#ifndef INCLUDE_GRAPHENE_CHAIN_PROTOCOL_PD_ADDR_HPP_
#define INCLUDE_GRAPHENE_CHAIN_PROTOCOL_PD_ADDR_HPP_

#include <vector>

#include <graphene/chain/protocol/types.hpp>

#include "../../BlockForthMachine/ForthMachine.h"

namespace graphene { namespace chain {

using TPDAddr = IFMHost::TPDAddr;

static const char PDA_PREFIX[4] = { "PDA" };
static const unsigned char PDA_TESTNET_ID = 0xFF;

void    pda_parse(const TPDAddr& addr, unsigned char& net, account_id_type& account, asset_id_type& asset);
TPDAddr pda_build(unsigned char net, account_id_type account, asset_id_type asset);

} } // graphene::chain



#endif /* INCLUDE_GRAPHENE_CHAIN_PROTOCOL_PD_ADDR_HPP_ */
