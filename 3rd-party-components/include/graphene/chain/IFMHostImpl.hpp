/*
 * IFMHostImpl.hpp
 *
 *  Created on: Mar 16, 2018
 *      (c) Array.io 2018
 */
#pragma once
#ifndef INCLUDE_GRAPHENE_CHAIN_IFMHOSTIMPL_HPP_
#define INCLUDE_GRAPHENE_CHAIN_IFMHOSTIMPL_HPP_

#include <fc/string.hpp>
#include <fc/vector.hpp>
#include <fc/exception/exception.hpp>

#include <graphene/chain/protocol/contract.hpp>

#include <graphene/chain/pd_addr.hpp>
#include "../../../../BlockForthMachine/ForthMachine.h"

namespace graphene { namespace chain {

class transaction_evaluation_state;
struct signed_transaction;
struct contract_execute_operation;
struct contract_create_operation;
class database;

struct IFMHostImpl: IFMHost
{
    IFMHostImpl(transaction_evaluation_state& txs, const contract_execute_operation& call_op);
    IFMHostImpl(transaction_evaluation_state& txs, const contract_create_operation& create_op, account_id_type created_contract);
    IFMHostImpl(transaction_evaluation_state& txs, account_id_type contract);

    // { IFMHost
    virtual bool stg_get(const TStgKey& key, TStgVal& val_out) override;
    virtual uint32_t stg_get(const IFMHost::TStgKey& key, uint32_t num_of_keys, std::vector<IFMHost::TStgVal>& data) override;
    virtual bool stg_set(const TStgKey& key, const TStgVal& val) override;
    virtual uint32_t stg_set(const TStgKey& key, const std::vector<TStgVal>& vals);
    virtual uint32_t stg_del(const TStgKey& key, uint32_t num_of_keys);
    virtual TAccountID env_get_caller() override
    { return(get_caller().operator uint64_t()); }
    virtual TAccountID env_get_contract_id() override
    { return(get_contract().operator uint64_t()); }
    virtual TAssetVal env_get_balance(TAccountID account, TAssetID asset) override;
    virtual TPDAddr env_get_caller_address() override;
    virtual TPDAddr env_get_contract_address() override;
    virtual TAssetVal env_get_balance(const TPDAddr& addr) override;
    virtual TAssetID env_get_assetID() override;
    virtual bool env_get_asset_attr(AssetAttr *attr) override;
    virtual void env_set_starting_balance(TAssetVal amount) override;

    virtual void transfer_to(TAccountID account,
                             TAssetID asset, TAssetVal amount,
                             const std::string& memo                    ) override;
    virtual void transfer_in(TAssetID& asset, TAssetVal& amount, std::string& memo) override;
    virtual void transfer_to(const TPDAddr& address, TAssetVal amount, const std::string& memo) override;
    virtual void transfer_in(TPDAddr& address_from, TAssetVal& amount, std::string& memo) override;

    virtual TNumberBlock number_last_block () override;
    virtual TTimeStampBlock time_stamp_last_block () override;
    virtual void block_hash (TNumberBlock& numberBlock, TBlockHash blockHash[5]) override;
    virtual const std::vector<char> env_get_contract_code(const TAccountID contract_id) override;
    virtual const std::vector<char> env_get_contract_code(const std::string& contract_name) override;
    virtual bool env_map_address(const TAddr& addr, TAccountID& account, bool *create) override;

    virtual TPDAddr construct_address(TAssetID assetID, TAccountID accountID) override;
    virtual bool parse_address(const TPDAddr& address, TAssetID& assetID, TAccountID& accountID) override;
    virtual bool check_address(const TPDAddr& address) override;
    virtual bool isAddressApplicable(const TPDAddr& address) override;
    // } IFMHost

static vector<char> DoDumpFromCharArr (const vector<char>& code, IFMHost *ifmhost, uint64_t gas, const vector<char>& args);

private:
    account_id_type     get_caller();
    account_id_type     get_contract() { return(m_contract); }
    const asset&        get_amount();
    const std::string&  get_memo();

    transaction_evaluation_state&   m_txs;
    database&                       m_d;
    const contract_execute_operation  * const m_pcall_op = nullptr;
    const contract_create_operation* const m_pcreate_op = nullptr;
    const account_id_type           m_contract;
    const asset_id_type             m_asset;
};

} } // graphene::chain


#endif /* INCLUDE_GRAPHENE_CHAIN_IFMHOSTIMPL_HPP_ */
