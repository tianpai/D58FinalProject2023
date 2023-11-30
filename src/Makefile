CC = gcc 								# Compiler to use

# this openssl path is Macos specific
# to changed to linux, you need to make sure the openssl is installed
# and change the path to the openssl include and lib

# for linux, you can use the following path
# (this may not be correct not you can uncomment the following two lines)
# CFLAGS = -Wall -I/usr/include/openssl
# LDFLAGS = -L/usr/lib -lssl -lcrypto

CFLAGS = -Wall -I/usr/local/opt/openssl@3/include
LDFLAGS = -L/usr/local/opt/openssl@3/lib -lssl -lcrypto

# List of your .c files
SRC = \
client.c \
server.c \
encapsulation.c \
packet.c \
encrypt.c \
slidewin.c \
utils.c \
decap.c \

# Corresponding .o files
OBJ = $(SRC:.c=.o)

# Name of your final executable
TARGET = svpn

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean 
