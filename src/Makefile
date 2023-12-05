CC = gcc 								# Compiler to use

#headers are in the same directory as the Makefile 
FLAGS = -I.
SHOW_WARNINGS = -Wall

ALL_FLAGS = $(FLAGS) $(SHOW_WARNINGS)


# Object files that are common to both client and server
COMMON_OBJ = packet.o\
			 utils.o\
			 encrypt.o\
			 encap.o\
			 decap.o\

CLIENT_OBJ = client.o $(COMMON_OBJ)
SERVER_OBJ = server.o $(COMMON_OBJ)

# all target builds both client and server
all: vpn_client vpn_server

# client: dependes on CLIENT_OBJ
vpn_client: $(CLIENT_OBJ)
	# link all the object files to single binary named vpn_client
	$(CC) -o vpn_client $(CLIENT_OBJ)

vpn_server: $(SERVER_OBJ)
	# link all the object files to single binary named vpn_server
	$(CC) -o vpn_server $(SERVER_OBJ)

# rule for generating object files from .c files
# %.o: %.c means "for all" .o files,
# use the corresponding .c file to generate the .o file
%.o: %.c
	# compile the .c file to generate the .o file
	# $< is the first prerequisite (the .c file)
	# $@ is the target (the .o file)
	$(CC) -c -o $@ $< $(ALL_FLAGS)

clean:
	rm -f *.o vpn_client vpn_server
