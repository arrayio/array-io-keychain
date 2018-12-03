#include <bitcoin_transaction.hpp>

#include <openssl/sha.h>
#include <iostream>

#include <fc_light/crypto/base58.hpp>

std::string bintohex (std::string const &str, bool asNum);
std::string p2pkh_to_address(std::string const &str);
std::string p2sh_to_address(std::string const &str);


bitcoin_transaction_t::bitcoin_transaction_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_vins = 0;
    m_vouts = 0;
    _read();
}

void bitcoin_transaction_t::_read() {
    m_version = m__io->read_u4le();
    m_num_vins = m__io->read_u1();
    int l_vins = num_vins();
    m_vins = new std::vector<vin_t*>();
    m_vins->reserve(l_vins);
    for (int i = 0; i < l_vins; i++) {
        m_vins->push_back(new vin_t(m__io, this, m__root));
    }
    m_num_vouts = m__io->read_u1();
    int l_vouts = num_vouts();
    m_vouts = new std::vector<vout_t*>();
    m_vouts->reserve(l_vouts);
    for (int i = 0; i < l_vouts; i++) {
        m_vouts->push_back(new vout_t(m__io, this, m__root));
    }
    m_locktime = m__io->read_u4le();
}

bitcoin_transaction_t::~bitcoin_transaction_t() {
    for (std::vector<vin_t*>::iterator it = m_vins->begin(); it != m_vins->end(); ++it) {
        delete *it;
    }
    delete m_vins;
    for (std::vector<vout_t*>::iterator it = m_vouts->begin(); it != m_vouts->end(); ++it) {
        delete *it;
    }
    delete m_vouts;
}

bitcoin_transaction_t::vout_t::vout_t(kaitai::kstream* p__io, bitcoin_transaction_t* p__parent, bitcoin_transaction_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void bitcoin_transaction_t::vout_t::_read() {
    m_amount = m__io->read_u8le();
    m_script_len = m__io->read_u1();
    m_script_pub_key = m__io->read_bytes(script_len());
}

bitcoin_transaction_t::vout_t::~vout_t() {
}

bitcoin_transaction_t::public_key_t::public_key_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void bitcoin_transaction_t::public_key_t::_read() {
    m_type = m__io->read_u1();
    m_x = m__io->read_bytes(32);
    m_y = m__io->read_bytes(32);
}

bitcoin_transaction_t::public_key_t::~public_key_t() {
}

bitcoin_transaction_t::vin_t::vin_t(kaitai::kstream* p__io, bitcoin_transaction_t* p__parent, bitcoin_transaction_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    _read();
}

void bitcoin_transaction_t::vin_t::_read() {
    m_txid = m__io->read_bytes(32);
    m_output_id = m__io->read_u4le();
    m_script_len = m__io->read_u1();
    m_script_sig = m__io->read_bytes(script_len());
    m_end_of_vin = m__io->ensure_fixed_contents(std::string("\xFE\xFF\xFF\xFF", 4));
}

bitcoin_transaction_t::vin_t::~vin_t() {
}

bitcoin_transaction_t::script_signature_t::script_signature_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
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

bitcoin_transaction_t::der_signature_t::der_signature_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, bitcoin_transaction_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
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

bitcoin_transaction_t::der_signature_t::~der_signature_t() {
}


std::string bitcoin_transaction_t::toJSON() const {
	std::string res;
	res = " { ";
	res += "\"version\" : \"" + std::to_string(version()) + "\", ";
	res += "\"num_vins\" : \"" + std::to_string((uint32_t)num_vins()) + "\", ";
	res += "\"vins\" : [ ";
	std::size_t num = vins()->size();
	for (auto it : *vins()) {
		res += it->toJSON();
		if (--num) {
			res += ", ";
		}
	}
	res += " ], ";
	res += "\"num_vouts\" : \"" + std::to_string((uint32_t)num_vouts()) + "\", ";
	num = vouts()->size();
	res += "\"vouts\" : [ ";
	for (auto it : *(vouts())) {
		res += it->toJSON();
		if (--num) {
			res += ", ";
		}
	}
	res += " ], ";
	res += "\"locktime\" : \"" + std::to_string(locktime()) + "\"";
	res += " } ";
	return res;
}

std::string bitcoin_transaction_t::vin_t::toJSON() const {
	std::string res;
	res = " { ";
	res += "\"txid\" : \"" + bintohex(txid(), false) + "\", ";
	res += "\"output_id\" : \"" + std::to_string(output_id()) + "\", ";
	res += "\"script_len\" : \"" + std::to_string((uint32_t)script_len()) + "\", ";
	res += "\"script_sig\" : \"" + bintohex(script_sig(), false) + "\", ";
	res += "\"end_of_vin\" : \"" + bintohex(end_of_vin(), false) + "\"";
	res += " } ";
	return res;
}

std::string bitcoin_transaction_t::vout_t::toJSON() const {
	std::string res;
	res = " { ";
	if (script_pub_key().length() == 25 && script_pub_key().substr(0, 3) == "\x76\xa9\x14" && script_pub_key().substr(23,2) == "\x88\xac") {
//		res += "\"address\" : \"" + bintohex(p2pkh_to_address(script_pub_key().substr(3, 20)), false) + "\", ";
		std::string binaddr = p2pkh_to_address(script_pub_key().substr(3, 20));
		res += "\"address\" : \"" + fc_light::to_base58(binaddr.data(), binaddr.size()) + "\", ";
	} else if (script_pub_key().length() == 23 && script_pub_key().substr(0, 2) == "\xa9\x14" && (uint8_t)script_pub_key()[22] == 0x87) {
//		res += "\"address\" : \"" + bintohex(p2sh_to_address(script_pub_key().substr(2, 20)), false) + "\", ";
		std::string binaddr = p2sh_to_address(script_pub_key().substr(2, 20));
		res += "\"address\" : \"" + fc_light::to_base58(binaddr.data(), binaddr.size()) + "\", ";
//	} else {
//		std::cout << script_pub_key().length() << std::endl;
//		std::cout << std::hex << (uint32_t)(uint8_t)script_pub_key()[0] << (uint32_t)(uint8_t)script_pub_key()[1] << std::endl;
//		std::cout << (uint32_t)(uint8_t)script_pub_key()[22] << std::dec << std::endl;
	}
	res += "\"amount\" : \"" + std::to_string(amount()) + "\", ";
	res += "\"script_len\" : \"" + std::to_string((uint32_t)script_len()) + "\", ";
	res += "\"script_pub_key\" : \"" + bintohex(script_pub_key(), false) + "\"";
	res += " } ";
	return res;
}

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
