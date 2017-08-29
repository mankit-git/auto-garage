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
				xy->y = buf.value;
			}
			if(buf.code == ABS_X)
			{
				xy->x = buf.value;
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

int car_pos(int ts, struct coordinate *xy, sqlite3 *db, struct info *cardinfo)
{
	while(1)
	{	
		int ret = judge(ts, xy, db, cardinfo);
		unsigned int in_car;
		unsigned int out_car;
		if(ret == 1)
		{
			printf("touch OK!\n");
			printf("welcome to the park!\n");
			printf("please get car ID...\n");
			cardid = getcardid();
			flag = 0;
			beep();
			in_car = sqlite(db, cardinfo);
		}
		if(ret == 2)
		{
			printf("touch OK!\n");
			printf("out the park!\n");
			printf("please get car ID...\n");
			cardid = getcardid();
			flag = 1;
			beep();
			out_car = out_table(db, cardinfo);
			int total = ((out_car - in_car)/3600);
			int money = 0;
			if(total < 1)
			{
				money = (total+1) * 5;
			}
			else
				money = total * 5;
			printf("你消费%d元\n", money);
			printf("goodbye!have a good day!\n");
		}	
		if(ret == -1)
		{
			printf("wrong area!\n");
			exit(0);
		}
	}
}
