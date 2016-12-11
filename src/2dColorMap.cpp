/*
 * 2dColorMap.cpp
 * 
 * Copyright 2015 Sanat <sanat@sanat-HP-Pavilion-dv6-Notebook-PC>
 *  
 */

#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <fstream>
#include <GL/gl.h>
#include <math.h>

int length = 90;
int width = 60;
int h, w;

float scale = 1;
float data[90][60];
float maxsal = 0, minsal = 40;
float transx = 0, transy = 0;

/*
 * initRendering: Setting up glut window and view
 * 
 */
void initRendering() {
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	::w = glutGet( GLUT_WINDOW_WIDTH );
    ::h = glutGet( GLUT_WINDOW_HEIGHT );
	glViewport(0,0, w, h );
	gluOrtho2D(-50, 60.0, -50.0, 35.0);
}

void readData(char **argv)
{
	std::cout<<"Reading data"<<std::endl;
	std::ifstream ifile;
	ifile.open(argv[1]);
	int i,j;
	
	for( i = 0; i < width; i++)
		for( j = 0;j < length; j++)
		{
			ifile >> data[j][i];
			if (data[j][i] == -999)
				continue;
			
			if (maxsal < data[j][i])
				maxsal = data[j][i];
			
			if (minsal > data[j][i])
				minsal = data[j][i];
		}
	ifile.close();
}

/*
 * handleResize: Setting up glut window and view after window resize
 * 
 */ 
