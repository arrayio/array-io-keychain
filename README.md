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

**Requirements**
 - gcc minimum version 6
 - cmake minimum version 3.0.2
 - BOOST lib minimum version 1.67
 - openssl-1.1.0g
 
 For building all libraries and project to use gcc are required.
 
**Dependencies:**
 You must have libraries:
 - libfc.a
 - libgraphene_utilities.a
 - libsecp256k1.a
 
 These files are built in the project https://github.com/arrayio/array-io-core.git.
 Put these libraries into directory of cloned source *../3rd-party-components/lib*

**Compiling and Installing from Source**
```
 $ git clone https://github.com/arrayio/array-io-keychain.git
 $ cd array-io-keychain/
 $ mkdir build
 $ cd build 
 $ cmake -DBOOST_ROOT="/opt/boostlib" -DOPENSSL_ROOT_DIR="/opt/openssllib" -G "Unix Makefiles" ../CMakeLists.txt
 $ make
```

**Installing from binary**
 - go to page https://github.com/arrayio/array-io-keychain/releases 
 - download corresponding release


**How to use**

 See corresponding wiki
 https://github.com/arrayio/array-io-keychain/wiki/

## OSX

