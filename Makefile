CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -std=c99

POBJS = 

FILE_TRANSLATOR_OBJS = file_translator.o

FILE_TRANSLATOR = file_translator

TARGET_ALL = $(FILE_TRANSLATOR)

all: $(TARGET_ALL)

%o:%c
	$(CC) -c $(CFLAGS) $< -o $@

$(FILE_TRANSLATOR): $(POBJS) $(FILE_TRANSLATOR_OBJS)
	$(CXX) -o $@ $^


clean:
	rm -f *.o $(TARGET_ALL)
