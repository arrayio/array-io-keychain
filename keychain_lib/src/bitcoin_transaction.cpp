#include <bitcoin_transaction.hpp>

#include <openssl/sha.h>
#include <iostream>

#include <fc_light/crypto/base58.hpp>

std::string bintohex (std::string const &str, bool asNum);
std::string p2pkh_to_address(std::string const &str);
std::string p2sh_to_address(std::string const &str);


bitcoin_transaction_t::bitcoin_transaction_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root)
	: kaitai::kstruct(p__io)
  , m__parent(p__parent)
  , m__root(this)
{
    _read();
}

bitcoin_transaction_t::bitcoin_transaction_t()
  : kaitai::kstruct(nullptr)
  , version(0)
  , num_vins(0)
  , num_vouts(0)
  , locktime(0)
{}

void bitcoin_transaction_t::_read() {
    version = m__io->read_u4le();
    num_vins = m__io->read_u1();
    int l_vins = num_vins;
    vins.reserve(l_vins);
    for (int i = 0; i < l_vins; i++) {
        vins.push_back(vin_t(m__io, this, m__root));
    }
    num_vouts = m__io->read_u1();
    int l_vouts = num_vouts;
    vouts.reserve(l_vouts);
    for (int i = 0; i < l_vouts; i++) {
        vouts.push_back(vout_t(m__io, this, m__root));
    }
    locktime = m__io->read_u4le();
}

bitcoin_transaction_t::~bitcoin_transaction_t() {}

bitcoin_transaction_t::vout_t::vout_t(kaitai::kstream* p__io, bitcoin_transaction_t* p__parent, bitcoin_transaction_t* p__root)
	: kaitai::kstruct(p__io)
  , m__parent(p__parent)
  , m__root(p__root)
{
    _read();
}

bitcoin_transaction_t::vout_t::vout_t()
  : kaitai::kstruct(nullptr)
  , amount(0)
  , script_len(0)
{}

void bitcoin_transaction_t::vout_t::_read() {
    amount = m__io->read_u8le();
    script_len = m__io->read_u1();
    auto script_pub_key_tmp = m__io->read_bytes(script_len);
    if (script_pub_key_tmp.length() == 25 && script_pub_key_tmp.substr(0, 3) == "\x76\xa9\x14" && script_pub_key_tmp.substr(23,2) == "\x88\xac") {
        std::string binaddr = p2pkh_to_address(script_pub_key_tmp.substr(3, 20));
        address = fc_light::to_base58(binaddr.data(), binaddr.size());
    } else if (script_pub_key_tmp.length() == 23 && script_pub_key_tmp.substr(0, 2) == "\xa9\x14" && (uint8_t)script_pub_key_tmp[22] == 0x87) {
        std::string binaddr = p2sh_to_address(script_pub_key_tmp.substr(2, 20));
        address += fc_light::to_base58(binaddr.data(), binaddr.size());
    }
    script_pub_key = bintohex(script_pub_key_tmp, false);
    std::string binaddr = p2pkh_to_address(script_pub_key.substr(3, 20));
}

bitcoin_transaction_t::vout_t::~vout_t() {}

bitcoin_transaction_t::public_key_t::public_key_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root)
  : kaitai::kstruct(p__io)
  , m__parent(p__parent)
  , m__root(p__root)
{
    _read();
}

void bitcoin_transaction_t::public_key_t::_read() {
    m_type = m__io->read_u1();
    m_x = m__io->read_bytes(32);
    m_y = m__io->read_bytes(32);
}

bitcoin_transaction_t::public_key_t::~public_key_t() {}

bitcoin_transaction_t::vin_t::vin_t(kaitai::kstream* p__io, bitcoin_transaction_t* p__parent, bitcoin_transaction_t* p__root)
  : kaitai::kstruct(p__io)
  , m__parent(p__parent)
  , m__root(p__root)
{
    _read();
}

bitcoin_transaction_t::vin_t::vin_t()
  : kaitai::kstruct(nullptr)
  , output_id(0)
  , script_len(0)
{}

void bitcoin_transaction_t::vin_t::_read() {
    txid = bintohex(m__io->read_bytes(32), false);
    output_id = m__io->read_u4le();
    script_len = m__io->read_u1();
    script_sig = bintohex(m__io->read_bytes(script_len), false);
    end_of_vin = bintohex(m__io->ensure_fixed_contents(std::string("\xFE\xFF\xFF\xFF", 4)), false);
}

bitcoin_transaction_t::vin_t::~vin_t() {
}

bitcoin_transaction_t::script_signature_t::script_signature_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root)
  : kaitai::kstruct(p__io)
  , m__parent(p__parent)
  , m__root(p__root)
{
    m_der_sig = 0;
    m_pubkey = 0;
    _read();
}

void bitcoin_transaction_t::script_signature_t::_read() {
    m_sig_stack_len = m__io->read_u1();
    m_der_sig = new der_signature_t(m__io, this, m__root);
    m_sig_type = static_cast<bitcoin_transaction_t::sighash_type_t>(m__io->read_u1());
    m_pubkey_stack_len = m__io->read_u1();
    m_pubkey = new public_key_t(m__io, this, m__root);
}

bitcoin_transaction_t::script_signature_t::~script_signature_t() {
    delete m_der_sig;
    delete m_pubkey;
}

bitcoin_transaction_t::der_signature_t::der_signature_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root)
  : kaitai::kstruct(p__io)
  , m__parent(p__parent)
  , m__root(p__root)
{
    _read();
}

void bitcoin_transaction_t::der_signature_t::_read() {
    m_sequence = m__io->ensure_fixed_contents(std::string("\x30", 1));
    m_sig_len = m__io->read_u1();
    m_sep_1 = m__io->ensure_fixed_contents(std::string("\x02", 1));
    m_sig_r_len = m__io->read_u1();
    m_sig_r = m__io->read_bytes(sig_r_len());
    m_sep_2 = m__io->ensure_fixed_contents(std::string("\x02", 1));
    m_sig_s_len = m__io->read_u1();
    m_sig_s = m__io->read_bytes(sig_s_len());
}

bitcoin_transaction_t::der_signature_t::~der_signature_t() {}

std::string bintohex (std::string const &str, bool asNum) {
	std::string res;
	char el[3] = {0, };
	if (asNum) {
		for (int64_t i = str.length() - 1; i >= 0; i--) {
			uint8_t temp = ((str[i]>>4) & 0x0f);
			el[0] = temp > 9 ? (temp -10 + 'a') : (temp + '0');
			temp = (str[i] & 0x0f);
			el[1] = temp > 9 ? (temp -10 + 'a') : (temp + '0');
			res += el;
		}
	} else {
		for (int64_t i = 0; i < str.length(); i++) {
			uint8_t temp = ((str[i]>>4) & 0x0f);
			el[0] = temp > 9 ? (temp -10 + 'a') : (temp + '0');
			temp = (str[i] & 0x0f);
			el[1] = temp > 9 ? (temp -10 + 'a') : (temp + '0');
			res += el;
		}
	}
	return res;
}

std::string p2pkh_to_address(std::string const &str) {
	std::string _str = std::string("\x00", 1) + str;
	std::string checksum;
	checksum.resize(32);
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, _str.data(), _str.length());
	SHA256_Final((unsigned char*)(void*)(&(checksum[0])), &ctx);
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &checksum[0], 32);
	SHA256_Final((unsigned char*)(void*)(&(checksum[0])), &ctx);
	return (_str + std::string(checksum.data(), 4));
}

std::string p2sh_to_address(std::string const &str) {
	std::string _str = std::string("\x05", 1) + str;
	std::string checksum;
	checksum.resize(32);
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, _str.data(), _str.length());
	SHA256_Final((unsigned char*)(void*)(&(checksum[0])), &ctx);
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, &checksum[0], 32);
	SHA256_Final((unsigned char*)(void*)(&(checksum[0])), &ctx);
	return (_str + std::string(checksum.data(), 4));
}

