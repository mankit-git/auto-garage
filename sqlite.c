#include "head.h"

struct info *cardinfo;
sqlite3 *db;

int create_table(sqlite3 *db)
{
	char *errmsg = NULL;
	char *sql = "create table if not exists card \
		     (                \
		      	id INTEGER,    \
			name TEXT,     \
			carnum TEXT   \
		     );"; 
	int ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if(ret)
	{
		printf("errmsg: %s\n", errmsg);
		perror("sqlite3_create_table() failed");
		exit(0);
	}
}

int sqlite3_insert(sqlite3 *db, struct info *cardinfo)
{
	char *errmsg = NULL;
	time_t t_car_in;
	time(&t_car_in);
	cardinfo->id = cardid;
	srand((unsigned)time(NULL));
	sprintf(cardinfo->carnum, "粤%c%d", (rand()%1000000)%24+'A', rand()%100000);
	printf("车牌号：%s\n", cardinfo->carnum);
	cardinfo->time = ctime(&t_car_in);
	printf("当前时间是：%s\n", cardinfo->time);

	char insert[256] = {0};
	sprintf(insert, "insert into card values(%d, '%s', '%s');", cardinfo->id, cardinfo->carnum, cardinfo->time);
	int ret = sqlite3_exec(db, insert, NULL, NULL, &errmsg);
	if(ret)
	{
		perror("sqlite3_insert() failed");
		exit(0);
	}
	
	return t_car_in;
}

int callback(void *data, int n, char **cloumn_value, char **cloumn_name)
{
	printf("%s\n", (char *)data);

	int i;
	for(i=0; i<n-1; i++)
	{
		printf("%s:%s\n", cloumn_name[i], cloumn_value[i]);
	}

	printf("callback test!\n");
	printf("======================================\n");
	return 0;
}

int sqlite3_del(sqlite3 *db, struct info *cardinfo)
{
	char *errmsg = NULL;
	char insert[256] = {0};
	char *data = "sqlite callback\n";
	
	sprintf(insert, "delete from card where id=%d;select * from card;", cardinfo->id);
	int ret = sqlite3_exec(db, insert, callback, (void *)data, &errmsg);
	if(ret)
	{
		perror("sqlite3_del() failed");
		exit(0);
	}
	
	return 0;
}

int display(sqlite3 *db)
{
	if(db == NULL)
		return 0;

	char *errmsg = NULL;
	char *check = "select * from card;";
	char **result = NULL;
	int nrow, ncolumn;
	
	int show = sqlite3_get_table(db, check, &result, &nrow, &ncolumn, &errmsg);
	if(show)
	{
		perror("sqlite3_get_table() failed");
		exit(0);
	}

	int i, j;
	for(i=0; i<=nrow; i++)
	{
		for(j=0; j<ncolumn; j++)
		{
			printf("%s |", result[i*ncolumn+j]);
		}
		printf("\n");
	}
	sqlite3_free_table(result);
}

int sqlite(sqlite3 *db, struct info *cardinfo)
{
	int ret = sqlite3_open("cardinfo.db", &db);
	if(ret < 0)
	{
		perror("sqlite3_open() failed");
		exit(0);
	}
	create_table(db);
	sqlite3_insert(db, cardinfo);
	display(db);
}

int out_table(time_t t_car_in, sqlite3 *db, struct info *cardinfo)
{
	int ret = sqlite3_open("cardinfo.db", &db);
	if(ret < 0)
	{
		perror("sqlite3_open() failed");
		exit(0);
	}
	sqlite3_del(db, cardinfo);
	/*time_t t_car_out;
	int money = ((t_car_out - t_car_in)/3600)*5;
	printf("你消费%d元\n", money);
	printf("goodbye!have a good day!\n");*/
	display(db);
}
