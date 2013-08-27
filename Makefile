CC = gcc

CFLAGS = -pipe -Wall -Wextra -std=c99 -pedantic-errors -Werror -O3
CFLAGS = -pipe -Wall -Wextra -std=c99 -pedantic -g


#bundled iniparser
INIPARSER = -Ldeps/iniparser/ -liniparser

LINKERFLAGS = $(INIPARSER) -lpthread -fwhole-program


LINT = splint
LINTFLAGS = +quiet +weak -warnposix


FRONTEND = CLI
TARGET = ReaktorServer$(FRONTEND)
INSTALLATIONDIR = /usr/local/bin
.SUFFIXES:

OBJS = $(FRONTEND)/program.o \
System/Server.o System/Log.o System/Alarm.o\
Monitor/Packet.o Monitor/connection.o Monitor/Client.o Monitor/Printer.o \
Data/Database.o Data/ReactorData.o Data/sensor.o \
Util/LinkedList.o Util/Trie.o Util/Path.o Util/DeathRow.o Util/util.o Util/queue.o Util/Rest.o\


LIBS = deps/iniparser/libiniparser.a \


.PHONY : all
all : $(TARGET)

.PHONY : coloured
coloured :
	@$(MAKE) 2>&1 | sed '{s/\(error\)/[31m\1[m/}' | sed '{s/\(warning\|note\)/[33m\1[m/}'

.PHONY : install
install : $(TARGET)
	cp $(TARGET) $(INSTALLATIONDIR)

.PHONY : clean
clean :
	-rm $(OBJS)
	-cd deps/iniparser && make veryclean
	-rm printer.txt
	-find . -type f -name "*.h.gch" -exec rm -f {} \;

$(TARGET) : $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKERFLAGS) -o $(TARGET)

deps/iniparser/libiniparser.a : deps/iniparser/Makefile
	cd deps/iniparser && make default

%.o : %.c %.h.gch
	-$(LINT) $(LINTFLAGS) $<
	$(CC) $(CFLAGS) -o $@ -c $<

%.h.gch : %.h
	$(CC) $(CFLAGS) -o $@ -c $^
