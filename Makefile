CC = clang

SERVER_LIBS = -lc -lstdc++ -lpthread

SERVER_PATH = .

SERVER_INC = -I$(SERVER_PATH)/include

CFLAGS = -c -mtune=native -Wall -pedantic

LDFLAGS  = -L.

SERVER_SRC=$(SERVER_PATH)/application/familytree/main.cpp $(SERVER_PATH)/source/atom.cpp $(SERVER_PATH)/source/class.cpp $(SERVER_PATH)/source/keyname.cpp $(SERVER_PATH)/source/object.cpp $(SERVER_PATH)/source/odb.cpp $(SERVER_PATH)/source/reason.cpp $(SERVER_PATH)/source/root.cpp $(SERVER_PATH)/source/rope.cpp $(SERVER_PATH)/source/stream.cpp $(SERVER_PATH)/source/transaction.cpp $(SERVER_PATH)/source/atom/data.cpp $(SERVER_PATH)/source/atom/data3dpoint.cpp $(SERVER_PATH)/source/atom/databinary.cpp $(SERVER_PATH)/source/atom/datafile.cpp $(SERVER_PATH)/source/atom/datafilelink.cpp $(SERVER_PATH)/source/atom/datainteger.cpp $(SERVER_PATH)/source/atom/datareal.cpp $(SERVER_PATH)/source/atom/datastring.cpp
SERVER_OBJ=$(SERVER_SRC:.cpp=.o)
SERVER=odb.server

all: $(SERVER)

server: $(SOCKET)

$(SERVER): $(SERVER_OBJ) $(SOCKET)
	$(CC) $(SERVER_OBJ) $(LDFLAGS) $(SERVER_LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(SERVER_INC)

clean:
	rm -f $(SERVER_OBJ) $(SERVER)