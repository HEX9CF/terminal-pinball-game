#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <algorithm>
#include <string>
using namespace std;

long delay = 100;

// 球
struct Ball {
	int x;
	int y;
	int vx;
	int vy;
	char str = 'O';
} ball;

// 板
struct Bar {
	int x;
	int y;
	string str = "[########]";
	int length = 10;
} bar;

void startGame();

// 设置定时器
int setTimer(long n_msecs) {
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

// 游戏结束画面
void gameOverView() {
	clear();
	mvprintw(LINES / 2 - 1, COLS / 2 - 10, "Game Over!");
	mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Press 'q' to quit");
	mvprintw(LINES / 2 + 2, COLS / 2 - 10, "Press 'r' to restart");
	refresh();
}

// 信号处理函数
void update(int n) {
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
		setTimer(0);
		gameOverView();
	}
}

// 游戏
void keyboardHandler() {
	chtype input;

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
				startGame();
				break;
			}
		}
	}
}

// 开始游戏
void startGame() {
	// 初始化
	ball.x = 5;
	ball.y = 5;
	ball.vx = 2;
	ball.vy = 1;
	bar.x = 10;
	bar.y = LINES - 1;

	// 设置定时器
	signal(SIGALRM, update);
	setTimer(delay);

	keyboardHandler();
}

// 开始画面
void startView() {
	chtype input;

	clear();
	mvprintw(LINES / 2 - 1, COLS / 2 - 10, "Pinball Game");
	mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Press 'q' to quit");
	mvprintw(LINES / 2 + 2, COLS / 2 - 10, "Press 's' to start");
	refresh();
	while ((input = getch()) && input != ERR && input != 'q') {
		if (input == 's') {
			startGame();
			break;
		}
	}
}

int main(int argc, char* argv[]) {
	// 初始化
	initscr();
	crmode();
	noecho();

	startView();

	endwin();
	return 0;
}