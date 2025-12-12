#include "Robot.h"
#include <math.h>
#include <cstdlib>
#include "PrimitiveAssembly.h"
#include "Utilities.h"
// ========== Robot Class =============

Robot::Robot() { //default constructor handles default robot structure
	num = 0;
	cHead = Vector3(1, 1, 1);
	cBody = Vector3(1, 0, 0);
	cLArm = cRArm = cLLeg = cRLeg = Vector3(0, 0, 1);
	transform = headrot = bodyrot = larmrot = rarmrot = llegrot = rlegrot = Vector3(0, 0, 0);
	headpos = Vector3(0, 2.75, 0);
	bodypos = Vector3(0, 0, 0);
	larmpos = Vector3(-2, 1, 0);
	rarmpos = Vector3(2, 1, 0);
	llegpos = Vector3(-.5, -3, 0);
	rlegpos = Vector3(.5, -3, 0);
	colliderPos = Vector3(0, 0, 0);
	colliderRadius = 2;
	activeCollider = true;
	targetPos = Vector3(rand() % 100 - 50, 1.5, rand() % 100 - 50);
	moveSpeed = .001;
}

Robot::Robot(Vector3 sendOffset, int i, GLint tex) : Robot() { // offset robot applies a global location to each robot
	transform = sendOffset;
	num = i;
	texIDIX = tex;
	//cout << "Test: "<< num<<" " << origin.x << " " << origin.y << " " << origin.z << endl;

}
Vector3 Robot::GetColliderPos() {
	return colliderPos + transform;
}

//BONUS 1
void Robot::Retarget() {
	targetPos = Vector3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);
}


//BONUS 1
void Robot::Move() {
	transform = transform + (targetPos - transform) * moveSpeed;
	Vector3 tmp = transform - targetPos;
	if (sqrt(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z) < 1) Retarget();
}

void Robot::Draw(int mode, bool bnw, bool showColliders) { //handles rendering of individual robots;
	//Head
	SpawnSphere(mode, texIDIX, 10, 10, headpos + transform, Vector3(.75, .75, .75), headrot, bnw ? toGrayscale(cHead) : cHead);
	//Body
	SpawnCube(mode, texIDIX, bodypos + transform, Vector3(2, 4, 2), bodyrot, bnw ? toGrayscale(cBody) : cBody);
	//Left Arm
	SpawnCube(mode, texIDIX, larmpos + transform, Vector3(2, .5, .5), larmrot, bnw ? toGrayscale(cLArm) : cLArm);
	//Right Arm
	SpawnCube(mode, texIDIX, rarmpos + transform, Vector3(2, .5, .5), rarmrot, bnw ? toGrayscale(cRArm) : cRArm);
	//Left Leg
	SpawnCube(mode, texIDIX, llegpos + transform, Vector3(.5, 2, .5), llegrot, bnw ? toGrayscale(cLLeg) : cLLeg);
	//Right Leg
	SpawnCube(mode, texIDIX, rlegpos + transform, Vector3(.5, 2, .5), rlegrot, bnw ? toGrayscale(cRLeg) : cRLeg);
	//Visual Collider
	if (activeCollider && showColliders) SpawnSphere(1, -1, 10, 10, colliderPos + transform, Vector3(colliderRadius, colliderRadius, colliderRadius), Vector3(), Vector3(1, 1, 1));
	glFlush();

}

// defaut t-pose
void Robot::TPose() {
	headrot = bodyrot = larmrot = rarmrot = llegrot = rlegrot = Vector3(0, 0, 0);

	headpos = Vector3(0, 2.75, 0);
	bodypos = Vector3(0, 0, 0);
	larmpos = Vector3(-2, 1, 0);
	rarmpos = Vector3(2, 1, 0);
	llegpos = Vector3(-.5, -3, 0);
	rlegpos = Vector3(.5, -3, 0);
}

// dance positions to alternate between with random alternate options
void Robot::Dance1() {
	//cout << num << "call " << endl;
	headpos = Vector3(0, 3.25, 0);

	larmpos = Vector3(-2, .5, 0);
	larmrot = Vector3(0, 90, 30);

	rarmpos = Vector3(2, 1, 0);
	rarmrot = Vector3(0, 90, -30);

	llegrot = Vector3(30, 0, 0);

	rlegrot = Vector3(-30, 0, 0);
}

//same as dance 1
void Robot::Dance2() {
	headpos = Vector3(0, 2.75, 0);

	larmpos = Vector3(-2, 1, 0);
	larmrot = Vector3(0, 90, -30);

	rarmpos = Vector3(2, .5, 0);
	rarmrot = Vector3(0, 90, 30);

	llegrot = Vector3(-30, 0, 0);
	rlegrot = Vector3(30, 0, 0);

}

//handles limb color adjustment called from menus
void Robot::changeLimbColor(Vector3 color, int limbSelect) {
	switch (limbSelect) {
	case 1:
		cHead = color;
		break;
	case 2:
		cBody = color;
		break;
	case 3:
		cLArm = color;
		break;
	case 4:
		cRArm = color;
	case 5:
		cLLeg = color;
	case 6:
		cRLeg = color;
	}
}

void Robot::Hit() {
	//cout << "Hit!" << endl;
	activeCollider = false;
	cHead = cBody = cRArm = cLArm = cRLeg = cLLeg = Vector3(0, 0, 0);
}