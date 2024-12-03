#include <curses.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

long delay = 200000;
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
	int vx;
	char* str;
	int len;
} bar = {10, 0, 2, "[########]", 10};

void startGame();

int max(int x, int y) { return x > y ? x : y; }

int min(int x, int y) { return x < y ? x : y; }

// 游戏结束画面
void gameOverView() {
	clear();
	mvprintw(LINES / 2 - 2, COLS / 2 - 10, "Game Over!");
	mvprintw(LINES / 2 - 1, COLS / 2 - 10, "score: %d", score);
	mvprintw(LINES / 2 + 1, COLS / 2 - 10, "Press 'q' to quit");
	mvprintw(LINES / 2 + 2, COLS / 2 - 10, "Press 'r' to restart");
	refresh();
}

// 游戏
void* game(void* arg) {
	while (1) {
		// 反弹
		if (ball.y == bar.y - 1 && ball.x >= bar.x &&
			ball.x <= bar.x + bar.len) {
			ball.vy = -ball.vy;
			ball.y = bar.y - 1;
			score++;
			beep();
		}

		// 变换方向
		if (ball.x >= COLS) {
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
			break;
		}
		usleep(delay);
	}
	gameOverView();
}

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
				ball.vx = max(ball.vx / 2, 1);
				ball.vy = max(ball.vy / 2, 1);
				break;
			}
			case 'h': {
				bar.x = max(bar.x - bar.vx, 0);
				break;
			}
			case 'l': {
				bar.x = min(bar.x + bar.vx, COLS - 1 - bar.len);
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

	pthread_t threadGame;
	pthread_create(&threadGame, NULL, game, NULL);

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