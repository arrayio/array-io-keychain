(function(){
  "use strict";

  this.WS =
    function WS(url) {
      this.ws = new WebSocket(url);
      const _parent = this;
      this.ws.onmessage = function(response) {
        _parent.queue.shift()(JSON.parse(response.data));
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
  }
}).call(this);