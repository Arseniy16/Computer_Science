CC = gcc
CFLAGS = -Wall

SOURCES = list.c test.c sort_list.c 
OBJECTS = $(SOURCES:.c=.o)
EXEC = list

.PHONY: all clean

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) 

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXEC)