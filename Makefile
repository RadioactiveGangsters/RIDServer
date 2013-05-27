CC = gcc
CFLAGS = -pipe -Wall -Wextra -std=c99 -pedantic-errors -Werror -O3
CFLAGS = -pipe -Wall -Wextra -std=c99 -pedantic -g
LINKERFLAGS = -l iniparser -lpthread -fwhole-program
FRONTEND = CLI
LINT = splint
LINTFLAGS = +quiet -warnposix +weak -castfcnptr
TARGET = ReaktorServer$(FRONTEND)
INSTALLATIONDIR = /usr/local/bin
.SUFFIXES:

OBJS = $(FRONTEND)/program.o \
		System/Server.o System/Log.o\
		Monitor/connection.o Monitor/Printer.o \
		Data/Database.o Data/ReactorData.o Data/sensor.o \
		Util/LinkedList.o Util/Trie.o Util/Path.o Util/util.o Util/DeathRow.o
		

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
