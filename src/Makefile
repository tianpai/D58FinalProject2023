# Define the compiler to use
CC=gcc

# Compiler flags:
# -Wall and -Wextra for additional warnings
# For macOS, adjust the include and library paths according to your OpenSSL installation
CFLAGS=-Wall -Wextra -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib

# Libraries to link against (SSL and crypto libraries)
LIBS=-lssl -lcrypto

# Source file for the server
SERVER_SRC=server.c

# Source file for the client
CLIENT_SRC=client.c

# Output executable name for the server
SERVER_OUT=server

# Output executable name for the client
CLIENT_OUT=client

# Default target: build both server and client
all: $(SERVER_OUT) $(CLIENT_OUT)

# Rule to build the server executable
$(SERVER_OUT): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_OUT) $(SERVER_SRC) $(LIBS)

# Rule to build the client executable
$(CLIENT_OUT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_OUT) $(CLIENT_SRC) $(LIBS)

# Clean rule: remove compiled executables
clean:
	rm -f $(SERVER_OUT) $(CLIENT_OUT)

