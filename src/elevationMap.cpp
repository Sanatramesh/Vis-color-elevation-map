/*
 * elevationMap.cpp
 * 
 * Copyright 2015 Sanat <sanat@sanat-HP-Pavilion-dv6-Notebook-PC>
 * 
 *  
 */

#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <fstream>
#include <GL/gl.h>

int length = 90;
int width = 60;
float data[90][60];
float maxsal = 0, minsal = 40;
float h, w;
float rot[3]={0.0,0.0,0.0}, transx = 0, transy=0, transz = 0;
float scale = 2;

int flag = 1;
int xneg = -10, xpos = 100,
	yneg =  -5, ypos =  50,
	zneg = -55, zpos =  10;
	
enum {UP = 1, DOWN, };

int mState = UP;

int oldX = -13;
int oldY = -13;

/*
 * initRendering: Setting up glut window and view
 * 
 */ 
void initRendering() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	::w = glutGet( GLUT_WINDOW_WIDTH );
    ::h = glutGet( GLUT_WINDOW_HEIGHT );
	glViewport(0, 0, w, h);
   
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();    
    gluPerspective( 45.0, w / h, 0., 10.0 );
}

/*
 * handleResize: Setting up glut window and view after window resize
 * 
 */ 
void handleResize(int weight, int height){
	
	if (height  == 0)
		height = 1;

	::w = weight;
	::h = height;
	glViewport(0,0, weight, height);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective( 45.0, weight / height, 0, 10.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void readData(char **argv) {
	std::ifstream ifile;
	ifile.open(argv[1]);
	std::cout<<"Reading data ... "<<std::endl;
	int i,j;
	for(i=0; i < width; i++) {
		for(j=0;j < length;j++) {
			ifile >> data[j][i];
			if (data[j][i] == -999)
				continue;
			
			if (maxsal < data[j][i])
				maxsal = data[j][i];
			
			if (minsal> data[j][i])
				minsal= data[j][i];
		}
	}
}

/*
 * legend: draw color legend
 * 
 */
void legend(){
	int digit;
	float r = 0;
	float g = 0;
	float b = 1;
	float xval = 0, var= 0.1,sal= 0.1;
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(sal=minsal;sal<=maxsal;sal+=(maxsal-minsal)/10){
		g = (sal-minsal)/(maxsal-minsal);
		glColor3f(r, g, b);
		glBegin(GL_QUADS);
		glVertex3f(-0.6+xval,-1,0);
		glVertex3f(-0.6+var+xval,-1,0);
		glVertex3f(-0.6+var+xval,-1.2,0);
		glVertex3f(-0.6+xval,-1.2,0);
		glEnd();
		
		char conVal[5];
		sprintf(conVal, "%.2f", sal);
		glRasterPos3f( -0.6+xval, -1.3, 0); // location to start printing text
		for(digit=0; digit < 5; digit++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit] ); // Print a character on the screen
		
		xval+=0.1;
	}
}

/*
 * drawScene: Render the visualization(elevation map).
 * 
 */
void drawScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	// Set Camera Position and Direction
	gluLookAt(
		0., -2,5,
		0., 0., 0,
		0, 1, 0
	);
	
	int i,j;
	float tp, avg = 0.0;
	float lval = 0.3, scle = 0.5, transp = 0.3;
	
	glPushMatrix();
	glScalef(scale, scale, scale); // set scaling factor
	glRotatef(rot[0], 1, 0, 0); // set rotation about x axis
	glRotatef(rot[1], 0, 1, 0); // set rotation about y axis
	glTranslatef(transx, transy, transz); //set translation

	glPointSize(1.0); // set point size
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS); //Begin quadrilateral coordinates
	for ( i = 0; i < length - 1; i++)
		for( j = 0; j < width - 1; j++){
			
			tp = (data[i][j] - minsal)/(maxsal - minsal);
			
			if (data[i][j] == -999){
				glColor4f(0.2, 0.2, 0.2, transp);
				glVertex3f((float)i/length - 0.5, (float)j/length - 0.33, lval);
			}else{
				glColor4f(0.0, tp, 1,transp);
				glVertex3f((float)i/length- 0.5, (float)j/length- 0.33, tp*scle);
			}
			
			tp = (data[i+1][j] - minsal)/(maxsal - minsal);
			
			if (data[i+1][j] == -999){
				glColor4f(0.2, 0.2, 0.2,transp);
				glVertex3f((float)(i+1)/length- 0.5, (float)j/length- 0.33, lval);
			}else{
				glColor4f(0.0, tp, 1,transp);
				glVertex3f((float)(i+1)/length- 0.5, (float)j/length- 0.33, tp*scle);
			}
			
			tp = (data[i+1][j+1] - minsal)/(maxsal - minsal);
			
			if (data[i+1][j+1] == -999){
				glColor4f(0.2, 0.2, 0.2, transp);
				glVertex3f((float)(i+1)/length- 0.5, (float)(j+1)/length- 0.33, lval);
			}else{
				glColor4f(0.0, tp, 1,transp);
				glVertex3f((float)(i+1)/length- 0.5, (float)(j+1)/length- 0.33, tp*scle);
			}
			
			tp = (data[i][j+1] - minsal)/(maxsal - minsal);
			
			if (data[i][j+1] == -999){
				glColor4f(0.2, 0.2, 0.2, transp);
				glVertex3f((float)i/length- 0.5, (float)(j+1)/length- 0.33, lval);
			}else{
				glColor4f(0.0, tp, 1, transp);
				glVertex3f((float)i/length- 0.5, (float)(j+1)/length- 0.33, tp*scle);
			}
		}
		
	glEnd(); //End quadrilateral coordinates
	glPopMatrix();
	
	glViewport(0,0, w, h ); // create new viewport
	legend(); // draw legend
	
	glutSwapBuffers();
}

