#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>

#define GLUT_KEY_ESCAPE 27 //escape button's ascii code
#define DEG2RAD(a) (a * 0.0174532925)

void* fontA = GLUT_BITMAP_8_BY_13;
void* fontB = GLUT_BITMAP_9_BY_15;
void* fontC = GLUT_BITMAP_TIMES_ROMAN_10;
void* fontD = GLUT_BITMAP_TIMES_ROMAN_24;
void* fontE = GLUT_BITMAP_HELVETICA_10;
void* fontF = GLUT_BITMAP_HELVETICA_12;
void* fontG = GLUT_BITMAP_HELVETICA_18;

using namespace std;

//fence
float fenceX = 0.05;
float fenceY = 0.2;
float fenceW = 0.02;
//float fenceH = 0.2;

//character movements
//0.1,0.12,0.096
double characterInitX = 0;
double characterInitY = 0;
double characterInitZ = 0;
double characterX = 0;
double characterY = 0;
double characterZ = 0;
bool frontFlag = false;
bool leftFlag = false;
bool rightFlag = false;
bool backFlag = false;
GLdouble rotAng = 0;
GLdouble rotX = 0;
GLdouble rotY = 0;
GLdouble rotZ = 0;

//gift locations //-0.2, 0, 0.06
double gift1 = -0.2;
double gift2 = 0.5;
double gift3 = 0.2;
double gift4 = -0.1;
double gift1Z = 0.06;
double gift2Z = 0;
double gift3Z = 0.3;
double gift4Z = 0.2;

int giftX1Disappear = 0;
int giftY1Disappear = 0;
int giftZ1Disappear = 0;

int giftX2Disappear = 0;
int giftY2Disappear = 0;
int giftZ2Disappear = 0;

int giftX3Disappear = 0;
int giftY3Disappear = 0;
int giftZ3Disappear = 0;

int giftX4Disappear = 0;
int giftY4Disappear = 0;
int giftZ4Disappear = 0;

//object animations
double buildingXAnim = 1;
double buildingYAnim = 1;
double buildingZAnim = 1;
bool scaleBuilding = false;

double gateZAnim = 0;
bool translateGate = false;

double snowAngle = 0;
bool rotateSnowman = false;

double lanternY = 0;
bool translateLanterns = false;

double giftAngle = 0;
bool rotateGifts = false;

double fenceXAnim = 1;
double fenceYAnim = 1;
double fenceZAnim = 1;
bool scaleFence = false;

float fenceRed = 0.3f;
float fenceGreen = 0.3f;
float fenceBlue = 0.1f;

bool color = true;
bool building = true;
bool gate = true;
bool fence = true;
bool snowman = true;
bool lantern = true;

bool gameEnd = false;
double timeInc = 0;
double gameOverX = 0;
double gameOverY = 0;
double gameOverZ = 0;
bool playEnd = true;
bool won = false;

bool playBackground = true;
bool playCollect1 = true;
bool playCollect2 = true;
bool playCollect3 = true;
bool playCollect4 = true;

bool playWon = true;


class Vector3f {
public:
	float x, y, z;
	//vector constructor -> initializes all its coordinates to 0s
	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	//adds values to vector coordinates
	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	//subtracts values from vector coordinates
	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	//multiplies vector coordinates by values
	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	//divides vector coordinates by values
	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	//return the unit vector
	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	//returns the cross product
	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
}; //end of vector class //

class Camera {
public:
	Vector3f eye, center, up; //the three modifiers of the camera class are vectors

