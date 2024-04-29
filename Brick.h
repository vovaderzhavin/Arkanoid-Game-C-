#pragma once

class Brick
{
public:
	bool active, win;
	short row, col, hit;
	float w, h; //длина и высота блока
	Brick(short r_, short c_, float w_, float h_, short hit_) : row(r_), col(c_), hit(hit_), w(w_), h(h_), active(false), win(false) {};
	Brick() : row(), col(), w(50), h(50), hit(1), active(false), win(false) {};
};