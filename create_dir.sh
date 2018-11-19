# !/bin/sh
if [ ! -d "/var/keychain" ]; then
	sudo mkdir /var/keychain
	sudo chown -R $USER keychain
fi 


