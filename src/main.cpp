#include "../include/main.h"
#include "../include/drawobject.h"

#include <pthread.h>
#define BALL_COUNT 3

double fovy=50;
double aspect;
double Near = 0.1, Far = 100;
int height = 760;
int width = 1260;

int rotAngle = 0;

int timeStep = 100;

V3 eye, lookup, lookat;

DrawObject cowObject;
LightObject lights[3];
ShadowMap shadowMap;

bool isSelected=false;
bool isLightOn[3];

GLint viewport[4];

void display() {
	GLfloat grey[] = {0.5, 0.5, 0.5, 1};
	//Display Light Sources
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	for(int i=0;i<2;i++){
		if(isLightOn[i]){
			glEnable(lights[i].lightID);
			GLfloat light_pos[] = {lights[i].pos[0], lights[i].pos[1], lights[i].pos[2], lights[i].pos[3]};
			GLfloat light_diffuse[] = {lights[i].diffuse[0], lights[i].diffuse[1], lights[i].diffuse[2], lights[i].diffuse[3]};
			GLfloat light_ambient[] = {lights[i].ambient[0], lights[i].ambient[1], lights[i].ambient[2], lights[i].ambient[3]};
			glLightfv(lights[i].lightID, GL_POSITION, light_pos);
			glLightfv(lights[i].lightID, GL_DIFFUSE, light_diffuse);
			glLightfv(lights[i].lightID, GL_AMBIENT, light_ambient);
		}
	}

	//=====================Shadow Map Construction====================
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, 1, Far);
	gluLookAt(
	lights[0].pos[0], lights[0].pos[1], lights[0].pos[2],
	lights[0].lookat[0], lights[0].lookat[1], lights[0].lookat[2],
	lights[0].lookup[0], lights[0].lookup[1], lights[0].lookup[2]);

	glShadeModel(GL_SMOOTH); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(1.f, 1.f, 1.f, 1.f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shadowMap.beginShadowMap();

	glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
	glPushMatrix();
		glColor3f(0.3, 0.6, 0.6);
		glTranslatef(0, -5, 0);
		glBegin(GL_POLYGON);
			glVertex3f(-20, 0, 20);
			glVertex3f(20, 0, 20);
			glVertex3f(20, 0, -20);
			glVertex3f(-20, 0, -20);
		glEnd();
	glPopMatrix();

	//Display Objects
	glPushMatrix();
		glRotatef(rotAngle*3, -1, 0, 0);
		cowObject.drawObject();
	glPopMatrix();

	shadowMap.endShadowMap();

	//=====================Actual Scene===============================
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, Near, Far);
	gluLookAt(
	eye[0], eye[1], eye[2],
	lookat[0], lookat[1], lookat[2],
	lookup[0], lookup[1], lookup[2]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.f, 1.f, 1.f, 1.f );
	glShadeModel(GL_SMOOTH); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Display Floor
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
	glPushMatrix();
		glColor3f(0.3, 0.6, 0.6);
		glTranslatef(0, -5, 0);
		glBegin(GL_POLYGON);
			glVertex3f(-20, 0, 20);
			glVertex3f(20, 0, 20);
			glVertex3f(20, 0, -20);
			glVertex3f(-20, 0, -20);
		glEnd();
	glPopMatrix();

	//Display Objects
	glPushMatrix();
		glRotatef(rotAngle*3, -1, 0, 0);
		cowObject.drawObject();
	glPopMatrix();


	//=======Left Bottom Viewport================

	glViewport(0, 0, width/3, height/3);
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glClearColor( 1.f, 1.f, 1.f, 1.f );

	glDisable(GL_LIGHTING);

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, shadowMap.rTexID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1, -1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1, -1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1, 1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	//=================================================================

	glFlush();
	glutSwapBuffers();
}

void keyboardCB(unsigned char keyPressed, int x, int y) {
	switch (keyPressed) {

	case 'q':
	case 27:
		exit(0);
		break;
	case '1':
	case '2':
	case '3':
		if(isLightOn[keyPressed-'1'])
			cout<<"Light "<<keyPressed<<" is Toggled Off\n";
		else
			cout<<"Light "<<keyPressed<<" is Toggled On\n";
		isLightOn[keyPressed-'1'] = !isLightOn[keyPressed-'1'];
	break;
	default:
	break;
	}
	glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(fovy, aspect, Near, Far);
	gluLookAt(
	eye[0], eye[1], eye[2],
	lookat[0], lookat[1], lookat[2],
	lookup[0], lookup[1], lookup[2]);
}

void glutMouse(int button, int state, int x, int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				cout<<"LM Click"<<endl;
			}else if(state==GLUT_UP){
			}
			break;
		default:
			break;
	};
	glutPostRedisplay();
}
void glutMotion(int x, int y){
	if(isSelected){
		cout<<"Dragging...\n";
	}
	glutPostRedisplay();
}

void timerFunc(int value){
	rotAngle=(rotAngle+2)%360;
	glutPostRedisplay();
	glutTimerFunc(timeStep, timerFunc, value+1);
}

void init(){
	aspect = (GLfloat)width / (GLfloat)height;

	cowObject=DrawObject("assets/cow.obj", 0.5);

	lights[0].lightID = GL_LIGHT0;
	lights[0].pos = V4(0, 20, -20, 1);
	lights[0].lookat = V3(0, 0, 0);
	lights[0].lookup = V3(0, 1, 1);
	lights[0].ambient = V4(0.1, 0.1, 0.1, 1);
	lights[0].diffuse = V4(0.8, 0.8, 0.8, 1);

	lights[1].lightID = GL_LIGHT1;
	lights[1].pos = V4(-20, 20, 0, 1);
	lights[1].lookat = V3(0, 0, 0);
	lights[1].lookup = V3(1, 1, 0);
	lights[1].ambient = V4(0, 0, 0, 1);
	lights[1].diffuse = V4(0.8, 0.8, 0.8, 1);

	eye = V3(0, 10, 10);
	lookat = V3(0, 0, 0);
	lookup = V3(0, 1, -1);
	
	lookup.normalize();

	isLightOn[0]=isLightOn[1]=isLightOn[2]=true;
}
void initGL(){
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("20170325_HW1");

	shadowMap = ShadowMap(width, height, 0);

	glClearColor(0, 0, 0, 0);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_CULL_FACE);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardCB);
	glutReshapeFunc(reshape);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);
	//glutTimerFunc(timeStep, timerFunc, 0);
	glutMainLoop();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	init();
	initGL();
	return 0;
}