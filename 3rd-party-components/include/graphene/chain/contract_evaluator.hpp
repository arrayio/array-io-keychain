/*
 * contract_evaluator.hpp
 *
 *  Created on: Aug 14, 2017
 *  (c) 2017 Annihilat.io
 */

#ifndef INCLUDE_GRAPHENE_CHAIN_CONTRACT_EVALUATOR_HPP_
#define INCLUDE_GRAPHENE_CHAIN_CONTRACT_EVALUATOR_HPP_

#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/account_object.hpp>

namespace graphene { namespace chain {

class contract_create_evaluator : public evaluator<contract_create_evaluator>
{
public:
   typedef contract_create_operation operation_type;

   void_result do_evaluate( const contract_create_operation& o );
   operation_result do_apply( const contract_create_operation& o );

   void prepare_gas(const contract_create_operation& o);
};

class contract_call_evaluator : public evaluator<contract_call_evaluator>
{
public:
   typedef contract_execute_operation operation_type;

   void_result do_evaluate(const contract_execute_operation& o);
   void_result do_apply   (const contract_execute_operation& o) ;

static void init();

   void prepare_gas(const contract_execute_operation& o);

   const account_object* ctr;
};

} } // graphene::chain


#endif /* INCLUDE_GRAPHENE_CHAIN_CONTRACT_EVALUATOR_HPP_ */
