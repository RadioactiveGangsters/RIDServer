CC = gcc
CFLAGS = -pipe -O3 -pedantic-errors -Wall -Wextra -Werror -std=c99 -x c
CFLAGS = -pipe -Wall -std=c99 -pedantic
LINKERFLAGS = -l iniparser -fwhole-program
FRONTEND = CLI
LINT = splint
LINTFLAGS = +quiet +posix-strict-lib +weak
TARGET = ReaktorServer$(FRONTEND)
INSTALLATIONDIR = /usr/local/bin
.SUFFIXES:

OBJS = $(FRONTEND)/program.o \
		System/Server.o System/Log.o\
		Monitor/Client.o Monitor/Network.o Monitor/connection.o Monitor/toFile.o \
		Data/Database.o Data/ReactorData.o Data/sensor.o \
		Util/LinkedList.o Util/Trie.o Util/Path.o Util/util.o
		

.PHONY : all
all : $(TARGET)

.PHONY : install
install : $(TARGET)
	cp $(TARGET) $(INSTALLATIONDIR)

.PHONY : clean
clean :
	-rm $(OBJS)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(LINKERFLAGS) -o $(TARGET) $(OBJS)

%.o : %.c %.h.gch
	-$(LINT) $(LINTFLAGS) $<
	$(CC) $(CFLAGS) -o $@ -c $< 

%.h.gch : %.h
	$(CC) $(CFLAGS) -o $@ -c $^
