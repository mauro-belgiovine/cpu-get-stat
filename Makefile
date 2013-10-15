CC=gcc
CFLAGS=-I.
DEPS = get_cpu_info.h
OBJ = test.o get_cpu_info.o 

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

get_cpu_info: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o get_cpu_info