/*
 * keyboard: add keys for interaction
 */
void keyboard(unsigned char key, int x, int y){
	
	if (key == char(27) || key == 'q'){
		std::cout<<"Quiting ..."<<std::endl;
		exit(-1);
	}
	else if (key  == 'z'){
		scale += 0.1;
	}
	else if (key == 'x'){
		scale -= 0.1;
	}
	glutPostRedisplay();
}

/*
 * SpecialKeys: add special keys for interaction
 */
void SpecialKeys(int key, int x, int y){
	
	if (key == GLUT_KEY_LEFT){
		transx+=0.1;
	}
	else if (key == GLUT_KEY_UP){
		transy-=0.1;
	}
	else if (key == GLUT_KEY_RIGHT){
		transx-=0.1;
	}
	else if (key == GLUT_KEY_DOWN){
		transy+=0.1;
	}
	glutPostRedisplay();
}

/*
 * glutMotion: add mouse drag for interaction
 */
void glutMotion(int x, int y)
{
	if (mState == DOWN){
		if (oldY - y < 0){
			rot[0]+=0.5;
		}else{
			rot[0]-=0.5;
		}
		if (oldX - x < 0){
			rot[1] += 0.5;
		}else{
			rot[1] -= 0.5;
		}

		glutPostRedisplay ();
	}
	oldX = x;
	oldY = y;
}

/*
 * glutMouse: add mouse click for interaction
 * 
 */
void glutMouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
            case GLUT_RIGHT_BUTTON:
                mState = DOWN;
                oldX = x;
                oldY = y;
            break;
        }
    } else if (state == GLUT_UP)
        mState = UP;
}

/*
 * main: initialize glut window, keyboard, mouse, etc and start Mainloop
 * 
 */
int main(int argc, char **argv){
	
	//Initialize GLUT
	glutInit(&argc, argv);
	
	if (argc <= 1){
		std::cout<<"Usage: ./elevationMap <file path>"<<std::endl; 
		exit(-1);
	}
	readData(argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400); //Set the window size
	glutInitWindowPosition(50,50);
	//Create the window
	glutCreateWindow("Basic Shapes");
	initRendering(); //Initialize rendering
	//Set handler function for drawing
	glutDisplayFunc(drawScene);
	//Set handler function for window resize
	glutReshapeFunc(handleResize);
	//Set handler function for keypress
	glutKeyboardFunc(keyboard);
	//Set handler function for Special keypress
	glutSpecialFunc(SpecialKeys);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutMainLoop(); //Start the main loop.
	return 0;
}
