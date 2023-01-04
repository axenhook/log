CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -I../

POBJS = 

TEST_LOG_MGR = test_log_mgr

TEST_LOG_MGR_SRC = test_log_mgr.c ../log_mgr.c

TARGET_ALL = $(TEST_LOG_MGR)

all: $(TARGET_ALL)

$(TEST_LOG_MGR):
	gcc -o $@ $(TEST_LOG_MGR_SRC) $(CFLAGS)
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
