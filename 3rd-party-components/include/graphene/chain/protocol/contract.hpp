/*
 * contract.hpp
 *
 *  Created on: Aug 14, 2017
 *  (c) 2017 Annihilat.io
 */

#ifndef INCLUDE_GRAPHENE_CHAIN_PROTOCOL_CONTRACT_HPP_
#define INCLUDE_GRAPHENE_CHAIN_PROTOCOL_CONTRACT_HPP_

#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/ext.hpp>
#include <graphene/chain/protocol/types.hpp>

namespace graphene { namespace chain {


   struct gas_price_type
   {
       share_type       asset_amount;
       safe<uint32_t>   gas_amount;

       gas_price_type(uint32_t gas, uint64_t asset)
           : asset_amount(asset), gas_amount(gas)
       {}
       gas_price_type()
           : asset_amount(0), gas_amount(0)
       {}

       share_type gas_cost(uint32_t gas);
   };

   /**
    *  @ingroup operations
    */
   struct contract_create_operation : public base_operation
   {
      struct fee_parameters_type
      {
         uint64_t basic_fee      = 1000*GRAPHENE_BLOCKCHAIN_PRECISION; ///< the cost to register the cheapest non-free account
         uint64_t premium_fee    = 10000*GRAPHENE_BLOCKCHAIN_PRECISION; ///< the cost to register the cheapest non-free account
         uint64_t gas_price_quote = 1 * GRAPHENE_BLOCKCHAIN_PRECISION;
         uint32_t gas_price_base  = 100;  // you get gas_price_base gas for gas_price_quote Core units
         uint32_t price_per_kbyte = GRAPHENE_BLOCKCHAIN_PRECISION;

         gas_price_type gas_price() const { return(gas_price_type(gas_price_base, gas_price_quote)); }
      };

      asset           fee;
      gas_price_type  gas_price; // asset is in fee.asset_id
      uint32_t        gas_limit = 0;

      /// This account pays the fee. Must be a lifetime member.
      account_id_type registrar;

      /// This account receives a portion of the fee split between registrar and referrer. Must be a member.
      account_id_type referrer;
      /// Of the fee split between registrar and referrer, this percentage goes to the referrer. The rest goes to the
      /// registrar.
      uint16_t        referrer_percent = 0;

      string          name;

      vector<char>    code;
      vector<char>    args;

      string          asset_symbol;
       /// See @ref contract_options_flags
      uint16_t        flags = 0;

      extensions_type extensions;

      account_id_type fee_payer()const { return registrar; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& )const;
      share_type      calculate_gas_fee(const fee_parameters_type&, uint32_t gas_amount )const;

      void get_required_active_authorities( flat_set<account_id_type>& a )const
      {
         // registrar should be required anyway as it is the fee_payer(), but we insert it here just to be sure
         a.insert( registrar );
      }
   };

   /**
    *  @ingroup operations
    */
   struct contract_execute_operation : public base_operation
   {
      struct fee_parameters_type {
           uint64_t fee       = 50 * GRAPHENE_BLOCKCHAIN_PRECISION;
           uint64_t gas_price_quote = 1 * GRAPHENE_BLOCKCHAIN_PRECISION;
           uint32_t gas_price_base  = 100;  // you get gas_price_base gas for gas_price_quote Core units
           uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.

           gas_price_type gas_price() const { return(gas_price_type(gas_price_base, gas_price_quote)); }
      };

      asset           fee;
      gas_price_type  gas_price; // asset is in fee.asset_id
      uint32_t        gas_limit = 0;

      account_id_type caller;

      account_id_type contract;
      vector<char>    args;
      asset           amount;

      string          memo;
      extensions_type extensions;

      account_id_type fee_payer()const { return caller; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& )const;
      share_type      calculate_gas_fee(const fee_parameters_type&, uint32_t gas_amount )const;
   };


} } // graphene::chain

FC_REFLECT( graphene::chain::gas_price_type, (asset_amount)(gas_amount) )

FC_REFLECT( graphene::chain::contract_create_operation::fee_parameters_type, (basic_fee)(premium_fee)(price_per_kbyte) )

FC_REFLECT( graphene::chain::contract_create_operation,
            (fee)(gas_price)(gas_limit)(registrar)
            (referrer)(referrer_percent)
            (name)(code)(args)
            (asset_symbol)(flags)
            (extensions)
          )

FC_REFLECT( graphene::chain::contract_execute_operation::fee_parameters_type, (fee)(gas_price_quote)(gas_price_base)(price_per_kbyte) )

FC_REFLECT( graphene::chain::contract_execute_operation,
            (fee)(gas_price)(gas_limit)
            (caller)
            (contract)(args)(amount)
            (memo)(extensions)
          )

#endif /* INCLUDE_GRAPHENE_CHAIN_PROTOCOL_CONTRACT_HPP_ */
