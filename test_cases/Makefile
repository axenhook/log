CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -I../

POBJS = 

TEST_LOG = test_log

TEST_LOG_SRC = test_log.c ../log.c

TARGET_ALL = $(TEST_LOG)

all: $(TARGET_ALL)

$(TEST_LOG):
	gcc -o $@ $(TEST_LOG_SRC) $(CFLAGS)
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
