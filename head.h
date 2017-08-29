#ifndef __HEAD_H_
#define __HEAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>
#include "jpeglib.h"
#include <errno.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <setjmp.h>
#include <time.h>
#include <sys/select.h>
#include <termios.h>
#include "sqlite3.h"

#define DEV_PATH   "/dev/ttySAC1"

unsigned int cardid ;
//sqlite3 *db;
extern struct info *cardinfo;
struct timeval timeout;

struct image_info
{
	int width;
	int height;
	int pixel_size;
};

struct coordinate
{
	int x;
	int y;
};

struct info
{
	unsigned int id;
	char *time;
	char carnum[10];
};
struct coordinate *xy;
extern time_t t_car_in;

char *read_image_from_file(int fd, unsigned char *jpg_buffer, unsigned long image_size);
int touchlcd(int ts, struct coordinate *xy);
int judge(int ts, struct coordinate *xy, sqlite3 *db, struct info *cardinfo);

int create_table(sqlite3 *db);
int sqlite3_insert(sqlite3 *db, struct info *cardinfo);
int callback(void *data, int n, char **cloumn_name, char **cloumn_value);
int sqlite3_del(sqlite3 *db, struct info *cardinfo);
int sqlite(sqlite3 *db, struct info *cardinfo);
//int in_table(sqlite3 *db, struct info *cardinfo);
int out_table(sqlite3 *db, struct info *cardinfo);
int car_pos(int ts, struct coordinate *xy, sqlite3 *db, struct info *cardinfo);

void init_tty(int fd);
unsigned char CalBCC(unsigned char *buf, int n);
int PiccRequest(int fd);
int PiccAnticoll(int fd);
int getcardid(void);

int beep_init(void);
int beepctl(int fd, int flag);
int beep(void);

#endif
