(function(){
  "use strict";

  this.Keychain =
      class Keychain {
        constructor(url) {
          this.ws = new WebSocket(url);
          this.ws.onmessage = (response) => {
            this.queue.shift()(JSON.parse(response.data));
          }
          this.queue = [];
        }
      
        command(name, params, callback) {
          const request = Object.assign(Keychain.commands[name], {});
          if (request.params) {
            request.params = Object.assign(request.params, params);
          }
          this.ws.send(JSON.stringify(request));
          this.queue.push(callback);
        }
      
        /** Promise implementation of the 'command' method */
        method(name, params) {
            return new Promise((resolve, reject) => {
            this.command(name, params, resolve)
          });
        }
      
        static get commands() {
          return {
            UNLOCK: {
              command: "unlock",
              params: {
                // keyname: null,
                // unlock_time: null
              }
            },
            PUBLIC_KEY: {    
              command: "public_key",
              params: {
                // keyname: null
              }
            },
            SIGN_HEX: {
              command: "sign_hex",
              params: {
                chainid: "de5f4d8974715e20f47c8bb609547c9e66b0b9e31d521199b3d8d6af6da74cb1",
                transaction: "871689d060721b5cec5a010080841e00000000000011130065cd1d0000000000000000",
                blockchain_type: "array",
                // keyname: null,
                // unlock_time: null
              }
            },
            SIGN_HASH: {
              command: "sign_hash",
              params: {
                sign_type: "VRS_canonical",
                hash: "fe5e4a8974715e20f47c8bb609547c9e66b0b9e31d521199b3d8d6af6da74cb1",
                // keyname: null
              }
            },
            CREATE: {
              command: "create",
              params: {
                keyname: "test1",
                encrypted: true,
                curve: "secp256k1",
                cipher: "aes256"
              }
            },
            LIST: {
              command: "list"
            },
            LOCK: {
              command: "lock"
            },
            ABOUT: {
              command: "about"
            },
            VERSION: {
              command: "version"
            }
          }
        }
      }
      

}).call(this);