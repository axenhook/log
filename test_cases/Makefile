CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -I../ -std=c99

POBJS = 

TEST_LOG = test_log
TEST_LOG_INTERNAL = test_log_internal

TEST_LOG_SRC = test_log.c ../log.c threads_group.c test_module_a.c test_module_b.c
TEST_LOG_INTERNAL_SRC = test_log_internal.c ../log.c threads_group.c 

TARGET_ALL = $(TEST_LOG) $(TEST_LOG_INTERNAL)

all: $(TARGET_ALL)

$(TEST_LOG):
	gcc -o $@ $(TEST_LOG_SRC) $(CFLAGS)
	
$(TEST_LOG_INTERNAL):
	gcc -o $@ $(TEST_LOG_INTERNAL_SRC) $(CFLAGS)
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
