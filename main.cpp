#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b

struct Ball {
	int x = 10;
	int y = 10;
	int dir = 1;
	char str = 'O';
} ball;

struct Bar {
	int x = 10;
	int y = 0;
	char* str = "__________";
	int length = 10;
} bar;

// 设置定时器
int setTicker(long n_msecs) {
	struct itimerval new_timeset;
	long n_sec, n_usecs;
	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;
	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;
	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

// 信号处理函数
void paint(int n) {
	clear();
	mvaddch(ball.y, ball.x, ball.str);
	refresh();

	// 更新坐标
	ball.x += ball.dir;

	// 变换方向
	if (ball.x == COLS) {
		ball.dir = -1;
		ball.x = COLS - 1;
		beep();
	} else if (ball.x < 0) {
		ball.dir = 1;
		ball.x = 0;
		beep();
	}
}

int main(int argc, char* argv[]) {
	chtype input;
	long delay = 100;

	// 初始化
	initscr();
	crmode();
	noecho();

	// 设置定时器
	signal(SIGALRM, paint);
	setTicker(delay);
	while ((input = getch()) && input != ERR && input != 'q') {
		switch (input) {
			case 'f': {
				delay /= 2;
				setTicker(delay);
				break;
			}
			case 's': {
				delay *= 2;
				setTicker(delay);
				break;
			}
		}
	}

	endwin();
	return 0;
}