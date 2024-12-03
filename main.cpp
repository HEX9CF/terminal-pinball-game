#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include <algorithm>
#include <string>
using namespace std;

long delay = 100;

struct Ball {
	int x;
	int y;
	int vx;
	int vy;
	char str = 'O';
} ball;

struct Bar {
	int x;
	int y;
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
	ball.x += ball.vx;
	ball.y += ball.vy;

	// 变换方向
	if (ball.x == COLS) {
		ball.vx = -ball.vx;
		ball.x = COLS - 1;
		beep();
	} else if (ball.x < 0) {
		ball.vx = -ball.vx;
		ball.x = 0;
		beep();
	}
	if (ball.y < 0) {
		ball.vy = -ball.vy;
		ball.y = 0;
		beep();
	} else if (ball.y == bar.y - 1 && ball.x >= bar.x &&
			   ball.x <= bar.x + bar.length) {
		ball.vy = -ball.vy;
		ball.y = bar.y - 1;
		beep();
	}

	// game over
	if (ball.y >= bar.y) {
		setTicker(0);
		clear();
		mvprintw(LINES / 2 - 1, COLS / 2 - 10, "Game Over!");
		mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Press 'q' to quit");
		mvprintw(LINES / 2 + 2, COLS / 2 - 10, "Press 'r' to restart");
		refresh();
	}
}

void init() {
	ball.x = 5;
	ball.y = 5;
	ball.vx = 2;
	ball.vy = 1;
	bar.x = 10;
	bar.y = LINES - 1;

	// 设置定时器
	signal(SIGALRM, paint);
	setTicker(delay);
}

int main(int argc, char* argv[]) {
	chtype input;

	// 初始化
	initscr();
	crmode();
	noecho();

	init();
	while ((input = getch()) && input != ERR && input != 'q') {
		switch (input) {
			case 'j': {
				ball.vx *= 2;
				ball.vy *= 2;
				break;
			}
			case 'k': {
				ball.vx /= 2;
				ball.vy /= 2;
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
			case 'r': {
				init();
				break;
			}
		}
	}

	endwin();
	return 0;
}