void handleResize(int weight, int height){
	
	if (height  == 0)
		height = 1;
	
	w = weight;
	h = height;
	
	glViewport(0, 0, weight, height);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluOrtho2D(-50, 60.0, -50.0, 35.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * contourLegend: draw coutour color legend
 * 
 */
void contourLegend(){
	int digit;
	float r = 0;
	float g = 0;
	float b = 0.25;
	float xval = 0, var= 4,sal= 0.1;
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(sal=minsal;sal<=maxsal;sal+=(maxsal-minsal)/5){
		r = (sal-minsal)/(maxsal-minsal);
		g = 2*(sal-minsal)/(maxsal-minsal)- 0.5;
		b = 1- (sal-minsal)/(maxsal-minsal);
		
		glColor3f(r, g, b);
		glBegin(GL_QUADS);
		glVertex3f(52,20 -xval,0);
		glVertex3f(52+ var,20 -xval,0);
		glVertex3f(52+ var,20 -xval-var,0);
		glVertex3f(52,20 -xval-var,0);
		glEnd();
		
		char conVal[5];
		sprintf(conVal, "%.2f", sal);
		glRasterPos3f( 48, 20-xval, 0); // location to start printing text
		
		for( digit = 0; digit < 5; digit++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit] ); // Print a character on the screen
		
		xval+=4;
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
	float b = 0.25;
	float xval = 0, var= 4,sal= 0.1;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(sal=minsal;sal<=maxsal;sal+=(maxsal-minsal)/10){
		g = 1 -(sal-minsal)/(maxsal-minsal);
		
		glColor3f(r, g, b);
		glBegin(GL_QUADS);
		glVertex3f(-20+xval,-42,0);
		glVertex3f(-20+var+xval,-42,0);
		glVertex3f(-20+var+xval,-46,0);
		glVertex3f(-20+xval,-46,0);
		glEnd();
		
		char conVal[5];
		sprintf(conVal, "%.2f", sal);
		glRasterPos3f( -20+xval, -42+1, 0); // location to start printing text
		for( digit=0; digit < 5; digit++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit] ); // Print a character on the screen
		
		xval+=4;
	}
}

/*
 * findContour: find contour using marching squares
 * 
 */
void findContour(int num){
	int count = 0;
	int i,j, k;
	float vertices[4][2];
	float sal, inc = (maxsal - minsal)/num, colormap;
	glBegin(GL_LINES);
	
	for(sal = minsal; sal <= maxsal; sal += inc){
		colormap = (sal-minsal)/(maxsal - minsal);
		glColor3f(colormap, 2*colormap-0.5, 1- colormap);
	
		for (i = 0;i < length - 1; i++)
			for(j = 0;j < width - 1; j++){
				count = 0;
				if ((data[i][j] >= sal && data[i][j+1] <=sal) ||
					(data[i][j+1] >= sal && data[i][j] <=sal)){
					vertices[count][0] = i;
					vertices[count][1] = (float)j+ (sal - data[i][j])/(data[i][j+1]-data[i][j]);
					count++;
				}
				if ((data[i][j] >= sal && data[i+1][j] <=sal) ||
					(data[i+1][j] >= sal && data[i][j] <=sal)){
					vertices[count][0] = (float)i + (sal - data[i][j])/(data[i+1][j]-data[i][j]);
					vertices[count][1] = j;
					count++;
				}
				if ((data[i][j+1] >= sal && data[i+1][j+1] <=sal) ||
					(data[i+1][j+1] >= sal && data[i][j+1] <=sal)){
					vertices[count][0] = (float)i+(sal - data[i][j+1])/(data[i+1][j+1]-data[i][j+1]);
					vertices[count][1] = j+1;
					count++;
				}
				if ((data[i+1][j] >= sal && data[i+1][j+1] <=sal) ||
					(data[i+1][j+1] >= sal && data[i+1][j] <=sal)){
					vertices[count][0] = i+1;
					vertices[count][1] = (float)j+(sal - data[i+1][j])/(data[i+1][j+1]-data[i+1][j]);
					count++;
				}
			if (count >=2){
				for (k = 0;k < count;k++)
				glVertex2f(vertices[k][0]-45, vertices[k][1]-30);
			}
		}
	}
	glEnd();
}

/*
 * drawScene: Render the visualization(2D color map with contours).
 * 
 */
void drawScene(){
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glPushMatrix();
	
	int i,j;
	float tp, avg = 0.0, blueval = 0.25;
	
	glPointSize(1.0);
	glLoadIdentity();
	glScalef(scale, scale, 1);
	glTranslatef(transx, transy, 0);
	glBegin(GL_QUADS); //Begin quadrilateral coordinates
	
	for (i=0;i<length-1;i++){
		for(j=0;j<width-1;j++){
			
			avg = (float)(data[i][j] + data[i+1][j] + data[i+1][j+1])/3.0;
			tp = (data[i][j] - minsal)/(maxsal - minsal);
			
			if (data[i][j] < 0)
				glColor3f(0.2, 0.2, 0.2);
			else
				glColor3f(0.0, 1-tp, blueval);
			
			glVertex2f(i-45, j-30);
			
			tp = (data[i+1][j] - minsal)/(maxsal - minsal);
			
			if (data[i+1][j] < 0)
				glColor3f(0.2, 0.2, 0.2);
			else
				glColor3f(0.0, 1-tp, blueval);
			
			glVertex2f(i+1-45, j-30);
			
			tp = (data[i+1][j+1] - minsal)/(maxsal - minsal);
			
			if (data[i+1][j+1] < 0)
				glColor3f(0.2, 0.2, 0.2);
			else
				glColor3f(0.0, 1-tp, blueval);
			
			glVertex2f(i+1-45, j+1-30);
			
			tp = (data[i][j+1] - minsal)/(maxsal - minsal);
			
			if (data[i][j+1] < 0)
				glColor3f(0.2, 0.2, 0.2);
			else
				glColor3f(0.0, 1-tp, blueval);
			
			glVertex2f(i-45, j+1-30);
		}
	}
	glEnd(); //End quadrilateral coordinates
	
	findContour(5); // find contour
	
	glPopMatrix();
	glViewport(0,0, w, h );
	legend(); // draw Legend
	contourLegend(); // draw contour legend
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
 * main: initialize glut window, keyboard, mouse, etc and start Mainloop
 */
int main(int argc, char **argv){
	
	//Initialize GLUT
	glutInit(&argc, argv);
	if (argc <= 1){
		std::cout<<argc<<"Usage: ./2dColorMap <file path>"<<std::endl; 
		exit(-1);
	}
	readData(argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400); //Set the window size
	glutInitWindowPosition(50,50);
	//Create the window
	glutCreateWindow("Color Map");
	initRendering(); //Initialize rendering
	//Set handler function for drawing
	glutDisplayFunc(drawScene);
	//Set handler function for window resize
	glutReshapeFunc(handleResize);
	//Set handler function for keypress
	glutKeyboardFunc(keyboard);
	//Set handler function for Special keypress
	glutSpecialFunc(SpecialKeys);
	//glutMouseFunc(mousekeys);
	glutMainLoop(); //Start the main loop.
	return 0; 
}
