#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h> 
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <IrrKlang/irrKlang.h>

//========== My Includes ===============
#include "Robot.h"
#include "PrimitiveAssembly.h"
#include "GameFuncs.h"
#include "Bullet.h"
#include "Rendering.h"
#include "Utilities.h"
#include "Vector.h"
#include "Collisions.h"


using namespace irrklang;
using namespace std;

#define WINDOW_W 640
#define WINDOW_H 480

#define GL_CLAMP_TO_EDGE 0x812F // OpenGL texture mapping parameter

// ========== Engine Vars =============
bool bGouraud = true;
bool dLighting = false;
bool bObj = false;
bool bnw = false;
bool solid = false;
bool vert = false;
bool axis = true;
bool clear = false;
bool showColliders = false;
bool dance = false;
bool mute = false;
bool perspective = true;
bool pose = false;
bool back = true;
bool top = true;
bool swapCam = false;
bool individual = false;
int spawnNum = 10;
int mode = 3;
int rotx;
int roty;
int rotz;
int rotAngle = 30;
int limbSelect = -1;
int spin = 0;
int frame = 0;
bool showPause = false;
int menuSelection = 0;
// windowing
int windowW = 0;
int windowH = 0;
int windowPosx = 50;
int windowPosy = 50;
static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;
GLint leftMouseButton, rightMouseButton; //status of the mouse buttons
int mouseX = 0, mouseY = 0; //last known X and Y of the mouse
float cameraTheta, cameraPhi, cameraRadius; //camera position in spherical coordinates
float arcx, arcy, arcz; //camera world location
float lx = 0.0f, lz = -1.0f; // actual vector representing the camera's direction
float x = 0.0f, y = 0.0f, z = 5.0f; // XZ position of the camera
float angle = 0.0f; // angle for rotating triangle
float zoom = .2; //Normal Camera
float fov = 90.0; //ESV camera fov
bool fullScreenMode = false;

// ========== Game Vars =============
//robot jumping
bool jumping;
int jumper;
int jump = 0;
float gravity = 0.6;
int jumpTrigLim = 200, jumpTrig;
float jumpTimer = 10, resetJumpTimer;
float jumpAcceleration = 0.5f; // Adjust this value for jump height
float jumpVelocity = 2.0f; // Adjust this value for initial jump velocity
//Game
float gameTime;
int kills = 0;
int score = 0;
bool gameOver = false;
string gameState = "";
//Bullet Speed
int bulletSpeedMode = 0;
float bulletSpeed = .5;

// ========== Texture Vars =============
GLuint texID[3];

char* textureFileNames[3] = {
	(char*)"images/dirt.jpg",
	(char*)"images/metal.jpg",
	(char*)"images/space.jpg"
};
int texWidth, texHeight, texChannels;


// ========== Model Vars =============
Assimp::Importer importer;     // persistent importer
const aiScene* scene = nullptr;
static std::string modelpath = "fbx/model.obj";
// images / texture
std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
GLuint* textureIds;							// pointer to texture Array


//BONUS 2
// ========== Sound Vars =============
ISoundEngine* SoundEngine = createIrrKlangDevice();

// ========== Bullet Initializers =============
int Bullet::nextid = 0;
vector<Robot> Robots, Dead;
vector<Bullet> Bullets;

