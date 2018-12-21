var Web3 = require('web3');
const Transaction = require('ethereumjs-tx');
const ethUtil = require('ethereumjs-util');
const WS  = require('../../keychain').WS;
let rawHexGlobal;

window.onload = function () {
  let web3;
  const valueTx = 100;

  const keychain = new WS('ws://localhost:16384/');

  keychain.ws.onopen = function() {
    document.body.style.backgroundColor = '#cfc';
    keychain.method({command: 'version'})
    .then(data => {
      document.getElementById('version').innerText = data.result;
    })
  };
  keychain.ws.onclose = function() {
    document.body.style.backgroundColor = null;
    document.getElementById('error').style.display = 'block';
  };

  document.getElementById('btn_SIGN').addEventListener('click', function() {
    const endpoint = document.getElementById('input-endpoint').value;
    web3 = new Web3(new Web3.providers.HttpProvider(endpoint));
    const keyname = document.getElementById('input-keyname').value;
    const toAddress = document.getElementById('input-to-address').value;
    let fromAddress;
    
    log(`Getting public key by keyname from KeyChain: ${keyname}`);
    keychain.method({ command: 'public_key', params: { keyname } }).then(data => {
      log(`Result: ${JSON.stringify(data)}`);
      if (data.error) {
        throw data.error;
      }
      const publicKey = `0x${data.result}`;
      return ethUtil.publicToAddress(publicKey).toString('hex');
    }).then( data => {
      fromAddress = data;
      log(`"From" address calculated from public key: ${fromAddress}`);
      log('Building transaction with empty signature');
      return buildTxSinature('', fromAddress, toAddress, valueTx);
    }).then (rawHex => {
      log(`Result: ${rawHex}`);
      const params = { keyname, transaction: rawHex, blockchain_type: "ethereum" };
      log(`Signing it with KeyChain: ${JSON.stringify(params)}}`);
      return keychain.method({command: 'sign_hex',  params });
    }).then( data => {
      const signature = data.result;
      log(`Signature: ${signature}`);
      return buildTxSinature(signature, fromAddress, toAddress, valueTx);
    }).then( rawHex => {
      log(`Transaction built, rawHex: ${rawHex}`);
      log(`Please, publish your transaction`);
      saveRawHex(rawHex);
    }).catch( (error) => {
      log(error);
    })
  });

  document.getElementById('btn_PUBLISH').addEventListener('click', async function() {
    if (!rawHexGlobal) {
      log('Please sign your transaction');
      return;
    }
    log(`Sending signed transaction: ${rawHexGlobal}`);
    try {
      document.getElementById('progress').style.display = 'inline-block';
      const result = await web3.eth.sendSignedTransaction(`0x${rawHexGlobal}`);
      document.getElementById('progress').style.display = 'none';
      log(`Transfer result: \n${JSON.stringify(result, undefined, 2)}`);
      document.getElementById('etherscanLink').innerHTML = `<a href='https://ropsten.etherscan.io/tx/${result.transactionHash}'>Etherscan Link</a>`;
    } catch (e) {
      console.log(e);
      log(e);
    }
  });

  const buildTxSinature = async (signature, fromAddress, to, value, data = '') => {
    const nonce = await web3.eth.getTransactionCount(fromAddress);
    const gasPrice = await web3.eth.getGasPrice().then(wei => Number(wei))
    const chainId = 3;
    const gasLimit = 21000; // await web3.eth.estimateGas(draftTxParams) ||    

    const ret = rsv(signature, chainId);

    const txParams = {
      nonce,
      gasPrice,
      to,
      value,
      data,
      // EIP 155 chainId - mainnet: 1, ropsten: 3, rinkeby: 4
      chainId,
      gasLimit,
      ...ret
    }
    // console.log('tx KeyChain params', txParams);
    const tx = new Transaction(txParams);
    const buffer = tx.serialize();  
    return buffer.toString('hex');
  }
}

const rsv = (signature, chainId) => {
  const ret = {};
  if (signature) {
    ret.r = `0x${signature.slice(0, 64)}`;
    ret.s = `0x${signature.slice(64, 128)}`;
    const recovery = parseInt(signature.slice(128, 130), 16);
    let tmpV = recovery + 27;
    if (chainId > 0) {
      tmpV += chainId * 2 + 8;
    }
    ret.v = tmpV;
  } else {
    ret.r = '0x00';
    ret.s = '0x00';
    ret.v = chainId;
  }
  return ret;
}

function saveRawHex(rawHex) {
  document.getElementById('rawHex').innerText = rawHex;
  rawHexGlobal = rawHex;
}

function log(message) {
  const loggerElement = document.getElementById('log');
  if (loggerElement.value) {
    loggerElement.value += '\n';
  }
  loggerElement.value += message;
}