//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_SIGN_DEFINE_HPP
#define KEYCHAINAPP_SIGN_DEFINE_HPP

namespace keychain_app
{

using signature_t = fc::ecc::compact_signature;
using unit_list = std::vector<fc::sha256>;
using sign_functor_t = std::function<signature_t(const fc::ecc::private_key& key, const unit_list& list)>;

}

#endif //KEYCHAINAPP_SIGN_DEFINE_HPP
