#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

long delay = 100;
int score = 0;

// 球
struct Ball {
	int x;
	int y;
	int vx;
	int vy;
	char ch;
} ball = {5, 5, 2, 1, 'o'};

// 板
struct Bar {
	int x;
	int y;
	char* str;
	int len;
} bar = {10, 0, "[########]", 10};

void startGame();

int max(int x, int y) { return x > y ? x : y; }

int min(int x, int y) { return x < y ? x : y; }

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
	mvprintw(LINES / 2 - 2, COLS / 2 - 10, "Game Over!");
	mvprintw(LINES / 2 - 1, COLS / 2 - 10, "score: %d", score);
	mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Press 'q' to quit");
	mvprintw(LINES / 2 + 2, COLS / 2 - 10, "Press 'r' to restart");
	refresh();
}

// 信号处理函数
void update(int n) {
	// 反弹
	if (ball.y == bar.y - 1 && ball.x >= bar.x && ball.x <= bar.x + bar.len) {
		ball.vy = -ball.vy;
		ball.y = bar.y - 1;
		score++;
		beep();
	}

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
	}

	clear();
	mvaddch(ball.y, ball.x, ball.ch);
	mvaddstr(bar.y, bar.x, bar.str);
	refresh();

	// 更新坐标
	ball.x += ball.vx;
	ball.y += ball.vy;

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
				bar.x = min(bar.x + 1, COLS - 1 - bar.len);
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
	score = 0;

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