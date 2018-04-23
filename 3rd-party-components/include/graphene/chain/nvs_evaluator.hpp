/*
 * nvs_evaluator.hpp
 *
 *  Created on: Jan 29, 2018
 *  (c) 2018 Annihilat.io
 */

#ifndef INCLUDE_GRAPHENE_CHAIN_NVS_EVALUATOR_HPP_
#define INCLUDE_GRAPHENE_CHAIN_NVS_EVALUATOR_HPP_

#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/account_object.hpp>

namespace graphene { namespace chain {

class nvs_create_evaluator : public evaluator<nvs_create_evaluator>
{
public:
   typedef nvs_create_operation operation_type;

   void_result do_evaluate(const nvs_create_operation& o);
   void_result do_apply   (const nvs_create_operation& o) ;
};

class nvs_update_evaluator : public evaluator<nvs_update_evaluator>
{
public:
   typedef nvs_update_operation operation_type;

   void_result do_evaluate(const nvs_update_operation& o);
   void_result do_apply   (const nvs_update_operation& o) ;
};

class nvs_set_evaluator : public evaluator<nvs_set_evaluator>
{
public:
   typedef nvs_set_operation operation_type;

   void_result do_evaluate(const nvs_set_operation& o);
   void_result do_apply   (const nvs_set_operation& o) ;
};

class nvs_delete_evaluator : public evaluator<nvs_delete_evaluator>
{
public:
   typedef nvs_delete_operation operation_type;

   void_result do_evaluate(const nvs_delete_operation& o);
   void_result do_apply   (const nvs_delete_operation& o) ;
};


} } // graphene::chain


#endif /* INCLUDE_GRAPHENE_CHAIN_CONTRACT_EVALUATOR_HPP_ */
