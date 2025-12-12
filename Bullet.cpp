#include "Bullet.h"
// ========== Bullet Class =============
Bullet::Bullet(Vector3 Position, float Angle) {
	pos = Position;
	angle = Angle;
	id = nextid;
	nextid++;
	colliderRadius = .05;
}

void Bullet::Draw(int mode) {
	SpawnSphere(mode, texIDIX, 10, 10, pos, Vector3(.05, .05, .05), Vector3(), Vector3(1, 1, 0));
	glFlush();
}

bool Bullet::operator==(const Bullet& in) const {
	if (in.id == id) {
		return true;
	}
	else {
		return false;
	}
} 


