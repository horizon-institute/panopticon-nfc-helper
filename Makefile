CC=gcc
OBJ=nfc-helper.o

nfc-helper: $(OBJ)
	$(CC) -o $@ $^ `pkg-config --libs libnfc`

%.o: %.c
	$(CC) -c -o $@ $< `pkg-config --cflags libnfc`

clean:
	rm -f *.o nfc-helper
