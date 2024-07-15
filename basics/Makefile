
# Compiler
CC=g++
 
# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall      \
         -ansi      \
         -pedantic	

OPCUA_FLAGS=-ldl \
			-lopen62541	

export CC
export CC_FLAGS
export OPCUA_FLAGS

all: 
	make server; make client

server:
	@ cd ./Server; make server

 
client: 
	@ cd ./Client; make client

clean_client:
	@ cd ./Client; make clean

clean_server:
	@ cd ./Server; make clean

clean:
	@ cd ./Client; make clean
	@ cd ./Server; make clean
