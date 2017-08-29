#include "head.h"

extern int flag;
extern sqlite3 *db;
extern int ts;
extern time_t t_car_in;
struct coordinate *xy;

char *read_image_from_file(int fd, unsigned char *jpg_buffer, unsigned long image_size)
{
	int nread = 0;
	int size = image_size;
	unsigned char *buf = jpg_buffer;

	// 循环地将jpeg文件中的所有数据，统统读取到jpg_buffer中
	while(size > 0)
	{
		nread = read(fd, buf, size);
		if(nread == -1)
		{
			printf("read jpg failed \n");
			exit(0);
		}
		size -= nread;
		buf +=nread;
	}

	return jpg_buffer;
}

int touchlcd(int ts, struct coordinate *xy)
{
	struct input_event buf;
	bzero(&buf, sizeof(buf));
	int flag1 = 0;
	int flag2 = 0;
	while(1)
	{
		read(ts, &buf, sizeof(buf));
		if(buf.type == EV_ABS)
		{
			if(buf.code == ABS_Y)
			{
				flag2 = 1;
				xy->y = buf.value;
				printf("y: %d\n", xy->y);
			}
			if(buf.code == ABS_X)
			{
				flag1 = 1;
				xy->x = buf.value;
				printf("x: %d\n", xy->x);
			}

		}

		if(buf.type == EV_KEY)
		{
			if(buf.code == BTN_TOUCH && buf.value == 0)
			{
				break;
			}	
		}
	}
}

int judge(int ts, struct coordinate *xy, sqlite3 *db, struct info *cardinfo)
{	
	touchlcd(ts, xy);
	if(xy->x >= 640 && xy->x <= 800)
	{
		if(xy->y >= 0 && xy->y <= 240)
		{
			return 1;
		}
		if(xy->y >= 240 && xy->y <= 480)
		{
			return 2;
		}
	}
	else
		return -1;

}

int car_pos(time_t t_car_in, int ts, struct coordinate *xy, sqlite3 *db, struct info *cardinfo)
{
	while(1)
	{	
		int ret = judge(ts, xy, db, cardinfo);
		if(ret == 1)
		{
			printf("touch OK!\n");
			printf("welcome to the park!\n");
			printf("please get car ID...\n");
			cardid = getcardid();
			flag = 0;
			beep();
			sqlite(db, cardinfo);
		}
		if(ret == 2)
		{
			printf("touch OK!\n");
			printf("out the park!\n");
			printf("please get car ID...\n");
			cardid = getcardid();
			flag = 1;
			beep();
			out_table(t_car_in, db, cardinfo);
		}	
		if(ret == -1)
		{
			printf("wrong area!\n");
			exit(0);
		}
	}
}
