CC = gcc

CFLAGS = -pipe -Wall -Wextra -std=c99 -pedantic-errors -Werror -O3
CFLAGS = -pipe -Wall -Wextra -std=c99 -pedantic -g


INIPARSER = -liniparser
#bundled iniparser
INIPARSER = -Liniparser/ -liniparser

LINKERFLAGS = $(INIPARSER) -lpthread -fwhole-program


LINT = splint
LINTFLAGS = +quiet +weak -warnposix -castfcnptr


FRONTEND = CLI
TARGET = ReaktorServer$(FRONTEND)
INSTALLATIONDIR = /usr/local/bin
.SUFFIXES:

OBJS = $(FRONTEND)/program.o \
		System/Server.o System/Log.o System/Alarm.o\
		Monitor/connection.o Monitor/Printer.o \
		Data/Database.o Data/ReactorData.o Data/sensor.o \
		Util/LinkedList.o Util/Trie.o Util/Path.o Util/DeathRow.o Util/util.o \
		

.PHONY : all
all : $(TARGET)

.PHONY : install
install : $(TARGET)
	cp $(TARGET) $(INSTALLATIONDIR)

.PHONY : clean
clean :
	-rm $(OBJS)
	-cd iniparser && make clean
	.rm printer.txt

$(TARGET) : $(OBJS)
	cd iniparser && make
	$(CC) $(CFLAGS) $(OBJS) $(LINKERFLAGS) -o $(TARGET)

%.o : %.c %.h.gch
	-$(LINT) $(LINTFLAGS) $<
	$(CC) $(CFLAGS) -o $@ -c $< 

%.h.gch : %.h
	$(CC) $(CFLAGS) -o $@ -c $^
