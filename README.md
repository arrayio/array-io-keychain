[![Logo](https://raw.githubusercontent.com/arrayio/array-io-keychain/master/img/keychain-logo.png)]()

# array-io-keychain

[![Join the chat at https://gitter.im/arrayio/array-io-keychain](https://badges.gitter.im/arrayio/array-io-keychain.svg)](https://gitter.im/arrayio/array-io-keychain?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Public Array.io keychain repository. Keychain is an app to create, manage private keys, sign transactions, and encrypt data with the keys.

Keychain may manage private keys in different formats. Most popular ECDSA (bitcoin, ethereum etc) and RSA supported. Everything a modern OpenSSL release supports.

Keychain is not to be confused with the Wallet built-in Dapp, the keychain app, is a standalone commandline/ui interface to interact with private keys.

Array.io keychain may be used by a node, a frontend or dapp's nodejs server to sign transactions. It may be used by a web application too, keeping keys in a separate environment from browser.

# Installation

## Windows

## Linux
Requirements
 - CMake (minimum required VERSION 3.0.2)
 - BOOST lib minimum required 1.67
 - openssl-1.1.0g

Dependences:
 You must have libraries:
 - libfc.a
 - libgraphene_utilities.a
 - libsecp256k1.a
 These files are built in the project https://github.com/arrayio/array-io-core.git


Installation
 - run terminal
 - git clone https://github.com/arrayio/array-io-keychain.git
 - put lib files into ../array-io-keychain/3rd-party-components/lib
 - cd array-io-keychain/
 - $ cmake -DCMAKE_BUILD_TYPE=Debug  -DOPENSSL_ROOT_DIR="/usr/local/lib" -G  "Unix Makefiles" ../


## OSX