// ========== Initialization =============
void init() {


	//Object Loading Initialization
	scene = importer.ReadFile("fbx/model.obj",
		aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene) {
		std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
	}
	std::cout << "Model loaded successfully!" << std::endl;

	

	// ========== Lighting =============
	//Directional Light
	GLfloat dir_light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat dir_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat dir_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat dir_light_position[] = { -1.0f, -1.0f, -1.0f, 0.0f }; 
	glLightfv(GL_LIGHT0, GL_AMBIENT, dir_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dir_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dir_light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, dir_light_position);

	//Point Light
	GLfloat point_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat point_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat point_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Position in world coordinates
	GLfloat point_light_position[] = { 0.0f, 5.0f, 0.0f, 1.0f }; 

	// Spotlight direction
	GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f }; // pointing down

	glLightfv(GL_LIGHT1, GL_AMBIENT, point_light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, point_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, point_light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, point_light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f); // narrow cone (0–90)

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	

	loadTextures(texID, textureFileNames, texWidth, texHeight, texChannels);

	// ========== Game state inititalization =============
	gameTime = 30;
	kills = 0;
	score = 0;
	gameOver = false;
	gameState = "Playing";

	angle = 0;
	x = z = 0;
	lx = 0.0f;
	lz = -1.0f;

	//arcball values 4.58611 6.72549 10.644 13.5 6.69001 1.045
	arcx = 4.58611;
	arcy = 6.72549;
	arcz = 10.644;
	cameraRadius = 13.5;
	cameraTheta = 6.69001;
	cameraPhi = 1.045;
	recomputeOrientation(arcx, arcy, arcy, cameraRadius, cameraTheta, cameraPhi);

	resetJumpTimer = jumpTimer;
	jumpTrig = 100+rand() % jumpTrigLim;

	//BONUS 2
	// ========== Sound initialization =============
	SoundEngine->stopAllSounds();
	if (!mute) {
		irrklang::ISound* music = SoundEngine->play2D("audio/music.mp3", true, false, true);
		if (music) music->setVolume(0.5f);
	}
	

	srand(time(NULL));
	
	// ========== Display Functions =============
	printf("Instruction: Enter a key\n");
	printf("w: Wireframe mode\n");
	printf("s: solid mode\n");
	printf("q: texture mode\n");
	printf("a: display axis\n");
	printf("c: display colliders\n");
	printf("m: move mode\n");
	printf("o: Toggle OBJ rendering\n");
	printf("l: Toggle Point/Directional light\n");
	printf("ESC: Pause Program\n");
	printf("F1: Toggle full screen\n");
	printf("F2: Swap FPV with ESV\n");
	printf("F3: Toggle Game Sound\n");
	printf("F4: Toggle Flat/Gouraud shading\n");
	printf("Up: Move Camera forward\n");
	printf("Down: Move the camera backwards\n");
	printf("Left: Rotate the camera left\n");
	printf("Right: Rotate the camera right\n");
	printf("Left click: (ESV Mode) Control Arcball angles\n");
	printf("Scroll Wheel: (ESV Mode) Control Arcball radius/zoom\n\n");
	printf("=============Rules======================\n");
	printf("Kill = +10 Score\n");
	printf("Miss = -10 Score\n");


	//clear dead robots
	Dead.clear();
	Robots.clear();

	//spawn all robots
	for (int i = 0; i < spawnNum; ++i) {
		Robot r = Robot(Vector3(((rand() % 50) - 25), 1.5, -(rand() % 50)-10), i, texID[1]);
		Robots.emplace_back(r);
	}
}

void MyDisplay(); //forward decl

// ========== Menu =============
	// Menu item values
enum MENU_TYPE {
	MENU_EXIT,
	MENU_RESUME,
	MENU_NEWGAME
};

void menuNewGame() {
	showPause = false;
	init();
}

void menuResume() {
	showPause = false;
}

void menuExit() {
	exit(0);
}

void drawMenuOverlay() {
	glDisable(GL_LIGHTING);
	glViewport(0, 0, windowW, windowH);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	// -------------------------------------

	const char* items[3] = { "RESUME", "NEW GAME", "EXIT" };

	for (int i = 0; i < 3; i++) {
		float y = 0.6f - i * 0.1f;

		if (menuSelection == i) {
			string selected = "-" + string(items[i]) + "-";
			drawText(0.45f, y, selected.c_str());
		}
		else drawText(0.45f, y, items[i]);

		
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void menu(int item) {
	switch (item) {
	case MENU_RESUME:
		init();
		//bPause = false;
		break;
	case MENU_NEWGAME:
		init();
		break;
	case MENU_EXIT:
		exit(0); // Exit the application
		break;
	}
	glutPostRedisplay(); // Request a redraw if needed
}

// ========== Old Menu =============
// Function to create the menu
void createMenu() {

	int main_menu = glutCreateMenu(menu);
	glutAddMenuEntry("Resume", MENU_RESUME);
	glutAddMenuEntry("New Game", MENU_NEWGAME);
	glutAddMenuEntry("Exit", MENU_EXIT);

	// Attach the menu to the right mouse button
}

// ========== Axis =============
// axis function
void MyAxis() {
	glPushMatrix();
	glBegin(GL_LINES);
	//Y
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, .5, 0);
	//X
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(.5, 0, 0);
	//Z
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.5);
	glEnd();

	//Y arrow
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0.02, .5, 0);
	glVertex3f(0, .52, 0);
	glVertex3f(-.02, .5,0);
	glEnd();

	//X arrow
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(.5, .02,0);
	glVertex3f(.52, 0,0);
	glVertex3f(.5, -.02,0);
	glEnd();

	//Z arrow
	glColor3f(0, 0, 1);
	glBegin(GL_POLYGON);
	glVertex3f(0, .02, 0.5);
	glVertex3f(0, 0, 0.52);
	glVertex3f(0, -.02, 0.5);
	glEnd();
	glPopMatrix();
	glFlush();
}

