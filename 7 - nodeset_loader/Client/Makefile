client: client.o ClientAgent.o
	@ $(CC) -o client client.o ClientAgent.o $(OPCUA_FLAGS)
	@ echo 'Finished building binary: $@'
 
client.o: client.cpp ClientAgent.h
	@ $(CC) -o client.o client.cpp $(CC_FLAGS) $(OPCUA_FLAGS)
	@ echo 'Building target using GCC compiler: $<'
 
ClientAgent.o: ClientAgent.cpp ClientAgent.h
	@ $(CC) -o ClientAgent.o ClientAgent.cpp $(CC_FLAGS)
	@ echo 'Building target using GCC compiler: $<'

clean:
	rm -rf *.o *~ client