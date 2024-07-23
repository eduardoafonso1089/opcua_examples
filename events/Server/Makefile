server: server.o CreatorIM.o
	@ $(CC) -o server server.o CreatorIM.o $(OPCUA_FLAGS)
	@ echo 'Finished building binary: $@'
 
server.o: server.cpp CreatorIM.h
	@ $(CC) -o server.o server.cpp $(CC_FLAGS) $(OPCUA_FLAGS)
	@ echo 'Building target using GCC compiler: $<'
 
CreatorIM.o: CreatorIM.cpp CreatorIM.h
	@ $(CC) -o CreatorIM.o CreatorIM.cpp $(CC_FLAGS)
	@ echo 'Building target using GCC compiler: $<'

clean:
	rm -rf *.o *~ server