// ========== Rendering =============



// Forward Declarations

void checkCollisions();

// ========== GLUT FUNCTIONS =============
//##From Powerpoint slides
void mouseCallback(int button, int state, int thisX, int thisY)
{
	//update the left and right mouse button states, if applicable
	if (button == GLUT_LEFT_BUTTON)
		leftMouseButton = state;
	else if (button == GLUT_RIGHT_BUTTON)
		rightMouseButton = state;
	//and update the last seen X and Y coordinates of the mouse
	mouseX = thisX;
	mouseY = thisY;

	if (swapCam) {
		//Middle scroll wheel 
		if (button == 3)
		{

			if (state == GLUT_UP)
			{
				rightMouseButton = state;
				//if (fov > 1) fov -= 1;
				if (cameraRadius > 2.0)
					cameraRadius -= .1;
				recomputeOrientation(arcx, arcy, arcz, cameraRadius, cameraTheta, cameraPhi);
				


			}
		}

		if (button == 4)
		{
			if (state == GLUT_UP)
			{

				rightMouseButton = state;
				//if (fov < 179) fov += 1;
				if (cameraRadius < 20.0)
					cameraRadius += .1;
				recomputeOrientation(arcx, arcy, arcz, cameraRadius, cameraTheta, cameraPhi);

			}
		}
	}

	// ========== Pause Menu Handling =============
	if (!showPause) return;

	int H = glutGet(GLUT_WINDOW_HEIGHT);

	float ny = (float)(H - thisY) / H; // convert to 0–1
	if (ny > 0.55f && ny < 0.65f) menuResume();  // RESUME
	else if (ny > 0.45f && ny < 0.55f) menuNewGame(); // NEWGAME
	else if (ny > 0.35f && ny < 0.45f) menuExit(); // EXIT
	glutPostRedisplay();
}
//##From Powerpoint slides
void mouseMotion(int x, int y)
{
	if (leftMouseButton == GLUT_DOWN && swapCam)
	{
		cameraTheta += (mouseX - x) * 0.005;
		cameraPhi += (mouseY - y) * 0.005;
		// make sure that phi stays within the range (0, PI)
		if (cameraPhi <= 0)
			cameraPhi = 0 + 0.001;
		if (cameraPhi >= PI)
			cameraPhi = PI - 0.001;
		recomputeOrientation(arcx, arcy, arcz, cameraRadius, cameraTheta, cameraPhi); //update camera (x,y,z)
	}

	else if (rightMouseButton == GLUT_DOWN) {
		
		/*double totalChangeSq = (x - mouseX) + (y - mouseY);
		cameraRadius += totalChangeSq * 0.01;
		//limit the camera radius to some reasonable values so the user can't get lost
			if (cameraRadius < 2.0)
				cameraRadius = 2.0;
		if (cameraRadius > 20.0)
			cameraRadius = 20.0;
		recomputeOrientation(); //update camera (x,y,z) based on(radius, theta, phi)
		*/
	}
	mouseX = x;
	mouseY = y;
}

