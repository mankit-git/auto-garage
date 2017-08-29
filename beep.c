#include "head.h"

extern sqlite3 *db;

int beep_init(void)
{
	int beep_fd = open("/dev/beep", O_RDWR);
	if(beep_fd == -1)
	{
		perror("openbeep() failed");
		exit(0);
	}

	return beep_fd;
}

int beepctl(int fd, int flag)
{
	int ret = ioctl(fd, flag, 1);
	if(ret < 0)
	{
		perror("ioctl() failed");
		exit(0);
	}
}

int beep(void)
{
	int beep_fd = beep_init();
	beepctl(beep_fd, 0);
	usleep(100000);
	beepctl(beep_fd, 1);

	close(beep_fd);
	return 0;
}
