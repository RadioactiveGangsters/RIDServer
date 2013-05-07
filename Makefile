CC = gcc
CFLAGS = -O3 -pedantic-errors -Wall -Wextra -Werror -std=c99
CFLAGS = -Wall -std=c99
FRONTEND = CLI
TARGET = ReaktorServer$(FRONTEND)
INSTALLATIONDIR = /usr/local/bin

OBJS = $(FRONTEND)/program.o \
		System/Alarm.o System/Control.o System/Printer.o System/Server.o \
		Monitor/Client.o Monitor/Network.o \
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
	 $(CC) $(CFLAGS) -o $@ @<
