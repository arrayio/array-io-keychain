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

  WS.prototype.signTransaction = function(tx, privateKey) {
    const rsv = { r: '0x00', s: '0x00', v: tx.chainId };
    const txParams = {...tx, ...rsv};
    const ethTx = new EthJS.Tx(txParams);
    const buffer = ethTx.serialize();
    const rawHex = buffer.toString('hex');

    const params = { keyname: privateKey, transaction: rawHex, blockchain_type: "ethereum" };
    return this.method({command: 'sign_hex',  params });
  };

}).call(this);