client: basic_client.o ClientAgent.o
	@ $(CC) -o client basic_client.o ClientAgent.o $(OPCUA_FLAGS)
	@ echo 'Finished building binary: $@'
 
basic_client.o: basic_client.cpp ClientAgent.h
	@ $(CC) -o basic_client.o basic_client.cpp $(CC_FLAGS) $(OPCUA_FLAGS)
	@ echo 'Building target using GCC compiler: $<'
 
ClientAgent.o: ClientAgent.cpp ClientAgent.h
	@ $(CC) -o ClientAgent.o ClientAgent.cpp $(CC_FLAGS)
	@ echo 'Building target using GCC compiler: $<'

clean:
	rm -rf *.o *~ client