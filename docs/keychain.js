(function(){
  "use strict";

  this.WS =
      class WS {
        constructor(url) {
          this.ws = new WebSocket(url);
          this.ws.onmessage = (response) => {
            this.queue.shift()(JSON.parse(response.data));
          }
          this.queue = [];
        }
      
        command(request, callback) {
          this.ws.send(JSON.stringify(request));
          this.queue.push(callback);
        }
      
        /** Promise implementation of the 'command' method */
        method(request) {
          return new Promise((resolve, reject) => {
          this.command(request, resolve);
        });
      }
    }
}).call(this);