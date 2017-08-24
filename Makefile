SRC = $(wildcard *.c)

CC=arm-none-linux-gnueabi-gcc

CPPFLAGS += -I /home/gec/jpeglib/include/
LDFLAGS += -L /home/gec/jpeglib/lib/
LDFLAGS += -ljpeg
#ENVFLAGS += -Wl,-rpath=/mnt/hgfs/winshare/linux/project/lib

main:$(SRC)
	$(CC) $^ -o $@ $(CPPFLAGS) $(LDFLAGS) $(ENVFLAGS)

	

