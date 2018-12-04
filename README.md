![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/ltl1%20(2).png) 
# KeyChain

## Overview

**KeyChain** is a standalone app for signing transactions and generating key pairs. It stores private keys in an isolated environment where no logger, debugger or spyware can intercept them because of the three-layer security protecting each action of the system.
**KeyChain** supports transactions from and to various blockchains, including Ethereum and Ethereum classic, Litecoin, Bitcoin, and Bitshares. 

You can integrate **KeyChain** as a service through `pipe` or `websocket` methods. For quick step-by-step guides see [Pipe integration guide](https://github.com/arrayio/array-io-keychain/wiki/Pipe-API) and [WebSocket integration guide](https://github.com/arrayio/array-io-keychain/wiki/WebSocket-API) that you can find in the corresponding API. For simple installation tutorials for Windows, MacOs, and Linux go to [Installation guides](https://github.com/arrayio/array-io-keychain/wiki/Installation-guides). 

Read full [KeyChain Protocol](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-Protocol) with comprehensive descriptions of JSON requests broken down into simple commands and parameters. 

### Demo pages

[Here](https://arrayio.github.io/array-io-keychain/eth_signer/) you can try out signing Ethereum transactions with KeyChain.

Test the KeyChain commands on the demo page [here](https://arrayio.github.io/array-io-keychain/demo/).

You can find the detailed technical specification of the KeyChain system below if you refer to [How it works](#how-it-works) section. 

## Installation

Download KeyChain and run the graphic installer if you are using [Windows](#windows) or [MacOs](#macos). [Linux](#linux) installer is coming soon.

### MacOs

#### System requirements

MacOs 10.12 or newer.

#### Download MacOs installer [here](https://github.com/arrayio/array-io-keychain/releases/download/0.7/KeyChain.Installer.v0.7.zip).

Follow the steps of the graphic installer. 

1. Click "next" to start installation
![text](https://github.com/arrayio/array-io-keychain/blob/master/img/keychain%20mac.png)

2. Choose a folder and click "next"
![text](https://github.com/arrayio/array-io-keychain/blob/master/img/keychain%202.png)

3. Click "install" for installation to start

![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/keychain%20install.png)

4. Wait until the setup is done
![alt text](
https://github.com/arrayio/array-io-keychain/blob/master/img/keychain%20direct.png)

5. Congratulations! You have installed KeyChain.

![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/keychain%20complete.png)

### Windows

#### System requirements

Windows 7 or newer.

Run the installer and follow the steps. 

1. Click "next" to prepare installation

![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/windows1.png)

2. Accept the terms of the License and click "next"

![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/windows3.png)

3. Choose a folder, click "next"

![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/windows4.png)

4. Click "install" for installation to start

![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/windows5.png)

5. Wait until the setup is done

![alt text](https://github.com/arrayio/array-io-keychain/blob/master/img/windows6.png)

### Linux

Coming soon

After successful installation you will be returned to the web page or app you started from. The WebSocket server will be ready to work.

## <a name="How it works"></a>How it works

![Three-layer security](https://github.com/arrayio/array-io-keychain/blob/master/img/Diagram%20Keychain%20fin%201.png)

Apps or websites send requests to the KeyChain through two types of communication - standard I/O streams (mostly called pipes), and the WebSocket. 
The architecture of the KeyChain software consists of the three independent layers:

1. **API layer** which integrates with your app, website or any external application. It is language-neutral. The protocol for the terminal application operates with the JSON format in synchronous request/response way. The main function of the **API layer** is to transmit and parse commands for given API. 
Each request carries information about commands, the type of key user wants to use to sign transactions and other relevant parameters which you can find in the [Protocol](https://github.com/arrayio/array-io-keychain/wiki/KeyChain-API). 

2. **Security layer** receives the commands from the API layer and acts as an OS-specific  protection mechanism for the **interface window** (third layer). It serves as a shield from potential attacks at sensitive data and information. **Security layer** is tailored for the Mac OS, Linux, and Windows OS and operates only with permitted files (through admin access). 
The request, transmitted to the **Signing module** which holds the private keys, works simultaneously with the Secured input module that uses OS-specific mechanism. The **Secured input module** protects the passphrase from key grabbers and malware.

3. **Representation layer** is the **UI window** which notifies the user about the details of transactions and necessary actions. The **interface window** is initiated from **Security layer**. Once the user inputs the correct passphrase, it sends the permission to the **Signing module** to unlock the demanded key. Passphrase input field is protected by the secured input module. **Security layer** decrypts the given key with the correct passphrase entered by the user.  In this instance **Signing module** can operate with the open private key, for example it can extract information, sign transactions, therefore responding to given requests.

## How to use

For more information, see corresponding wiki https://github.com/arrayio/array-io-keychain/wiki
