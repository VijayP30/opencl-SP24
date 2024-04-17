CC = gcc
CFLAGS = -I/usr/local/cuda/include
LDFLAGS = -L/usr/local/cuda/lib64 -lOpenCL

all: vector_add device_info

vector_add: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

device_info: device_info.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean: 
	rm -f vector_add device_info