//handles all keyboard functions
void Keyboard(unsigned char key, int xx, int yy) {
	switch (key)
	{
		if (!gameOver) {
			case 'p': // Display vertex model
				mode = 0;
				break;
			case 'w': // wireframe
				mode = 1;
				break;
			case 's': // solid
				mode = 2;
				break;
			case 'q': //texture
				mode = 3;
				break;

			case 'o':
				bObj = !bObj;
				break;
			case 'l':
				dLighting = !dLighting;
				if (dLighting) {
					glEnable(GL_LIGHT0);
					glDisable(GL_LIGHT1);
				}
				else {
					glEnable(GL_LIGHT1);
					glDisable(GL_LIGHT0);
				}
				break;
			case 'a': // axis
				axis = !axis;
				break;
			case 'b': //bullet speed mode
				//cout << "hit " << endl;
				bulletSpeedMode++;
				if (bulletSpeedMode > 2) {
					bulletSpeedMode = 0;
				}
				switch (bulletSpeedMode)
				{
				case 0:
					bulletSpeed = .003;
					break;
				case 1:
					bulletSpeed = 1;
					break;
				case 2:
					bulletSpeed = 3;
					break;
				}
				break;
			case 'c': // clear
				showColliders = !showColliders;
				break;
			case 'x': // rotate x
				rotx += rotAngle;
				break;
			case 'y': // rotate y
				roty += rotAngle;
				break;
			case 'z': // rotate z
				rotz += rotAngle;
				break;
			case 'd': //dancing
				//dance = !dance;
				if (dance) {
					PlaySound(TEXT("dance1.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
				else {
					PlaySound(NULL, 0, 0);
					for (Robot& r : Robots) {
						r.TPose();
					}
					frame = 0;
				}
				break;
			case 'i':
				individual = !individual;
				break;
			case 'm': //movement
				dance = !dance;
				break;
			case 32: // space
				Shoot(Bullets, Vector3(x,2,z), angle);
				//BONUS 2
				if (!mute) SoundEngine->play2D("audio/shot.mp3", false);
				break;
		}
		case 27: // escape
			menuSelection = 0;
			showPause = !showPause;
			break;

		
	}

	// ========== Pause Menu Interaction =============
	if (showPause && key == '\r') { // Enter
		if (menuSelection == 0) menuResume();
		else if (menuSelection == 1) menuNewGame();
		else if (menuSelection == 2) menuExit();
	}

	glutPostRedisplay();
}

//handles special keys
void processSpecialKeys(int key, int xx, int yy) {
	
	if (!showPause) {
		if (!gameOver) {
			float fraction = 0.5f;
			switch (key) {
			case GLUT_KEY_LEFT:
				angle -= 0.05f;
				lx = sin(angle);
				lz = -cos(angle);
				break;
			case GLUT_KEY_RIGHT:
				angle += 0.1f;
				lx = sin(angle);
				lz = -cos(angle);
				break;
			case GLUT_KEY_UP:
				x += lx * fraction;
				z += lz * fraction;
				break;
			case GLUT_KEY_DOWN:
				x -= lx * fraction;
				z -= lz * fraction;
				break;
			case GLUT_KEY_F1: // F1: toggle beween full - screen and windowed mode
				fullScreenMode = !fullScreenMode;
				if (fullScreenMode) {
					windowPosx = glutGet(GLUT_WINDOW_X);
					windowPosy = glutGet(GLUT_WINDOW_Y);
					windowW = glutGet(GLUT_WINDOW_WIDTH);
					windowH = glutGet(GLUT_WINDOW_HEIGHT);
					glutFullScreen(); // switch into fullscreen
				}
				else {// window mode
					glutReshapeWindow(WINDOW_W, WINDOW_H); //switch into windowed mode
					glutPositionWindow(windowPosx, windowPosy);
				}
				break;
			case GLUT_KEY_F2:
				swapCam = !swapCam;
				break;
			case GLUT_KEY_F3:
				mute = !mute;
				if (mute) SoundEngine->stopAllSounds();
				else {
					irrklang::ISound* music = SoundEngine->play2D("audio/music.mp3", true, false, true);
					if (music) music->setVolume(0.5f);
				}
				break;

			case GLUT_KEY_F4:
				bGouraud = !bGouraud;
				if (bGouraud) glShadeModel(GL_SMOOTH);
				else glShadeModel(GL_FLAT);
				glutPostRedisplay();
				break;
			}
			
		}
	}
	else {
		if (key == GLUT_KEY_UP) {
			menuSelection = (menuSelection + 2) % 3; // wrap
		}
		else if (key == GLUT_KEY_DOWN) {
			menuSelection = (menuSelection + 1) % 3; // wrap
		}
	}
		
}

//handles framerate and timing functions
void timer(int v)
{
	if (!gameOver && !showPause) {
		
		if (dance) {
			//running anim
			if (dance) {
				++frame;
				if (frame > 8) {
					frame = 0;
					pose = !pose;

					if (pose) {
						for (Robot& r : Robots) {
							r.Dance1();
						}

					}
					else {
						for (Robot& r : Robots) {
							r.Dance2();
						}
					}
				}
			}
			++jump;
			//jump
			if (jump > jumpTrig) {
				if (!jumping) {
					jumper = rand() % Robots.size();
					jumping = true;
					//BONUS 2
					if (!mute) {
						irrklang::ISound* jmp = SoundEngine->play2D("audio/jump.mp3", false, false, true);
						if (jmp) jmp->setVolume(0.2f);
					}
				}


				if (jumpTimer > 0) {
					//cout << "Jump!" << endl;
					float jumpAccelerationTemp = jumpAcceleration;
					float jumpVelocityTemp = jumpVelocity;

					Robots[jumper].transform.y += jumpVelocityTemp;

					jumpVelocityTemp -= jumpAccelerationTemp; // Apply gravity to decrease velocity

					jumpTimer--;
				}
				else {
					jump = 0;
					jumping = false;
					jumpTimer = resetJumpTimer;
					jumpTrig = 100 + rand() % jumpTrigLim;
				}
			}


			//BONUS 1
			//ai movement and gravity
			for (Robot& r : Robots) {
				r.Move();
				if (r.transform.y > 1.5) r.transform.y -= gravity;
			}
		}
		

		gameTime -= .01;
		checkCollisions();
		BulletHandling(Bullets, bulletSpeed, score, x, z);
	}

	glutPostRedisplay();
	glutTimerFunc(10, timer, v); // Adjust frame delay based on FPS


}

//window readjustment
void changeSize(int width, int height)
{
	windowW = width;
	windowH = height;
	// Set the viewport to cover the entire window
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	// Set up the projection matrix to maintain aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Use gluPerspective for a perspective projection
	// You can adjust the field of view (60.0), aspect ratio, near and farclipping planes
	gluPerspective(60.0, (double)(8 * windowW) / (windowH * 7), 0.1,
			100.0);
	// Switch back to the modelview matrix for drawing
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat light_position[] = { 1.0, 0.0, 2.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}


// ========== Camera Hanling =============
//ESC Cam
void topCam() {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	// handles switching fpv with top view
	if (!swapCam) {
		glViewport(windowW - ((windowW / 5)), windowH -
			(windowH / 5), windowW / 5, windowH / 5); // Small SizeWindow for the whole view
		// define the scissor box to match viewport
		glEnable(GL_SCISSOR_TEST);
		glScissor(windowW - (windowW / 5), windowH - (windowH / 5), windowW / 5, windowH / 5);

		// clear only that region
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// disable scissor after clear
		glDisable(GL_SCISSOR_TEST);

		if (dLighting)renderBackgroundTexture(texID[2]);

		
	}
	else {
		glViewport(0, (windowH / 8), windowW, windowH -(windowH /8));

		// Clear only the COLOR buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw background
		if (dLighting)renderBackgroundTexture(texID[2]);

		// NOW clear the DEPTH buffer so 3D objects can draw correctly
		glClear(GL_DEPTH_BUFFER_BIT);

		// Enable depth test for 3D scene
		glEnable(GL_DEPTH_TEST);
		
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	
	gluPerspective(fov, (double)(windowW / 5) / (windowH / 5),
		0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set the Camera (Entire scene view)
	gluLookAt(arcx, arcy, arcz,0, 0, 0, 0, 1, 0); // Entirescene view

	// Flashlight Allignment
	GLfloat point_light_position[] = { x, 2.5f, z, 1.0f };  // at camera
	GLfloat spot_direction[] = { lx, 0.0f, lz };      // points where camera looks

	glLightfv(GL_LIGHT1, GL_POSITION, point_light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

	glPushMatrix();
	glRotated(rotx, 1, 0, 0);
	glRotated(roty, 0, 1, 0);
	glRotated(rotz, 0, 0, 1);
	if (!clear) {
		DrawGround(mode, texID[0]);
		for (Robot& r : Robots) {
			r.Draw(mode, bnw, showColliders);
		}

		for (Robot& r : Dead) {
			//r.Draw();
		}

		if (axis) MyAxis();

		for (Bullet& b : Bullets) {
			b.Draw(mode);
		}

		if (bObj) {
			if (!scene) {
				std::cout << "Cannot render: scene is NULL!" << std::endl;
				return;
			}
			glPushMatrix();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glTranslatef(0, 40, -70);
			recursive_render(scene, scene->mRootNode, 5.0f, textureIdMap);
			glPopAttrib();
			glPopMatrix();
		}

		// Camera Visual
		SpawnCube(mode, texID[1], Vector3(x, 0, z), Vector3(1, 5, 1), Vector3(0, -angle* 57.27, 0), bnw ? toGrayscale(Vector3(0,1,1)) : Vector3(0, 1, 1));
		//Gun Visual
		SpawnCube(mode, texID[1], Vector3(x, 2.2, z), Vector3(.5, .5, 3), Vector3(0, -angle * 57.27, 0), bnw ? toGrayscale(Vector3(0, 1, 0)) : Vector3(0, 1, 0));
	}
	glPopMatrix();
}

void UICam() {
	glDisable(GL_LIGHTING);
	glViewport(0, 0,(windowW), (windowH / 8));
	// define the scissor box to match viewport
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, (windowW), windowH /8);

	// clear only that region
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// disable scissor after clear
	glDisable(GL_SCISSOR_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1); // simple 0–1 coordinate system
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	

	
	
	glColor3f(1.0f, 1.0f, 1.0f);
	list<string> col1, col2, col3, col4;
	string title = "(Kill +10 | Miss -10)";
	col1.push_back(title);
	string gamescore = "Score: " + std::to_string(score);
	col1.push_back(gamescore);
	string rk = "Kills: " + std::to_string(kills);
	col1.push_back(rk);
	string time = "Time: " + std::to_string((int)gameTime);
	col2.push_back(time);
	string BulletSpeed = "Bullet Speed: " ;
	string BulletSpeedResult = "- ";
	switch (bulletSpeedMode)
	{
	case 0:
		BulletSpeedResult += "Slow";
		break;
	case 1:
		BulletSpeedResult += "Fast";
		break;
	case 2:
		BulletSpeedResult += "Very Fast";
		break;
	}
	
	col2.push_back(BulletSpeed);
	col2.push_back(BulletSpeedResult);
	col3.push_back(gameState);
	string shading = "";
	bGouraud? shading = "Gouraud" : shading = "Flat";
	col3.push_back(shading);
	string lighting = "";
	dLighting ? lighting = "Directional" : lighting = "Point";
	col3.push_back(lighting);

	string texText = "Q - texture";
	col4.push_back(texText);
	string tex = "";
	mode == 3 ? tex = "Texture" : tex = "No Texture";
	col4.push_back(tex);

	
	float tx = 0.01f;
	float ty = 0.75f;

	for (string text : col1) {
		glRasterPos2f(tx, ty);
		ty -= 0.25f;
		for (char c : text) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}
	}

	tx = 0.27f;
	ty = 0.75f;

	for (string text : col2) {
		glRasterPos2f(tx, ty);
		ty -= 0.25f;
		for (char c : text) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}
	}

	tx = 0.5f;
	ty = 0.75f;

	for (string text : col3) {
		glRasterPos2f(tx, ty);
		ty -= 0.25f;
		for (char c : text) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}
	}
	tx = 0.75f;
	ty = 0.75f;

	for (string text : col4) {
		glRasterPos2f(tx, ty);
		ty -= 0.25f;
		for (char c : text) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		}
	}
	glEnable(GL_LIGHTING);
	
}

void mainCam() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//handles swapping top view with fpv
	if (!swapCam) {
		glViewport(0, (windowH / 8), windowW, windowH - (windowH / 8));

		// Clear only the COLOR buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw background
		if (dLighting)renderBackgroundTexture(texID[2]);

		// NOW clear the DEPTH buffer so 3D objects can draw correctly
		glClear(GL_DEPTH_BUFFER_BIT);

		// Enable depth test for 3D scene
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glViewport(windowW - ((windowW / 5)), windowH -
			(windowH / 5), windowW / 5, windowH / 5); // Small SizeWindow for the whole view
		// define the scissor box to match viewport
		glEnable(GL_SCISSOR_TEST);
		glScissor(windowW - (windowW / 5), windowH - (windowH / 5), windowW / 5, windowH / 5);

		// clear only that region
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// disable scissor after clear
		glDisable(GL_SCISSOR_TEST);

		if(dLighting)renderBackgroundTexture(texID[2]);
	}

	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	
	gluPerspective(60.0, (double)(windowW) / (windowH),
		0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, 2.5f, z,
		x + lx, 2.5f, z + lz,
		0.0f, 1.0f, 0.0f);

	// Flashlight Allignment
	GLfloat point_light_position[] = { x, 2.5f, z, 1.0f };  // at camera
	GLfloat spot_direction[] = { lx, 0.0f, lz };      // points where camera looks

	glLightfv(GL_LIGHT1, GL_POSITION, point_light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);


	glPushMatrix();
	glRotated(rotx, 1, 0, 0);
	glRotated(roty, 0, 1, 0);
	glRotated(rotz, 0, 0, 1);
	if (!clear) {
		DrawGround(mode, texID[0]);
		for (Robot& r : Robots) {
			r.Draw(mode, bnw, showColliders);
		}

		for (Robot& r : Dead) {
			//r.Draw();
		}

		for (Bullet& b : Bullets) {
			b.Draw(mode);
		}

		if (axis) MyAxis();

		
		if (bObj) {
			if (!scene) {
				std::cout << "Cannot render: scene is NULL!" << std::endl;
				return;
			}
			glPushMatrix();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glTranslatef(0, 40, -70);
			recursive_render(scene, scene->mRootNode, 5.0f, textureIdMap);
			glPopAttrib();
			glPopMatrix();
		}
		
		//Reticle
		SpawnSphere(2, -1, 10, 10, Vector3(x + lx, 2.5f, z + lz), Vector3(.007, .007, .007), Vector3(0,0,0), Vector3(255,255,255));
		//Gun Visual
		SpawnCube(mode, texID[1], Vector3(x, 2, z), Vector3(.1, .1, 3), Vector3(0, -angle * 57.27, 0), bnw ? toGrayscale(Vector3(0, 1, 0)) : Vector3(0, 1, 0));
		
	}
	glPopMatrix();

	
	
}



void checkCollisions() {
	for (int i = 0; i < Bullets.size(); ++i) {
		for (int j = 0; j < Robots.size(); ++j) {
			if (sphereCollision(Bullets[i].pos, Bullets[i].colliderRadius, Robots[j].GetColliderPos(), Robots[j].colliderRadius)) {

				++kills;
				score += 10;
				Robot r = Robot(Robots[j].transform, kills, -1);
				r.Hit();
				//BONUS 2
				if (!mute)SoundEngine->play2D("audio/kill.mp3", false);
				Dead.push_back(r);

				Robots[j] = Robots.back();
				Robots.pop_back();
				Bullets[i] = Bullets.back();
				Bullets.pop_back();
				--i;
				break;
			}
		}
	}
}



// ========== Game State Manager =============
void GameStateManager() {
	if (kills == spawnNum) {
		if (!gameOver) {
			gameState = "You Win!";
			gameOver = true;
			//BONUS 2
			SoundEngine->stopAllSounds();
			if (!mute)SoundEngine->play2D("audio/win.mp3", false);
		}
	}

	if (gameTime <= 0) {
		if (!gameOver) {
			gameState = "You Lose :(";
			gameOver = true;
			//BONUS 2
			SoundEngine->stopAllSounds();
			if (!mute)SoundEngine->play2D("audio/gameOver.mp3", false);
		}
	}

}

// ========== Main Display Function =============
//handles constant display
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);
	

	if (!showPause) {
		if (!swapCam) {
			mainCam();
			topCam();
		}
		else {
			topCam();
			mainCam();
		}
		UICam();
	}
	else {
		glDisable(GL_TEXTURE_2D);
		drawMenuOverlay();
	}
	

	GameStateManager();

	glutSwapBuffers();
	
}

// ========== Main OpenGL Function =============
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // RGB mode
	glutInitWindowSize(WINDOW_W, WINDOW_H); // window size
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Isaac Hraga 811296505");
	
	glClearColor(0.0, 0.0, 0.0, 1.0); // clear the window screen
	init();
	
	glutTimerFunc(0, timer, 0);
	
	glutDisplayFunc(MyDisplay); // call the drawing function
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(processSpecialKeys);

	

	createMenu();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 1;
}
