# KeyChain
![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/LOGOkeyHockey.png "KeyChain")

## Overview

**KeyChain** is a standalone app for signing transactions and generating key pairs. It stores private keys in an isolated environment where no logger, debugger or spyware can intercept them because of the three-layer security protecting each action of the system.
**KeyChain** supports transactions from and to various blockchains, including Ethereum and Ethereum classic, Litecoin, Bitcoin, and Bitshares. 

You can integrate **KeyChain** as a service through `pipe` or `websocket` methods. For quick step-by-step guides see [Pipe integration guide](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-API) and [WebSocket integration guide](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-API). For simple installation tutorials for Windows, MacOs, and Linux go to [Installation guides](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-API). 

Read full [KeyChain API](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-API) with comprehensive descriptions of JSON requests broken down into simple commands and parameters. 

You can find the detailed technical specification of the KeyChain system below if you refer to [How it works](#How it works) section. 

## Installation

Download KeyChain and run the graphic installer if you are using Windows or MacOs. If you are using Linux, run the commands that you find in the Linux installation guide after downloading the KeyChain package. 

#### Linux

For Linux installation, follow the steps:

1. There is an official KeyChain repository that is protected with the key. To download and install KeyChain, first you need to get the key
```
$ wget -q -O - https://array.io/keychain/keychain_signing_key.pub | Sudo apt-key add -
``` 
2. Install KeyChain from our repository and install it
```
$ sudo sh -c 'echo "deb http://array.io/keychain/deb/ stable main" >> /etc/apt/sources.list.d/array.list'
$ sudo apt-get update
$ sudo apt-get install keychain
``` 
#### MacOs
Follow the steps of the graphic installer. All you will need to do is choose a folder and wait until the KeyChain is successfully installed.

![Choose a folder and press "next"](https://github.com/arrayio/array-io-keychain/blob/master/img/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202018-11-23%20%D0%B2%2014.24.25.png)

![Press "Install" and wait till it is complete](https://github.com/arrayio/array-io-keychain/blob/master/img/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202018-11-23%20%D0%B2%2015.02.15.png)

#### Windows
Run the installer and follow the steps.

After successful installation you will be returned to the web page or app you started from. The WebSocket server will be ready to work.

## <a name="How it works"></a>How it works

![Three-layer security](https://github.com/arrayio/array-io-keychain/blob/master/img/Diagram%20Keychain%20fin%201.png)

Apps or websites send requests to the KeyChain through two types of communication - standard I/O streams (mostly called pipes), and the WebSocket. 
The architecture of the KeyChain software consists of the three independent layers:

1. **API layer** which integrates with your app, website or any external application. It is language-neutral. The protocol for the terminal application operates with the JSON format in synchronous request/response way. The main function of the **API layer** is to transmit and parse commands for given API. 
Each request carries information about commands, the type of key user wants to use to sign transactions and other relevant parameters which you can find in the [Protocol](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-API). 

2. **Security layer** receives the commands from the API layer and acts as an OS-specific  protection mechanism for the **interface window** (third layer). It serves as a shield from potential attacks at sensitive data and information. **Security layer** is tailored for the Mac OS, Linux, and Windows OS and operates only with permitted files (through admin access). 
The request, transmitted to the **Signing module** which holds the private keys, works simultaneously with the Secured input module that uses OS-specific mechanism. The **Secured input module** protects the passphrase from key grabbers and malware.

3. **Interface layer** is the **UI window** which notifies the user about the details of transactions and necessary actions. The **interface window** is initiated from **Security layer**. Once the user inputs the correct passphrase, it sends the permission to the **Signing module** to unlock the demanded key. Passphrase input field is protected by the secured input module. **Security layer** decrypts the given key with the correct passphrase entered by the user.  In this instance **Signing module** can operate with the open private key, for example it can extract information, sign transactions, therefore responding to given requests.
