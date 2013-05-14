CC = gcc
CFLAGS = -O3 -pedantic-errors -Wall -Wextra -Werror -std=c99
CFLAGS = -Wall -std=c99
FRONTEND = CLI
LINT = splint
TARGET = ReaktorServer$(FRONTEND)
INSTALLATIONDIR = /usr/local/bin

OBJS = $(FRONTEND)/program.o \
		System/Alarm.o System/Control.o System/Printer.o System/Server.o System/connection.o \
		Monitor/Client.o Monitor/Network.o Monitor/toFile.o \
		Data/Database.o Data/ReactorData.o \
		Util/LinkedList.o Util/Trie.o \
		

.PHONY: all
all: $(TARGET)

.PHONY: install
install: $(TARGET)
	cp $(TARGET) $(INSTALLATIONDIR)

.PHONY: clean
clean:
	rm $(OBJS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o: %c
	$(LINT) %c
	$(CC) $(CFLAGS) -o $@ @<
