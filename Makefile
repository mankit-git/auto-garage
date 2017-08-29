SRC = $(wildcard *.c)

CC=arm-none-linux-gnueabi-gcc

CPPFLAGS += -I /home/gec/jpeglib/include/
CPPFLAGS += -I /home/gec/armsqlite/include/
LDFLAGS += -L /home/gec/jpeglib/lib/
LDFLAGS += -L /home/gec/armsqlite/lib/
LDFLAGS += -ljpeg
LDFLAGS += -lpthread
LDFLAGS += -lsqlite3
#LDFLAGS += -Wall
#ENVFLAGS += -Wl,-rpath=/mnt/hgfs/winshare/linux/project/lib

main:$(SRC)
	$(CC) $^ -o $@ $(CPPFLAGS) $(LDFLAGS) $(ENVFLAGS)

	