	//constructor that initializes the positions of all the vectors
	Camera(float eyeX = 0.0f, float eyeY = 0.439682f, float eyeZ = 0.824115f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	//viewing vector
	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
}; //end of camera class //

Camera camera;


//////////////// DRAWING OBJECTS START ////////////////

void drawFloor(double thickness) {
	//glDisable(GL_LIGHTING);
	glPushMatrix();
	glScaled(1.0, thickness, 1.0); //width = x, height = y, thickness = z
	glutSolidCube(1);
	glPopMatrix();
	//glEnable(GL_LIGHTING);
}

void drawFence() {
	//glDisable(GL_LIGHTING);
	for (float i = -0.48; i <= 0.49; i += 0.08) {
		glPushMatrix();

		glPushMatrix();
		glTranslated(i, fenceY / 2, -0.49);
		glColor3f(fenceRed, fenceGreen, fenceBlue);
		glScaled(fenceX, fenceY, fenceW);
		glutSolidCube(1);
		/*drawRectangle(i, 0.5, 0.05, 0.2);*/
		glPopMatrix();

		glPushMatrix();
		glTranslated(i, fenceY / 2, -0.49);
		glRotated(90, 0, 0, 1);
		glColor3f(fenceRed, fenceGreen, fenceBlue);
		glScaled(fenceX, fenceY / 2, fenceW);
		glutSolidCube(1);
		/*drawRectangle(i, 0.5, 0.05, 0.2);*/
		glPopMatrix();

		glPopMatrix();
	}
	//glEnable(GL_LIGHTING);

}


void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient); //indirect light
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse); //direct light
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular); //reflection
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

	glEnable(GL_COLOR_MATERIAL);
	// Set Material Properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}


void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void drawScene() {

	//floor
	glPushMatrix();
	glColor3f(0.9, 1, 1);
	drawFloor(0.02);
	glPopMatrix();

	glPushMatrix();//animstart
	glScaled(fenceXAnim, fenceYAnim, fenceZAnim);
	//front fence
	drawFence();

	//right fence
	glPushMatrix();
	glRotated(90, 0, 1.0, 0);
	drawFence();
	glPopMatrix();

	//left fence
	glPushMatrix();
	glRotated(-90, 0, 1.0, 0);
	drawFence();
	glPopMatrix();
	glPopMatrix();//animend



}

void drawBuilding() { //width = x, height = y, thickness = z
	glPushMatrix();
	glColor3f(0, 0.2, 0.2);
	glScaled(0.2, 0.4, 0.2);
	glTranslated(-1.5, 0.5, -1.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.5, 0.1);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-9, 4.5, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9, 1, 0);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-6, 4.5, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9, 1, 0);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-7.5, 4.5, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.5, 0.1);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-9, 3, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.5, 0.1);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-6, 3, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.5, 0.1);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-7.5, 3, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9, 1, 0);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-9, 1.5, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.5, 0.1);
	glScaled(0.04, 0.07, 0.01);
	glTranslated(-6, 1.5, -20);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScaled(0.04, 0.14, 0.01);
	glTranslated(-7.5, 0.5, -20);
	glutSolidCube(1);
	glPopMatrix();

}

void drawCylinder() {
	glPushMatrix();
	glTranslated(0, 0, 1);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GL_QUADS);
	gluCylinder(qobj, 0.2, 0.2, 0.4, 8, 8);
	glPopMatrix();
}

void drawGate() {
	glPushMatrix();
	glColor3f(0.6, 0, 0);
	glScaled(0.08, 1, 0.08);
	glTranslated(-1.3, 1.41, -2);
	glRotated(90, 1, 0, 0);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScaled(0.09, 0.3, 0.09);
	glTranslated(-1.15, 1.41, -1.77);
	glRotated(90, 1, 0, 0);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0, 0);
	glScaled(0.08, 1, 0.08);
	glTranslated(1.3, 1.41, -2);
	glRotated(90, 1, 0, 0);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScaled(0.09, 0.3, 0.09);
	glTranslated(1.15, 1.41, -1.77);
	glRotated(90, 1, 0, 0);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0, 0);
	glTranslated(0, 0.3, -0.16);
	glRotated(90, 0, 0, 1);
	glScaled(0.03, 0.3, 0.03);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0, 0);
	glTranslated(0, 0.4, -0.16);
	glRotated(90, 0, 0, 1);
	glScaled(0.03, 0.35, 0.03);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0, 0.422, -0.16);
	glRotated(90, 0, 0, 1);
	glScaled(0.015, 0.35, 0.03);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0, 0);
	glTranslated(0, 0.35, -0.16);
	glScaled(0.04, 0.1, 0.03);
	glutSolidCube(1);
	glPopMatrix();
}

