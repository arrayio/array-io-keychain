var Web3 = require('web3');
const EthereumTx = require('ethereumjs-tx');
const ethUtil = require('ethereumjs-util');
const rlp = require('rlp');

let buffer;
const WS_PUBLIC_KEY_REQUEST_ID = 12345;

window.onload = function () {
  let web3;
  let keyname;
  let toAddress;
  let fromAddress;
  const valueTx = 100;

  const ws = new WebSocket('ws://localhost:16384/');

  ws.onopen = function() {
    document.body.style.backgroundColor = '#cfc';
  };
  ws.onclose = function() {
    document.body.style.backgroundColor = null;
  };
  ws.onclose = function () {
    log('Websocket has been closed. Check for errors in the browser console and if KeyChain is installed');
  }

  ws.onmessage = (async (response) => {
    log(`KeyChain response: ${response.data}`);
    const data = JSON.parse(response.data);
    if (data.result) {
      if (data.id === WS_PUBLIC_KEY_REQUEST_ID) {
        const publicKey = `0x${data.result}`;
        fromAddress = ethUtil.publicToAddress(publicKey).toString('hex');
        log(`"From" address calculated from public key: ${fromAddress}`);
        signTransacton();
        return;
      }

      console.log(data.result); // signature
      log(`Building transaction with signed by KeyChain transaction`);

      const rawHex = await buildTxSinature(
        data.result, // signature
        fromAddress,
        toAddress,
        valueTx
      )
      log(`Transaction built, rawHex: ${rawHex}`);
      log(`Please, publish your transaction`);
      saveRawHex(rawHex);
    } else {
      log(`KeyChain retured error: ${data.error}`);
    }
  });


  document.getElementById('btn_SIGN').addEventListener('click', function() {
    const endpoint = document.getElementById('input-endpoint').value;
    web3 = new Web3(new Web3.providers.HttpProvider(endpoint));
    keyname = document.getElementById('input-keyname').value;
    toAddress = document.getElementById('input-to-address').value;

    getPublicKey();
  });

  document.getElementById('btn_PUBLISH').addEventListener('click', async function() {
    const rawHex = document.getElementById('rawHex').innerText;
    log(`Sending signed transaction: ${rawHex}`);
    try {
      document.getElementById('progress').style.display = 'inline-block';
      const result = await web3.eth.sendSignedTransaction(`0x${rawHex}`);
      document.getElementById('progress').style.display = 'none';
      log(`Transfer result: \n${JSON.stringify(result, undefined, 2)}`);
      document.getElementById('etherscanLink').innerHTML = `<a href='https://ropsten.etherscan.io/tx/${result.transactionHash}'>Etherscan Link</a>`;
    } catch (e) {
      console.log(e);
      log(e);
    }
  });

  async function signTransacton() {    
    log('Building transaction signature');
    const rawHex = await buildTxSinature(
      '',
      fromAddress,
      toAddress,
      valueTx
    );
    console.log('sign tx:', rawHex);
    const command = {
      command: "sign_hex",
      params: {
        keyname,
        transaction: rawHex,
        blockchain_type: "ethereum",      
      }
    }
    log(`Result: ${rawHex}`);
    log(`Signing it with KeyChain: ${JSON.stringify(command)}`);
    ws.send(JSON.stringify(command));
  }

  function getPublicKey() {
    const command = { id: WS_PUBLIC_KEY_REQUEST_ID, command: "public_key", params: { keyname } };
    log(`Getting public key by keyname from KeyChain: ${JSON.stringify(command)}`);
    ws.send(JSON.stringify(command));
  }

  const buildTxSinature = async (signature, fromAddress, to, value, data = '') => {
    const nonce = await web3.eth.getTransactionCount(fromAddress);
    const gasPrice = await web3.eth.getGasPrice().then(wei => Number(wei))
    const chainIdHere = 3;
  
    const draftTxParams = {
      nonce,
      gasPrice,
      to,
      value,
      data,
      // EIP 155 chainId - mainnet: 1, ropsten: 3, rinkeby: 4
      chainId: chainIdHere
    }
  
    const gasLimit = 21000; // await web3.eth.estimateGas(draftTxParams) ||
  
    let txParams = {
      ...draftTxParams,
      gasLimit
    }
  
    if (signature) {
      const ret = rsv(signature, chainIdHere);
      txParams = { ...txParams,
        ...ret
      };
    }
  
    console.log('tx KeyChain params', txParams)
  
    class EthereumTxKeychain extends EthereumTx {
      hash(includeSignature) {
        if (includeSignature === undefined) includeSignature = true
  
        // EIP155 spec:
        // when computing the hash of a transaction for purposes of signing or recovering,
        // instead of hashing only the first six elements (ie. nonce, gasprice, startgas, to, value, data),
        // hash nine elements, with v replaced by CHAIN_ID, r = 0 and s = 0
  
        let items
        if (includeSignature) {
          items = this.raw
        } else {
          if (this._chainId > 0) {
            const raw = this.raw.slice()
            this.v = this._chainId
            this.r = 0
            this.s = 0
            items = this.raw
            this.raw = raw
          } else {
            items = this.raw.slice(0, 6)
          }
        }
        // create hash
        return rlp.encode(items)
      }
    }
  
    const tx = new EthereumTxKeychain(txParams);
    if (signature) {
      buffer = tx.serialize()
    } else {
      buffer = tx.hash(false);
    }
  
    const hex = buffer.toString('hex')
  
    console.log('final hex: ', hex); 
    return hex;
  }
}

function saveRawHex(rawHex) {
  document.getElementById('rawHex').innerText = rawHex;
}

const rsv = (signature, chainIdHere) => {
  const ret = {};
  ret.r = `0x${signature.slice(0, 64)}`;
  ret.s = `0x${signature.slice(64, 128)}`;
  const recovery = parseInt(signature.slice(128, 130), 16);
  let tmpV = recovery + 27;
  if (chainIdHere > 0) {
    tmpV += chainIdHere * 2 + 8;
  }
  ret.v = tmpV;
  return ret;
}

function log(message) {
  const loggerElement = document.getElementById('log');
  if (loggerElement.value) {
    loggerElement.value += '\n';
  }
  loggerElement.value += message;
}