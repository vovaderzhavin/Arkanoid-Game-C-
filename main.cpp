#include "glut.h"
#include "Ball.h"
#include "vars.h"
#include "Brick.h"
#include <math.h>
#include <ctime>


const int size_window = 800;
const int tick = 1; //скорость таймера
const int ball_radius = 8;
const int brick_rows = 7, brick_col = 16; //количество блоков
const int ball_speed = 7;
int k = brick_rows * brick_col;
char g_o[]{ 'G', 'A', 'M', 'E', ' ', ' ', 'O', 'V', 'E', 'R', '!',};
char g_win[]{ 'Y', 'O', 'U', ' ', ' ', 'W', 'I', 'N', '!', };
char g_xp[]{ 'X', 'P', ' ', '=', ' ', ' ',};

//Координаты ракетки и ёё размер
float r_x = 50.0, r_y = float(size_window - 5), r_w = 200, r_h = 30.0;

Ball ball(r_y, ball_radius);
Brick brick[brick_rows][brick_col];

bool isCollision(Ball& b, Brick& br) {
	float x = b.x, y = b.y, r = b.r, c = br.col, w = br.w, row = br.row, h = br.h;
	if (br.active) {
		//проверка на пересечение границ блока и шарика
		if (fabs(x - c * w - w / 2 - 2) <= r + w / 2 && fabs(y - row * h - h / 2 - 2) <= r + h / 2) {
			if (sqrt((c * w + w / 2 - x) * (c * w + w / 2 - x) + (row * h + h / 2 - y) * (row * h + h / 2 - y)) - (w / 2 - 1) * sqrt(2.0) - r > r * (sqrt(2.0) - 1)) {
				return 0;
			}
			else {
				return 1;
			}
		}
	}
	return 0;
}


void win() {
	glColor4f(0.08, 0.15, 0.26, 1.0);
	glRectf(0, 0, 800, 800);

	glColor4f(1.0, 0.84, 0.0, 1.0);
	glRectf(250, 600, 550, 650);
	glRectf(360, 500, 440, 600);
	glRectf(200, 275, 600, 500);
	glRectf(125, 325, 675, 350);
	glRectf(125, 425, 675, 450);
	glRectf(125, 350, 150, 450);
	glRectf(650, 350, 675, 450);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(350, 200);  // move in 10 pixels from the left and bottom edges
	for (int i = 0; i < 9; ++i) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, g_win[i]);
	}
	glPopMatrix();
}


void loss() {
	glColor4f(0.08, 0.15, 0.26, 1.0);
	glRectf(0, 0, 800, 800);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(320, 400);  // move in 10 pixels from the left and bottom edges
	for (int i = 0; i < 11; ++i) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, g_o[i]);
	}
	glPopMatrix();
}


void hp() {
	glColor4f(1.0, 1.0, 1.0, 1.0);
	if (ball.hp == 3) {
		g_xp[5] = '3';
	}
	if (ball.hp == 2) {
		g_xp[5] = '2';
	}
	if (ball.hp == 1) {
		g_xp[5] = '1';
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(0, 750);  // move in 10 pixels from the left and bottom edges
	for (int i = 0; i < 6; ++i) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, g_xp[i]);
	}
	glPopMatrix();
}


void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0.0, 1.0, 1.0, 0.2);
	glRectf(r_x, r_y, r_x + r_w, r_y + r_h); //рисует ракетку

	glColor3f(1.0, 1.0, 1.0); //ball color
	glBegin(GL_POLYGON); //рисует шарик
	for (float i = 0; i < 2 * 3.14; i += 3.14 / 4) {
		glVertex2f(ball.x + ball.r * sin(i), ball.y + ball.r * cos(i));
	}
	glEnd();
	if (ball.hp == 0 || ball.fail == true) {
		loss();
		ball.active = false;
		ball.fail = true;
	}
	else if (k == 0 || ball.win == true) {
		win();
		ball.active = false;
		ball.win = true;
	}
	else {
		hp();
		glBegin(GL_QUADS);
		for (int i = 0; i < brick_rows; i++) {
			for (int j = 0; j < brick_col; j++) {
				glColor4f(0.16, 0.95, 0.25, 1.0);
				if (brick[i][j].hit == 3) {
					glColor4f(0.84, 0.16, 0.16, 1.0);
				}
				if (brick[i][j].hit == 2) {
					glColor4f(1.0, 1.0, 0.0, 1.0);
				}
				Brick& b = brick[i][j];
				if (b.active) {
					glVertex2f(b.col * b.w + 1, b.row * b.h + 1);
					glVertex2f(b.col * b.w + b.w - 1, b.row * b.h + 1);
					glVertex2f(b.col * b.w + b.w - 1, b.row * b.h + b.h - 1);
					glVertex2f(b.col * b.w + 1, b.row * b.h + b.h - 1);
				}
			}
		}
		glEnd();
	}
	glutSwapBuffers();
}

void Timer(int) {
	if (ball.active) {
		ball.move();
		for (int i = 0; i < brick_rows; i++) {
			for (int j = 0; j < brick_col; j++) {
				Brick& b = brick[i][j];
				if (isCollision(ball, b)) {
					if (fabs(ball.x - b.col * b.w - b.w / 2) < fabs(ball.y - b.row * b.h - b.h / 2)) { //проверка на низ или верх для шарика
						ball.dy *= -1;
					}
					else if (fabs(ball.x - b.col * b.w - b.w / 2) > fabs(ball.y - b.row * b.h - b.h / 2)) { //справа или слева
						ball.dx *= -1;
					}
					else {
						if (ball.dx > 0) {
							if (ball.x < b.col * b.w + 1) ball.dx *= -1;
						}
						else if (ball.x > (b.col + 1) * b.w - 1) ball.dx *= -1;
						if (ball.dy > 0) {
							if (ball.y < b.row * b.h + 1) ball.dy *= -1;
						}
						else if (ball.y > (b.row + 1) * b.h - 1) ball.dy *= -1;
					}
					if (--b.hit == 0) {
						b.active = false;
						--k;
					}
					goto o;
				}
			}
		}
	}

o:	Draw();
	glutTimerFunc(tick, Timer, 0);
}

void MousePress(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && !ball.active) {
		ball.active = true;
		//скорость шарика
		ball.dx = ball_speed + 1; 
		ball.dy = -ball_speed;
	}
}

void Mouse(int ax, int ay) {
	r_x = ax - size_window / 2 + r_w * 1.5;
	if (!ball.active) {
		//положение шарика на ракетке
		ball.x = r_x + r_w / 2;
		ball.y = r_y - ball.r;
	}
}

void Init() {
	glClearColor(0.08, 0.15, 0.26, 1.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, size_window, size_window, 0, 1, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(size_window, size_window);
	glutInitWindowPosition(950, 400);
	glutCreateWindow("Arcanoid");
	glutDisplayFunc(Draw);
	glutTimerFunc(tick, Timer, 0);
	glutPassiveMotionFunc(Mouse);
	glutMouseFunc(MousePress);
	Init();
	for (int i = 0; i < brick_rows; i++) {
		for (int j = 0; j < brick_col; j++) {
			brick[i][j].active = true;
			if (i % 3 == 0) {
				brick[i][j].hit = 3;
			}
			brick[i][j].col = j;
			brick[i][j].row = i;
		}
	}
	glutMainLoop();
	return 0;
}
