CC := gcc
CFLAGS := -g -lm -pg -ggdb -O0 -std=c11 -Wall -Wno-missing-braces
TARGET := main 

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS))

b: $(TARGET)
$(TARGET): $(OBJS)
	$(CC)  -o $@ $^ -lm

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

r:
	./main > image.ppm && display image.ppm

len:
	find . -name '*.c' | xargs wc -l

clean:
	rm -rf obj/ $(TARGET)

.PHONY: all clean

