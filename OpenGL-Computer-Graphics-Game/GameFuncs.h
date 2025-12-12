#pragma once

#include "Bullet.h"
#include <vector>
#include <iostream>


void Shoot(std::vector<Bullet>& Bullets, Vector3 loc, float angle) {
	Bullet b = Bullet(loc, -angle);
	Bullets.push_back(b);
}

void BulletHandling(std::vector<Bullet>& Bullets, float bulletSpeed, int& score, float x, float z) {
	for (int i = 0; i < Bullets.size(); ++i) {

		float distance = sqrt(
			(Bullets[i].pos.x - x) * (Bullets[i].pos.x - x) +
			(Bullets[i].pos.z - z) * (Bullets[i].pos.z - z)
		);
		//Destroy Bullets
		if (distance > 200) {
			Bullets[i] = Bullets.back();
			Bullets.pop_back();
			--i;
			score -= 10;
		}
		else {
			Bullets[i].pos.x -= sin(Bullets[i].angle) * bulletSpeed;
			Bullets[i].pos.z -= cos(Bullets[i].angle) * bulletSpeed;

		}
	}

}
