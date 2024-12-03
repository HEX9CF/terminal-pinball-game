#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include <algorithm>
#include <string>
using namespace std;

struct Ball {
	int x = 10;
	int y = 10;
	int dx = 1;
	int dy = 1;
	int vx = 2;
	int vy = 1;
	char str = 'O';
} ball;

struct Bar {
	int x = 10;
	int y = 0;
	string str = "__________";
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
	mvaddstr(bar.y, bar.x, bar.str.c_str());
	refresh();

	// 更新坐标
	ball.x += ball.dx * ball.vx;
	ball.y += ball.dy * ball.vy;

	// 变换方向
	if (ball.x == COLS) {
		ball.dx = -1;
		ball.x = COLS - 1;
		beep();
	} else if (ball.x < 0) {
		ball.dx = 1;
		ball.x = 0;
		beep();
	}
	if (ball.y == LINES) {
		ball.dy = -1;
		ball.y = LINES - 1;
		beep();
	} else if (ball.y < 0) {
		ball.dy = 1;
		ball.y = 0;
		beep();
	}
}

int main(int argc, char* argv[]) {
	// 初始化
	chtype input;
	long delay = 100;
	initscr();
	crmode();
	noecho();
	bar.y = LINES - 1;

	// 设置定时器
	signal(SIGALRM, paint);
	setTicker(delay);
	while ((input = getch()) && input != ERR && input != 'q') {
		switch (input) {
			case 'j': {
				delay /= 2;
				setTicker(delay);
				break;
			}
			case 'k': {
				delay *= 2;
				setTicker(delay);
				break;
			}
			case 'h': {
				bar.x = max(bar.x - 1, 0);
				break;
			}
			case 'l': {
				bar.x = min(bar.x + 1, COLS - 1 - bar.length);
				break;
			}
		}
	}

	endwin();
	return 0;
}