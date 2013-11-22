CC=gcc
CFLAGS=-I.
LDFLAGS=-lm
DEPS = get_cpu_info.h
OBJ = test.o get_cpu_info.o 

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

get_cpu_info: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm *.o get_cpu_info