void drawSnowman() {

	//body
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0.046, 0);
	glScaled(0.3, 0.4, 0.3);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	//head
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0.1, 0);
	glScaled(0.3, 0.4, 0.3);
	glutSolidSphere(0.06, 25, 25);
	glPopMatrix();

	//buttons
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0, 0.066, 0.025);
	glutSolidSphere(0.005, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0, 0.046, 0.03);
	glutSolidSphere(0.005, 25, 25);
	glPopMatrix();

	//eyes
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(-0.006, 0.11, 0.016);
	glutSolidSphere(0.003, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.006, 0.11, 0.016);
	glutSolidSphere(0.003, 25, 25);
	glPopMatrix();

	//nose
	glPushMatrix();
	glColor3f(0.8, 0.2, 0);
	glTranslated(0, 0.1, 0.01);
	glutSolidCone(0.004, 0.02, 25, 25);
	glPopMatrix();

	//arms
	glPushMatrix();
	glColor3f(0.4, 0.1, 0);
	glTranslated(0.026, 0.06, 0);
	glRotated(90, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	glScaled(0.005, 0.05, 0.005);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.4, 0.1, 0);
	glTranslated(0.036, 0.076, 0);
	glRotated(45, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	glScaled(0.005, 0.01, 0.005);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.1, 0);
	glTranslated(-0.026, 0.06, 0);
	glRotated(-90, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	glScaled(0.005, 0.05, 0.005);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.4, 0.1, 0);
	glTranslated(-0.036, 0.076, 0);
	glRotated(45, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	glScaled(0.005, 0.01, 0.005);
	glutSolidCube(1);
	glPopMatrix();

}

void drawLantern() {

	glPushMatrix();
	glColor3f(1, 0.3, 0.1);
	glTranslated(0.2, 0.08, 0.3);
	//glRotated(90, 1, 0, 0);
	glScaled(1.4, 1.5, 1);
	glutSolidSphere(0.02, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.2, 0.05, 0.3);
	glScaled(0.026, 0.01, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.2, 0.11, 0.3);
	glScaled(0.026, 0.01, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.2, 0.112, 0.3);
	glScaled(0.01, 0.02, 0.02);
	glutSolidCube(1);
	glPopMatrix();

}

void drawGift() {
	glPushMatrix();
	glColor3f(0, 0.3, 0.1);
	glTranslated(gift1, 0, gift1Z);
	glScaled(0.056, 0.15, 0.06);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(-0.2, 0, 0.09);
	glScaled(0.02, 0.15, 0.003);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(-0.2, 0.076, 0.06);
	glRotated(90, 1, 0, 0);
	glScaled(0.02, 0.06, 0.003);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(-0.2, 0, 0.029);
	glScaled(0.02, 0.15, 0.003);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(-0.171, 0, 0.06);
	glRotated(90, 0, 1, 0);
	glScaled(0.02, 0.15, 0.003);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(-0.229, 0, 0.06);
	glRotated(-90, 0, 1, 0);
	glScaled(0.02, 0.15, 0.003);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(-0.2, 0.076, 0.06);
	glRotated(90, 0, 1, 0);
	glRotated(90, 1, 0, 0);
	glScaled(0.02, 0.06, 0.003);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glScaled(0.06, 0.06, 0.06);
	glTranslated(-3.15, 1.49, -0.2);
	glColor3f(0.5, 0, 0);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.06, 0.06, 0.06);
	glTranslated(-3.55, 1.49, -0.2);
	glColor3f(0.5, 0, 0);
	drawCylinder();
	glPopMatrix();
}

void drawCharacter() {
	//body
	glPushMatrix();
	glColor3f(0.4, 0.7, 0.5);
	glTranslated(0.1, 0.12, 0.096);
	glScaled(0.06, 0.11, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	//legs
	glPushMatrix();
	glColor3f(0.4, 0.7, 0.5);
	glTranslated(0.075, 0.07, 0.096);
	glScaled(0.01, 0.099, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.7, 0.5);
	glTranslated(0.125, 0.07, 0.096);
	glScaled(0.01, 0.099, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.7, 0.5);
	glTranslated(0.125, 0.02, 0.103);
	glRotated(90, 1, 0, 0);
	glScaled(0.01, 0.054, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.7, 0.5);
	glTranslated(0.075, 0.02, 0.103);
	glRotated(90, 1, 0, 0);
	glScaled(0.01, 0.054, 0.02);
	glutSolidCube(1);
	glPopMatrix();

	//face
	glPushMatrix();
	glColor3f(1, 0.9, 0.5);
	glTranslated(0.1, 0.15, 0.099);
	glScaled(0.05, 0.04, 0.04);
	glutSolidCube(1);
	glPopMatrix();

	//eyes
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.116, 0.153, 0.118);
	glutSolidSphere(0.003, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.083, 0.153, 0.118);
	glutSolidSphere(0.003, 25, 25);
	glPopMatrix();

	//nose
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.0995, 0.15, 0.12);
	glutSolidSphere(0.001, 25, 25);
	glPopMatrix();

}

