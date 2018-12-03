var Web3 = require('web3');
const EthereumTx = require('ethereumjs-tx');
const ethUtil = require('ethereumjs-util');
const rlp = require('rlp');

let flag = false;
let buffer;

const API_KEY = 'https://ropsten.infura.io/v3/046804e3dd3240b09834531326f310cf';
let web3 = new Web3(new Web3.providers.HttpProvider(API_KEY));

const ws = new WebSocket('ws://localhost:16384/');
const keyname = 'test1@6de493f01bf590c0';

const toAdd = '0xE8899BA12578d60e4D0683a596EDaCbC85eC18CC';
const publicKey = '0x6a99ea8d33b64610e1c9ff689f3e95b6959a0cc039621154c7b69c019f015f4521bb9f3fc36a4d447002787d4d408da968185fc5116b8ffd385e8ad3196812e2';
const fromAdd = ethUtil.publicToAddress(publicKey).toString('hex');
const valueTx = 100;
console.log('fromAddress ', fromAdd);

ws.onopen = function() {
  document.body.style.backgroundColor = '#cfc';
};
ws.onclose = function() {
  document.body.style.backgroundColor = null;
};

window.onload = function () {
  document.getElementById('btn_SIGN').addEventListener('click', function() {
    signTransacton();
  });
  document.getElementById('btn_PUBLISH').addEventListener('click', async function() {
    const rowHex = document.getElementById('rowHex').value;
    try {
      const result = await web3.eth.sendSignedTransaction(`0x${rowHex}`);
      document.getElementById('result').value = JSON.stringify(result, undefined, 2);
      document.getElementById('etherscanLink').innerHTML = `<a href='https://ropsten.etherscan.io/tx/${result.transactionHash}'>Etherscan Link</a>`;
    } catch (e) {
      console.log(e);
      document.getElementById('result').value = e;
    }
  });
}

function saveRowHex(rowHex) {
  document.getElementById('rowHex').value = rowHex;
}

async function signTransacton() {
  const rawHex = await buildTxSinature(
    null, // signature
    fromAdd,
    toAdd,
    valueTx
  )
  console.log('sign tx:', rawHex);
  const command = {
    command: "sign_hex",
    params: {
      keyname,
      transaction: rawHex,
      blockchain_type: "ethereum",      
    }
  }
  ws.send(JSON.stringify(command));
}

ws.onmessage = (async (response) => {
  console.log(response.data)
  const data = JSON.parse(response.data);
  console.log(data.result); // signature

  flag = true;
  const rawHex = await buildTxSinature(
    data.result, // signature
    fromAdd,
    toAdd,
    valueTx
  )
  saveRowHex(rawHex);
  console.log('keychain tx:', rawHex);  
});



const buildTxSinature = async (signature, fromAddress, to, value, data = '') => {
  console.log('buildTxSinature')
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

  console.log('tx keychain params', txParams)

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
  if (flag) {
    buffer = tx.serialize()
  } else {
    buffer = tx.hash(false);
  }

  const hex = buffer.toString('hex')

  console.log('final hex: ', hex); 
  return hex;
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