CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic -Iinclude

SRCS = test/scheduler_test.c src/scheduler.c src/task.c src/pqueue.c src/uid.c src/sorted_list.c src/dlist.c src/timer.c
OBJS = $(patsubst %, bin/%, $(notdir $(SRCS:.c=.o)))
TARGET = bin/scheduler_test
TIMER_TARGET = bin/timer_test
TIMER_OBJS = bin/timer_test.o bin/timer.o

VPATH = src:test

.PHONY: all clean run vlg timer timer_run

all: $(TARGET)

timer: $(TIMER_TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

$(TIMER_TARGET): $(TIMER_OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^



bin/%.o: %.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/timer_test.o: test/timer_test.c include/timer.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/timer.o: src/timer.c include/timer.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

timer_run: $(TIMER_TARGET)
	./$(TIMER_TARGET)

vlg: $(TARGET)
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)
