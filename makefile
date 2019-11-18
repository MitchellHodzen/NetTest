CLIENT_DIR = client
SERVER_DIR = server

clt:
	cd $(CLIENT_DIR); make;

cltClean:
	cd $(CLIENT_DIR); make clean;

srv:
	cd $(SERVER_DIR); make;

srvClean:
	cd $(SERVER_DIR); make clean;

clean:
	cd $(CLIENT_DIR); make clean;
	cd $(SERVER_DIR); make clean;