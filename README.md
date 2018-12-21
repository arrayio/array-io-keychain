![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/ltl1%20(2).png) 
# KeyChain

## Overview

**KeyChain** is a standalone app for signing transactions and generating key pairs. It stores private keys in an isolated environment where no logger, debugger or spyware can intercept them because of the [three-layer security](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-security#three-security-layers-of-keychain) protecting each action of the system.
**KeyChain** supports transactions from and to various blockchains, including Ethereum and Ethereum classic, Litecoin, Bitcoin, Bitcoin Cash, and Bitshares. 

## Installation

Download and install KeyChain for [macOS](https://github.com/arrayio/array-io-keychain/releases/download/0.10/KeyChain.Installer.v0.10.zip). Windows and Linux installers are coming soon.

*Try out KeyChain on the [demo page](https://arrayio.github.io/array-io-keychain/demo/).*

After installation, connect to the demo-page: http://localhost:16384/ to check if the installation was successful and to test the KeyChain commands. In case everything went well, you will see the following page and you will be able to see responses to the commands in the "Response" box when you click on them.

![screenshot from 2018-12-10 15-57-27](https://user-images.githubusercontent.com/34011337/49734247-be211a80-fc94-11e8-8d85-c70b738ecae3.png)

If you are having trouble connecting to the page, [contact us](#contact) and we will do our best to help you.

You can find comprehensive installation guides for [macOS](https://github.com/arrayio/array-io-keychain/wiki/Installation-guide-for-macOS), [Windows](https://github.com/arrayio/array-io-keychain/wiki/Installation-guide-for-Windows), and [Linux](https://github.com/arrayio/array-io-keychain/wiki/Installation-guide-for-Linux) in our Wiki. 

## Getting started

1. Require libraries setup 

```javascript
const unsign = require('@warren-bank/ethereumjs-tx-unsign')
const ethUtil = require('ethereumjs-util');
const WebSocket = require('ws');
```
```javascript
const ws = new WebSocket('ws://localhost:16384/');
const keyname = 'test1@6de493f01bf590c0';
```

2. Get public key

```javascript
let fromAdd;
const send = (command) => {
  ws.send(JSON.stringify(command));
}

const getPublicKey = (keyname) => {
  return { 
    command: "public_key",
    params: {
      keyname: keyname
    }
  }
}

ws.onopen = async () => {
  send(getPublicKey(keyname));
}

ws.on('message', async (response) => {
  const data = JSON.parse(response);
  console.log('Public key: ', `0x${data.result}`)
  fromAdd = `0x${ethUtil.publicToAddress(`0x${data.result}`).toString('hex')}`;
  console.log(fromAdd);
});
```

3. Unsign Ethereum transaction hex and pass the result to KeyChain

```javascript
const ethHex = 'e315843b9aca0082520894e8899ba12578d60e4d0683a596edacbc85ec18cc6480038080';

const signHex = (keyname, hexraw) => {
  return {
    "command": "sign_hex",
    "params": {
      "transaction": hexraw,
      "blockchain_type": "ethereum",
      "keyname": keyname
    }
  }
}

ws.onopen = async () => {
  send(signHex(keyname, ethHex));
}

ws.on('message', async (response) => {
  const data = JSON.parse(response);
  console.log('Signature: ', data.result);
});


const resHex = 'f86315843b9aca0082520894e8899ba12578d60e4d0683a596edacbc85ec18cc64802aa0c3b68e20527f8f304801986720de1aeb9ab126c55570942420361cedeec1199ca03002d853eb2089e4e63848fcdb7af0fbc46e8f5c856ef0eb849a2270fd621bdb';
let { txData, signature } = unsign(resHex);
console.log('txData:',    txData,    "\n")
console.log('signature:', signature, "\n")
```

## How to use 

For extensive documentation on KeyChain, refer to the [Wiki](https://github.com/arrayio/array-io-keychain/wiki).

There you will find:

- [Home](https://github.com/arrayio/array-io-keychain/wiki): how to navigate in our Wiki. 
- [How to sign an Ethereum transaction](https://github.com/arrayio/array-io-keychain/wiki/How-to-sign-Ethereum-transaction-via-KeyChain): a simple and precise tutorial.
- Installation guides for [macOS](https://github.com/arrayio/array-io-keychain/wiki/Installation-guide-for-macOS), [Windows](https://github.com/arrayio/array-io-keychain/wiki/Installation-guide-for-Windows), [Linux](https://github.com/arrayio/array-io-keychain/wiki/Installation-guide-for-Linux).
- [KeyChain Protocol](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-Protocol): full comprehensive descriptions of the KeyChain commands.
- [KeyChain sample commands](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-sample-commands): shortcut to using the commands.
- [Pipe API](https://github.com/arrayio/array-io-keychain/wiki/Pipe-API): integrating KeyChain through pipe.
- [Security](https://github.com/arrayio/array-io-keychain/wiki/Security): why KeyChain is highly secure.
- [Troubleshooting](https://github.com/arrayio/array-io-keychain/wiki/Troubleshooting): error handling, log files, debugging.
- [Useful reference](https://github.com/arrayio/array-io-keychain/wiki/Useful-reference): external links.
- [WebSocket API](https://github.com/arrayio/array-io-keychain/wiki/WebSocket-API): integrating KeyChain through WebSocket.

## Contact

If you need help interacting with KeyChain, please do not hesitate to contact us:

- [Twitter](https://twitter.com/ProjectArray)

- [Telegram](https://t.me/arrayio)

- [Stackoverflow](https://stackoverflow.com/users/10429540/array-io)

- Or you can write us an email to support@array.io. 

If you want to report a security issue, include the word "security" in the subject field.

We take security issues very seriously and we'll be looking forward to hearing from you. Still, we hope you enjoy using KeyChain and the integration goes smooth! 

## License

This project is licensed under the terms of the MIT license.
