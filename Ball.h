#pragma once

class Ball
{
public:
	float x, y, dx, dy, r; //������� ������ � ��� ��������
	unsigned int hp;
	bool active, win, fail;
	Ball(float y_, float r_) : x(), y(y_), r(r_), dx(), dy(), active(false), win(false), fail(false), hp(3) {}
	void move();
};