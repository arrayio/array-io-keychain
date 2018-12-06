![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/ltl1%20(2).png) 
# KeyChain

## Overview

**KeyChain** is a standalone app for signing transactions and generating key pairs. It stores private keys in an isolated environment where no logger, debugger or spyware can intercept them because of the [three-layer security](https://github.com/arrayio/array-io-keychain/wiki#three-security-layers-of-keychain) protecting each action of the system.
**KeyChain** supports transactions from and to various blockchains, including Ethereum and Ethereum classic, Litecoin, Bitcoin, and Bitshares. 

## Installation

Download and install KeyChain for [macOS](https://github.com/arrayio/array-io-keychain/releases/download/0.8/KeyChain.Installer.v0.8.zip). Windows and Linux installers are coming soon.

After installation, connect to the demo-page: `http://localhost:16384/` to test the KeyChain commands.

You can find comprehensive [installation guides](https://github.com/arrayio/array-io-keychain/wiki/Installation-guides) in our wiki. 

## Getting started

1. Create a function that will sign the transaction

```javascript
const signHexCommand = (hexraw) => {
  return {
    "command": "sign_hex",
    "params": {
      "transaction": hexraw,
      "blockchain_type": "ethereum",
      "keyname": keyname
    }
  }
}
```

2. Pass hex of the transaxtion to the function

```javascript
ws.onopen = async () => {
  console.log('ws open');
  const rawHex = await buildTxSinature(
    null, // signature
    fromAdd,
    toAdd,
    valueTx
  )
  console.log('sign tx:', rawHex);
  sendCommand(signHexCommand(rawHex));
}
```

## How to use 

For extensive documentation on KeyChain, refer to the [Wiki](https://github.com/arrayio/array-io-keychain/wiki).

There you will find:

- [Home](https://github.com/arrayio/array-io-keychain/wiki): how to navigate in our Wiki 
- [How to sign an Ethereum transaction](https://github.com/arrayio/array-io-keychain/wiki/How-to-sign-Ethereum-transaction-via-KeyChain): a simple and precise tutorial
- [Installation guides](https://github.com/arrayio/array-io-keychain/wiki/Installation-guides) for MacOs, Windows, Linux
- [KeyChain Protocol](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-Protocol): full comprehensive descriptions of the KeyChain commands
- [KeyChain sample commands](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-sample-commands): shortcut to using the commands
- [Pipe API](https://github.com/arrayio/array-io-keychain/wiki/Pipe-API): integrating KeyChain through pipe.
- [Security](https://github.com/arrayio/array-io-keychain/wiki/Security): why KeyChain is highly secure
- [Troubleshooting](https://github.com/arrayio/array-io-keychain/wiki/Troubleshooting): error handling, log files, debugging.
- [Useful reference](https://github.com/arrayio/array-io-keychain/wiki/Useful-reference): external links.
- [WebSocket API](https://github.com/arrayio/array-io-keychain/wiki/WebSocket-API): integrating KeyChain through WebSocket.


