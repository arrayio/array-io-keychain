/*
 * nvs.hpp
 *
 *  Created on: Jan 29, 2018
 *  (c) 2018 Annihilat.io
 */

#ifndef INCLUDE_GRAPHENE_CHAIN_PROTOCOL_NVS_HPP_
#define INCLUDE_GRAPHENE_CHAIN_PROTOCOL_NVS_HPP_

#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/ext.hpp>
#include <graphene/chain/protocol/types.hpp>

namespace graphene { namespace chain {


   /**
    *  @ingroup operations
    */
   struct nvs_create_operation : public base_operation
   {
      struct fee_parameters_type {
           static constexpr uint32_t PRICE_PER_KBYTE_STG = 1000 * GRAPHENE_BLOCKCHAIN_PRECISION;
           static constexpr uint32_t PRICE_PER_DAY_SQR = 15 * GRAPHENE_BLOCKCHAIN_PRECISION;
           static constexpr uint32_t PRICE_PER_KBYTE_BLK = 15 * GRAPHENE_BLOCKCHAIN_PRECISION;

           uint64_t fee                 = 50 * GRAPHENE_BLOCKCHAIN_PRECISION;
           uint32_t price_per_kbyte_stg = PRICE_PER_KBYTE_STG;
           uint32_t price_per_day_sqr   = PRICE_PER_DAY_SQR;
           uint32_t price_per_kbyte_blk = PRICE_PER_KBYTE_BLK;
      };

      static const uint32_t MAX_NAME_LEN = 512;
      static const uint32_t MAX_VAL_LEN  = 64 * 1024;
      static const uint32_t MAX_DAYS = 9999;
      static const uint32_t MAX_OWNERS_SIZE = 1024;

      asset                     fee;

      account_id_type           caller;

      vector<char>              name;
      uint32_t                  len;
      uint32_t                  days;
      vector<account_id_type>   owners;

      optional<vector<char> >   value;

      extensions_type           extensions;

      account_id_type fee_payer()const { return caller; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& )const;
   };

   /**
    *  @ingroup operations
    */
   struct nvs_update_operation : public base_operation
   {
      struct fee_parameters_type {
           uint64_t fee                 = 30 * GRAPHENE_BLOCKCHAIN_PRECISION;
           uint32_t price_per_kbyte_stg = nvs_create_operation::fee_parameters_type::PRICE_PER_KBYTE_STG;
           uint32_t price_per_day_sqr   = nvs_create_operation::fee_parameters_type::PRICE_PER_DAY_SQR;
           uint32_t price_per_kbyte_blk = nvs_create_operation::fee_parameters_type::PRICE_PER_KBYTE_BLK;
      };

      asset                     fee;

      account_id_type           caller;

      vector<char>              name;
      uint32_t                  len;
      uint32_t                  days;
      vector<account_id_type>   owners;

      optional<vector<char> >   value;

      extensions_type           extensions;

      account_id_type fee_payer()const { return caller; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& )const;
   };

   /**
    *  @ingroup operations
    */
   struct nvs_set_operation : public base_operation
   {
      struct fee_parameters_type {
           uint64_t fee                 = 50 * GRAPHENE_BLOCKCHAIN_PRECISION;
           uint32_t price_per_kbyte_blk = nvs_create_operation::fee_parameters_type::PRICE_PER_KBYTE_BLK;
      };

      asset                     fee;

      account_id_type           caller;

      vector<char>              name;
      vector<char>              value;

      extensions_type           extensions;

      account_id_type fee_payer()const { return caller; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& )const;
   };

   /**
    *  @ingroup operations
    */
   struct nvs_delete_operation : public base_operation
   {
      struct fee_parameters_type {
           uint64_t fee       = 50 * GRAPHENE_BLOCKCHAIN_PRECISION;
           uint32_t price_per_kbyte_blk = nvs_create_operation::fee_parameters_type::PRICE_PER_KBYTE_BLK;
      };

      asset                     fee;

      account_id_type           caller;

      vector<char>              name;

      extensions_type           extensions;

      account_id_type fee_payer()const { return caller; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& )const;
   };


} } // graphene::chain

FC_REFLECT( graphene::chain::nvs_create_operation::fee_parameters_type, (fee)(price_per_kbyte_stg)(price_per_day_sqr)(price_per_kbyte_blk) )

FC_REFLECT( graphene::chain::nvs_create_operation,
            (fee)
            (caller)
            (name)(len)(days)(owners)(value)
            (extensions)
          )

FC_REFLECT( graphene::chain::nvs_update_operation::fee_parameters_type, (fee)(price_per_kbyte_stg)(price_per_day_sqr)(price_per_kbyte_blk) )

FC_REFLECT( graphene::chain::nvs_update_operation,
            (fee)
            (caller)
            (name)(len)(days)(owners)(value)
            (extensions)
          )

FC_REFLECT( graphene::chain::nvs_set_operation::fee_parameters_type, (fee)(price_per_kbyte_blk) )

FC_REFLECT( graphene::chain::nvs_set_operation,
            (fee)
            (caller)
            (name)(value)
            (extensions)
          )

FC_REFLECT( graphene::chain::nvs_delete_operation::fee_parameters_type, (fee) )

FC_REFLECT( graphene::chain::nvs_delete_operation,
            (fee)
            (caller)
            (name)
            (extensions)
          )

#endif /* INCLUDE_GRAPHENE_CHAIN_PROTOCOL_CONTRACT_HPP_ */
