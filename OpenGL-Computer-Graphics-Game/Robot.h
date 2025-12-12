#pragma once

#include "Vector.h"
#include <GL/glut.h>
#include <GL/freeglut.h>


// ========== Robot Class =============
class Robot {
public:
	int num;
	GLint texIDIX;
	float colliderRadius, moveSpeed;
	bool activeCollider;
	Vector3 transform, //global offset
		cHead, cBody, cRArm, cLArm, cRLeg, cLLeg, //colors
		headpos, bodypos, larmpos, rarmpos, llegpos, rlegpos, //body positions
		headrot, bodyrot, larmrot, rarmrot, llegrot, rlegrot, //body rotations
		colliderPos, targetPos;
	Robot(); 
	Robot(Vector3 sendOffset, int i, GLint tex);
	Vector3 GetColliderPos();
	void Retarget();
	void Move();
	void Draw(int mode, bool bnw, bool showColliders);
	void TPose();
	void Dance1();
	void Dance2();
	void changeLimbColor(Vector3 color, int limbSelect);
	void Hit();
};