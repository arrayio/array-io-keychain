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
    const params = { keyname: privateKey, transaction: data, blockchain_type: "ethereum" };
    return this.method({command: 'sign_hex',  params });
  };

  WS.prototype.signTransaction = function (tx, privateKey) {
    tx.gasLimit = tx.gas; // rename gas to gasLimit for using with new EthJS.Tx()
    const rsv = this.rsv('', tx.chainId);
    const rawHex = this.getRawHex(rsv, tx);

    const params = { keyname: privateKey, transaction: rawHex, blockchain_type: "ethereum" };
    return this.method({ command: 'sign_hex', params })
      .then(data => {
        const signature = data.result;
        const rsv = this.rsv(signature, tx.chainId);
        return this.getRawHex(rsv, tx);
      });
  };

  WS.prototype.getRawHex = function(rsv, tx) {
    const txParams = {...tx, ...rsv};
    const ethTx = new EthJS.Tx(txParams);
    const buffer = ethTx.serialize();
    return buffer.toString('hex');
  }


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
      ret.v = tmpV;
    } else {
      ret.r = '0x00';
      ret.s = '0x00';
      ret.v = chainId;
    }
    return ret;
  };

}).call(this);