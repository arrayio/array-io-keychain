(function(){
  "use strict";

  this.WS =
    function WS(url) {
      this.ws = new WebSocket(url);
      const _parent = this;
      this.ws.onmessage = function(response) {
        try {
          const res = JSON.parse(response.data);
          _parent.queue.shift()(res);
        } catch (e) {
          console.log('response.data: ', response.data);
          console.log('Error: ', e);
        }
      };
      this.queue = [];
    };

  WS.prototype.command = function(request, callback) {
    this.ws.send(JSON.stringify(request));
    this.queue.push(callback);
  };

  /** Promise implementation of the 'command' method */
  WS.prototype.method = function (request) {
    const _parent = this;
    return new Promise(function(resolve, reject) {
      _parent.command(request, resolve);
    });
  };

  WS.prototype.sign = function(data, privateKey) {
    const params = { public_key: privateKey, transaction: data, blockchain_type: "ethereum" };
    return this.method({command: 'sign_hex',  params });
  };

  WS.prototype.signTransaction = function (tx, privateKey) {
    const rsv = this.rsv('', tx.chainId);
    const result = this.getResult(rsv, tx);
    const rawHex = result.rawTransaction;
    const messageHashInitial = result.messageHash;

    const params = { public_key: privateKey, transaction: rawHex, blockchain_type: "ethereum" };
    return this.method({ command: 'sign_hex', params })
      .then(data => {
        const signature = data.result;
        const rsv = this.rsv(signature, tx.chainId);
        const result = this.getResult(rsv, tx);
        return {
          ...rsv,
          rawTransaction: '0x' + result.rawTransaction,
          messageHash: '0x' + messageHashInitial
        }
      });
  };

  WS.prototype.getResult = function(rsv, tx) {
    const txParams = {...tx, ...rsv};
    const ethTx = new ethereumjs.Tx(txParams);
    const buffer = ethTx.serialize();
    const rawTransaction = buffer.toString('hex');
    const messageHash = ethTx.hash().toString('hex');
    return { messageHash, rawTransaction }
  };


  WS.prototype.rsv = function(signature, chainId) {
    const ret = {};
    if (signature) {
      ret.r = `0x${signature.slice(0, 64)}`;
      ret.s = `0x${signature.slice(64, 128)}`;
      const recovery = parseInt(signature.slice(128, 130), 16);
      let tmpV = recovery + 27;
      if (chainId > 0) {
        tmpV += chainId * 2 + 8;
      }
      let hexString = tmpV.toString(16);
      if (hexString.length % 2) {
        hexString = '0' + hexString;
      }
      ret.v = `0x${hexString}`;
    } else {
      ret.r = '0x00';
      ret.s = '0x00';
      ret.v = chainId;
    }
    return ret;
  };

}).call(this);