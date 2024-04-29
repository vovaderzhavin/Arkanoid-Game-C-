#include "Ball.h"
#include "vars.h"
int windows_size = 800;

void Ball::move() {
	x += dx;
	y += dy;
	//проверка близости шарика к стенкам и ракетке
	if (x - r <= 0) {
		x = r;
		dx *= -1;
	}
	else if (y - r <= 0) {
		y = r;
		dy *= -1;
	}
	else if (x + r >= windows_size) {
		x = windows_size - r;
		dx *= -1;
	}
	else if (y + r >= r_y && y + r <= r_y + r_h && x > r_x && x < r_x + r_w) {
		if (dx > 0) {
			if (x >= r_x && x <= r_x + r_w / 2) {
				y = r_y - r;
				dy *= -1;
				dx *= -1;
			}
			else {
				y = r_y - r;
				dy *= -1;
			}
		}
		else if (dx < 0) {
			if (x >= r_x && x <= r_x + r_w / 2) {
				y = r_y - r;
				dy *= -1;
			}
			else {
				y = r_y - r;
				dy *= -1;
				dx *= -1;
			}
		}
	}
	else if (y > windows_size) {
		active = false;
		--hp;
	}
}