void collectGifts() {
	//0.1,0.12,0.096
	//cout << "gift1: " << gift1 << endl;
	//cout << "gift2: " << gift2 << endl;
	//cout << "gift3: " << gift3 << endl;
	//cout << "gift4: " << gift4 << endl;
	if (characterX + 0.1 >= gift1 - (0.056 / 2) && characterX + 0.1 <= gift1 + (0.056 / 2)) {
		if (characterZ + 0.096 >= gift1Z - (0.06 / 2) && characterZ + 0.096 <= gift1Z + (0.06 / 2)) {
			if (playCollect1) {
				playBackground = false;
				PlaySound(TEXT("audio/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
				playCollect1 = false;
				playCollect2 = true;
				playCollect3 = true;
				playCollect4 = true;
			}
			giftX1Disappear = 10000;
		}
	}
	if (characterX + 0.1 >= (gift2 + gift1) - (0.056 / 2) && characterX + 0.1 <= (gift2 + gift1) + (0.056 / 2)) {
		if (characterZ + 0.096 >= (gift2Z + gift1Z) - (0.06 / 2) && characterZ + 0.096 <= (gift2Z + gift1Z) + (0.06 / 2)) {
			if (playCollect2) {
				playBackground = false;
				PlaySound(TEXT("audio/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
				playCollect2 = false;
				playCollect1 = true;
				playCollect3 = true;
				playCollect4 = true;
			}
			giftX2Disappear = 10000;
		}
	}
	if (characterX + 0.1 >= (gift3 + gift1) - (0.056 / 2) && characterX + 0.1 <= (gift3 + gift1) + (0.056 / 2)) {
		if (characterZ + 0.096 >= (gift3Z + gift1Z) - (0.06 / 2) && characterZ + 0.096 <= (gift3Z + gift1Z) + (0.06 / 2)) {
			if (playCollect3) {
				playBackground = false;
				PlaySound(TEXT("audio/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
				playCollect3 = false;
				playCollect4 = true;
				playCollect2 = true;
				playCollect1 = true;
			}
			giftX3Disappear = 10000;
		}
	}
	if (characterX + 0.1 >= (gift4 + gift1) - (0.056 / 2) && characterX + 0.1 <= (gift4 + gift1) + (0.056 / 2)) {
		if (characterZ + 0.096 >= (gift4Z + gift1Z) - (0.06 / 2) && characterZ + 0.096 <= (gift4Z + gift1Z) + (0.06 / 2)) {
			if (playCollect4) {
				playBackground = false;
				PlaySound(TEXT("audio/collect.wav"), NULL, SND_FILENAME | SND_ASYNC);
				playCollect4 = false;
				playCollect3 = true;
				playCollect2 = true;
				playCollect1 = true;
			}
			giftX4Disappear = 10000;
		}
	}
}


void Anim() {
	if (scaleBuilding) {
		if (building) {
			if (buildingXAnim > 0 && buildingYAnim > 0 && buildingZAnim > 0) {
				buildingXAnim -= 0.001;
				buildingYAnim -= 0.001;
				buildingZAnim -= 0.001;
			}
		}
		if (buildingXAnim <= 0.5 || !building) {
			building = false;
			buildingXAnim += 0.001;
			buildingYAnim += 0.001;
			buildingZAnim += 0.001;
			if (buildingXAnim >= 1) {
				building = true;
			}
		}
	}
	else if (!scaleBuilding) {
		buildingXAnim = 1;
		buildingYAnim = 1;
		buildingZAnim = 1;
	}

	if (translateGate) {
		if (gate) {
			if (gateZAnim >= -0.32) {
				//cout << "gate: " << gateZAnim << endl;
				gateZAnim -= 0.001;
			}
		}
		if (gateZAnim < -0.32 || !gate) {
			gate = false;
			if (gateZAnim <= 0.37) {
				gateZAnim += 0.001;
			}
			if (gateZAnim >= 0.37) {
				gate = true;
			}
		}
	}
	else if (!translateGate) {
		gateZAnim = 0;
	}

	if (rotateSnowman) {
		snowAngle -= 0.1;
	}
	else if (!rotateSnowman) {
		snowAngle = 0;
	}

	if (translateLanterns) {
		if (lantern) {
			lanternY += 0.0004;
		}
		if (lanternY >= 0.3 || !lantern) {
			lantern = false;
			lanternY -= 0.0004;
			if (lanternY <= 0) {
				lantern = true;
			}
		}
	}
	else if (!translateLanterns) {
		lanternY = 0;
	}

	if (rotateGifts) {
		giftAngle -= 0.1;
	}
	else if (!rotateGifts) {
		giftAngle = 0;
	}

	if (scaleFence) {
		if (fence) {
			fenceXAnim += 0.001;
			fenceYAnim += 0.001;
			fenceZAnim += 0.001;
		}
		if (fenceXAnim >= 2 || !fence) {
			fence = false;
			fenceXAnim -= 0.001;
			fenceYAnim -= 0.001;
			fenceZAnim -= 0.001;
			if (fenceXAnim <= 1) {
				fence = true;
			}
		}
	}
	else if (!scaleFence) {
		fenceXAnim = 1;
		fenceYAnim = 1;
		fenceZAnim = 1;
	}

	if (color) {
		fenceBlue += 0.001;
	}
	if (fenceBlue >= 1 || !color) {
		color = false;
		fenceBlue -= 0.001;
		if (fenceBlue <= 0)
		{
			color = true;
		}
	}
	//double time = 0.0;
	//while (time < 6767) {
	//	cout << "time: " << time << endl;
	//	if (giftX1Disappear == 10000 && giftX2Disappear == 10000 && giftX3Disappear == 10000 && giftX4Disappear == 10000) {
	//		//
	//	}
	//	time++;
	//}
	//if (time >= 6767) {
	//	gameEnd = true;
	//}

	glutPostRedisplay();
}

void play() {
	PlaySound(TEXT("audio/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

//void print(int x, int y, float r, float g, float b, void* font, char* string)
//{
//	glColor3f(r, g, b);
//	glRasterPos2f(x, y);
//	int len, i;
//	len = (int)strlen(string);
//	for (i = 0; i < len; i++) {
//		glutBitmapCharacter(font, string[i]);
//	}
//}

void timer(int value) {
	cout << "time: " << timeInc << endl;
	if (timeInc <= 1000) {
		timeInc += 0.5;
	}
	if (timeInc > 1000 && (giftX1Disappear != 10000 || giftX2Disappear != 10000 || giftX3Disappear != 10000 || giftX4Disappear != 10000)) {
		gameEnd = true;
		gameOverX = 10000;
		gameOverY = 10000;
		gameOverZ = 10000;
		playBackground = false;
		//playEnd = true;
		if (playEnd) {
			play();
			playEnd = false;
		}
	}
	if (timeInc <= 1000 && giftX1Disappear == 10000 && giftX2Disappear == 10000 && giftX3Disappear == 10000 && giftX4Disappear == 10000) {
		playBackground = false;
		playCollect1 = false;
		playCollect2 = false;
		playCollect3 = false;
		playCollect4 = false;
		if (playWon) {
			PlaySound(TEXT("audio/won.wav"), NULL, SND_FILENAME | SND_ASYNC);
			playWon = false;
		}
		rotateSnowman = true;
		scaleBuilding = true;
		translateGate = true;
		translateLanterns = true;
		scaleFence = true;
		won = true;
	}
	glutPostRedisplay;

	glutTimerFunc(20, timer, 0);
}

void displayText() {
	glDisable(GL_TEXTURE_2D); //added this
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 640, 0.0, 480);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	if (won) {
		glColor3f(0.0f, 0.6f, 0.2f);
		glRasterPos2i(235, 160);
	}
	else if (gameEnd) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glRasterPos2i(265, 160);
	}
	if (won) {
		string s = "Awesome! You won!";
		void* font = GLUT_BITMAP_HELVETICA_18;
		for (string::iterator i = s.begin(); i != s.end(); ++i)
		{
			char c = *i;
			glColor3d(1.0, 0.0, 0.0);
			glutBitmapCharacter(font, c);
		}
	}
	else if (gameEnd) {
		string s = "You lost... :(";
		void* font = GLUT_BITMAP_HELVETICA_18;
		for (string::iterator i = s.begin(); i != s.end(); ++i)
		{
			char c = *i;
			glColor3d(1.0, 0.0, 0.0);
			glutBitmapCharacter(font, c);
		}
	}
	glMatrixMode(GL_PROJECTION); //swapped this with...
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW); //...this
	glPopMatrix();
	//added this
	glEnable(GL_TEXTURE_2D);
}

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	displayText();

	drawScene();

	glPushMatrix(); //gameover start
	glTranslated(gameOverX, gameOverY, gameOverZ);

	glPushMatrix(); //anim1start
	glScaled(buildingXAnim, buildingYAnim, buildingZAnim);

	drawBuilding();

	glPushMatrix();
	glTranslated(0.6, 0, 0);
	drawBuilding();
	glPopMatrix();
	glPopMatrix(); //anim1end

	glPushMatrix(); //anim2start
	glTranslated(0, 0, gateZAnim);
	drawGate();
	glPopMatrix(); //anim2end

	glPushMatrix(); //anim3start
	glTranslated(0, 0.046, 0); //+ve
	glRotated(snowAngle, 0, 1, 0);
	glTranslated(0, -0.046, 0); //-ve
	drawSnowman();
	glPopMatrix(); //anim3end

	glPushMatrix(); //anim4start
	glTranslated(0, lanternY, 0);
	glPushMatrix();
	glTranslated(0, 0.1, -0.3);
	drawLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.06, 0.07, 0);
	drawLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.4, 0.1, -0.3);
	drawLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.44, 0.07, 0);
	drawLantern();
	glPopMatrix();
	glPopMatrix(); //anim4end

	glPushMatrix(); //anim5start
	glTranslated(gift1, 0, gift1Z);
	glRotated(giftAngle, 0, 1, 0);
	glTranslated(-gift1, 0, -gift1Z);
	glPushMatrix();
	glTranslated(giftX1Disappear, giftY1Disappear, giftZ1Disappear);
	drawGift();
	glPopMatrix();
	glPopMatrix(); //anim5end

	glPushMatrix(); //anim6start
	glTranslated(gift2 + gift1, 0, gift2Z + gift1Z);
	glRotated(giftAngle, 0, 1, 0);
	glTranslated(-(gift2 + gift1), 0, -(gift2Z + gift1Z));
	glPushMatrix();
	glTranslated(giftX2Disappear, giftY2Disappear, giftZ2Disappear);
	glTranslated(gift2, 0, gift2Z);
	drawGift();
	glPopMatrix();
	glPopMatrix(); //anim6end

	glPushMatrix(); //anim7start
	glTranslated(gift3 + gift1, 0, gift3Z + gift1Z);
	glRotated(giftAngle, 0, 1, 0);
	glTranslated(-(gift3 + gift1), 0, -(gift3Z + gift1Z));
	glPushMatrix();
	glTranslated(giftX3Disappear, giftY3Disappear, giftZ3Disappear);
	glTranslated(gift3, 0, gift3Z);
	drawGift();
	glPopMatrix();
	glPopMatrix(); //anim7end

	glPushMatrix(); //anim8start
	glTranslated(gift4 + gift1, 0, gift4Z + gift1Z);
	glRotated(giftAngle, 0, 1, 0);
	glTranslated(-(gift4 + gift1), 0, -(gift4Z + gift1Z));
	glPushMatrix();
	glTranslated(giftX4Disappear, giftY4Disappear, giftZ4Disappear);
	glTranslated(gift4, 0, gift4Z);
	drawGift();
	glPopMatrix();
	glPopMatrix(); //anim8end

	glPopMatrix(); //gameover end


	glPushMatrix();
	glTranslated(characterX, 0, characterZ); //for movement
	glTranslated(characterInitX, characterInitY, characterInitZ); //to retranslate after rotation
	glRotated(rotAng, rotX, rotY, rotZ);
	glTranslated(-characterInitX, -characterInitY, -characterInitZ);
	drawCharacter();
	glPopMatrix();

	collectGifts();

	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;
	float a = 1.0;

	switch (key) {
	case 'w':
		cout << "y: " << camera.eye.y << endl;
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		cout << "q: " << camera.eye.z << endl;
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);

		break;
	case 'f': //front view
		camera.eye.x = 0.0f;
		camera.eye.y = 0.4396f;
		camera.eye.z = 1.3241f;
		break;
	case 't': //top view
		camera.eye.x = 0.0f;
		camera.eye.y = 1.3241f;
		camera.eye.z = 0.4396f;
		break;
	case 'h': //side view
		camera.eye.x = 1.4241f;
		camera.eye.y = 0.4396f;
		camera.eye.z = -0.08;
		break;
	case 'g': //other side view
		camera.eye.x = -1.4241f;
		camera.eye.y = 0.4396f;
		camera.eye.z = -0.08;
		break;

		////rotations
	case 'i':
		camera.rotateX(a);
		break;
	case 'k':
		camera.rotateX(-a);
		break;
	case 'j':
		camera.rotateY(a);
		break;
	case 'l':
		camera.rotateY(-a);
		break;

		////animations
	case 'b':
		cout << scaleBuilding << endl;
		scaleBuilding = !scaleBuilding;
		break;
	case 'o':
		translateGate = !translateGate;
		break;
	case 'x':
		rotateSnowman = !rotateSnowman;
		break;
	case 'p':
		translateLanterns = !translateLanterns;
		break;
	case 'u':
		rotateGifts = !rotateGifts;
		break;
	case 'n':
		scaleFence = !scaleFence;
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	cout << "z: " << characterZ << endl;
	cout << "x: " << characterX << endl;
	//	if ((!(characterZ >= 1 || characterInitZ <=-1)) && (!(characterX >=1 || characterX <=-1))) {
	if (!gameEnd) {
		frontFlag = true;
		//0.1,0.12,0.096
		switch (key) {
		case GLUT_KEY_UP:
			if (frontFlag || leftFlag || rightFlag) {
				rotAng = 180;
				rotY = 1;
				characterInitX = 0.1;
				characterInitZ = 0.096;
				backFlag = true;
				frontFlag = false;
				leftFlag = false;
				rightFlag = false;
			}
			if (characterZ >= -0.54) {
				characterZ -= 0.01;
			}
			break;
		case GLUT_KEY_DOWN:
			if (backFlag || leftFlag || rightFlag) {
				rotAng = 0;
				rotY = 1;
				characterInitX = 0.1;
				characterInitZ = 0.096;
				backFlag = false;
				frontFlag = true;
				leftFlag = false;
				rightFlag = false;
			}
			if (characterZ <= 0.37) {
				characterZ += 0.01;
			}
			break;
		case GLUT_KEY_LEFT:
			if (rightFlag || frontFlag || backFlag) {
				rotAng = -90;
				rotY = 1;
				characterInitX = 0.1;
				characterInitZ = 0.096;
				leftFlag = true;
				rightFlag = false;
				frontFlag = false;
				backFlag = false;
			}
			if (characterX >= -0.55) {
				characterX -= 0.01;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (leftFlag || frontFlag || backFlag) {
				rotAng = 90;
				rotY = 1;
				characterInitX = 0.1;
				characterInitZ = 0.096;
				leftFlag = false;
				rightFlag = true;
				frontFlag = false;
				backFlag = false;
			}
			if (characterX <= 0.34) {
				characterX += 0.01;
			}
			break;
		}
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glOrtho(0.0F, 640, 0.0F, 480, -1.0F, 1.0F);

	if (bool playBackground = true) {
		PlaySound(TEXT("audio/ChristmasDreams.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	glutCreateWindow("Christmas Game");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutIdleFunc(Anim);
	glutTimerFunc(0, timer, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.1f, 0.1f, 0.2f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
