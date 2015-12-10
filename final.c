/*
 * 
 *  Key bindings:
 *  l          Toggles lighting
 *  []         Increase or decrease light elevation
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=65;         //  Azimuth of view angle
int ph=15;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=4.5;   //  Size of world
double car1Color[3][3] = {{.7,0,0},{0,0,.7},{0,.7,0}}; //car 1 color
double car2Color[4][3] = {{.7,0,.7},{0,.7,.7},{.7,.7,0},{.3,.3,.3}}; //car 2 color
int car1; //which color to use for car 1
int car2; //which color to use for car 2
//Texture names
unsigned int starTexture;
unsigned int pyramidTexture;
unsigned int wheelFrontTexture;
unsigned int wheelEdgeTexture;
unsigned int nightTexture;
unsigned int roadTexture;
unsigned int buildingTexture;
unsigned int buildingTexture1;
unsigned int buildingTexture2;
unsigned int buildingTexture3;
unsigned int sidewalkTexture;
unsigned int trashTexture;
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[]={1}; // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light


//Draw Sidewalk
static void Sidewalk(double x, double y, double z, double dx, double dy, double dz, double th,double rotate)
{
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,1,0,0);
	glRotated(rotate,0,1,0);
	glScaled(dx,dy,dz);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE ,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,sidewalkTexture);

	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glColor3f(.6,.6,.6);
	glVertex3f(0,0,-.2);
	glVertex3f(0,0,0);
	glVertex3f(0,10,0);
	glVertex3f(0,10,-.2);

	glNormal3f(0,0,1);
	glTexCoord3f(1,20,0); glVertex3f(.5,10,0);
	glTexCoord3f(0,20,0); glVertex3f(0,10,0);
	glTexCoord3f(0,0,0);  glVertex3f(0,0,0);
	glTexCoord3f(1,0,0);  glVertex3f(.5,0,0);

	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);


}

//Draw Road
static void Road(double x, double y, double z, double dx, double dy, double dz, double th)
{
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,1,0,0);
	glScaled(dx,dy,dz);
	int wallx,wally;

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE ,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,roadTexture);
	glBegin(GL_QUADS);

	float unitx = 1.0f/10;
	float unity = 1.0f/20;
	for(wallx=0;wallx<10;wallx++)
	{
		for(wally=0;wally<100;wally++)
		{
			glTexCoord2f( unitx * (float) wallx, unity * (float) wally );
			glVertex3f( -1 + wallx*2,-1+ wally*2,0 );    
			glTexCoord2f( unitx * (float) (wallx+1), unity * (float) wally );
			glVertex3f(  1+ wallx*2,-1+ wally*2,0 );             
			glTexCoord2f( unitx * (float) (wallx+1), unity * (float) (wally+1) );
			glVertex3f(  1+ wallx*2, 1+ wally*2,0 );             
			glTexCoord2f( unitx * (float) wallx, unity * (float) (wally+1) );
			glVertex3f(  -1+ wallx*2, 1+ wally*2,0 );           

		}
	}
	/*
	   glBegin(GL_QUADS);
	   glNormal3f(0,0,-1);
	   for (j=-10; j <= 9; j++){
	   for (i=0; i<=99; i++){

	   glTexCoord2f((j+10)/20,i/100);	
	   glVertex2f(j,i);
	   glTexCoord2f((j+11)/20,i/100);
	   glVertex2f(j+1,i);
	   glTexCoord2f((j+11)/20,(i+1)/100);
	   glVertex2f(j+1,i+1);
	   glTexCoord2f((j+10)/20,(i+1)/100);
	   glVertex2f(j,i+1);
	   }
	   }*/
	/*glTexCoord2f(0,0); glVertex2f(-10,10);
	  glTexCoord2f(0,10); glVertex2f(-10,-100);
	  glTexCoord2f(1,10); glVertex2f(10,-100);
	  glTexCoord2f(1,0); glVertex2f(10,10);*/
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

static void Building(double x, double y, double z, double dx, double dy, double dz, double th, double color[3], int which)
{
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE ,GL_MODULATE);
	if(which == 0)
		glBindTexture(GL_TEXTURE_2D,buildingTexture);
	else if (which == 1)
		glBindTexture(GL_TEXTURE_2D, buildingTexture1);
	else if (which == 2)
		glBindTexture(GL_TEXTURE_2D, buildingTexture2);
	else if (which == 3)
		glBindTexture(GL_TEXTURE_2D, buildingTexture3);

	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glColor3f(color[0],color[1],color[2]);
	glTexCoord3f(0,0,1); glVertex3f(0,0,1);
	glTexCoord3f(0,1,1); glVertex3f(0,8,1);
	glTexCoord3f(0,1,0); glVertex3f(0,8,-1);
	glTexCoord3f(0,0,0); glVertex3f(0,0,-1);

	glNormal3f(0,0,1);
	glTexCoord3f(0,0,0); glVertex3f(0,0,1);
	glTexCoord3f(0,1,0); glVertex3f(0,8,1);
	glTexCoord3f(1,1,0); glVertex3f(2,8,1);
	glTexCoord3f(1,0,0); glVertex3f(2,0,1);

	glNormal3f(0,0,-1);
	glTexCoord3f(1,0,0); glVertex3f(0,0,-1);
	glTexCoord3f(1,1,0); glVertex3f(0,8,-1);
	glTexCoord3f(0,1,0); glVertex3f(2,8,-1);
	glTexCoord3f(0,0,0); glVertex3f(2,0,-1);

	glNormal3f(1,0,0);
	glTexCoord3f(0,0,0); glVertex3f(2,0,1);
	glTexCoord3f(0,1,0); glVertex3f(2,8,1);
	glTexCoord3f(0,1,1); glVertex3f(2,8,-1);
	glTexCoord3f(0,0,1); glVertex3f(2,0,-1);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glVertex3f(2,8,1);
	glVertex3f(2,8,-1);
	glVertex3f(0,8,-1);
	glVertex3f(0,8,1);
	glEnd();
	glPopMatrix();

}




//Draw trashcan
static void Trash(double x, double y, double z, double dx, double dy, double dz, double th)
{
	int k;

	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);

	glRotated(90,1,0,0);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE ,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,trashTexture);

	//bottom
	glNormal3f(0,0,-1);
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(.5,.5);
	glVertex3f(0,0,1);
	for (k=0;k<=360;k+=10){
		glTexCoord2f(.5*Cos(k)+.5,.5*Sin(k)+.5);
		glVertex3f(Cos(k),Sin(k),1);
	}

	glEnd();

	//Edge
	glBegin(GL_QUAD_STRIP);
	for (k=0;k<=360;k+=10){
		glNormal3f(Cos(k),Sin(k),0);
		glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),1);
		glTexCoord2f(1,.5*k); glVertex3f(Cos(k),Sin(k),-1);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
	double x = Sin(th)*Cos(ph);
	double y = Cos(th)*Cos(ph);
	double z =         Sin(ph);
	//  For a sphere at the origin, the position
	//  and normal vectors are the same
	glNormal3d(x,y,z);
	glVertex3d(x,y,z);
}

/*
 *  *  Draw a ball
 *   *     at (x,y,z)
 *    *     radius (r)
 *     */
static void ball(double x,double y,double z,double r)
{
	int th,ph;
	float yellow[] = {1.0,1.0,0.0,1.0};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	// Save transformation
	glPushMatrix();
	//        //  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(r,r,r);
	glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
	//  Bands of latitude
	for (ph=-90;ph<90;ph+=inc)
	{
		glBegin(GL_QUAD_STRIP);
		for (th=0;th<=360;th+=2*inc)
		{
			Vertex(th,ph);
			Vertex(th,ph+inc);
		}
		glEnd();
	}
	//  Undo transofrmations
	glPopMatrix();
}

static void Wheel(double x, double y, double z, double dx, double dy, double dz, double th,double zh)
{
	int i,k;
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glRotated(zh,0,0,-1);
	glScaled(dx,dy,dz);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE ,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,wheelFrontTexture);

	glColor3f(1,1,1);
	//front and back	
	for (i=1;i>=-1;i-=2){
		glNormal3f(0,0,i);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(.5,.5);
		glVertex3f(0,0,i);
		for (k=0;k<=360;k+=10){
			glTexCoord2f(0.5*Cos(k)+.5,.5*Sin(k)+.5);
			glVertex3f(i*Cos(k),Sin(k),i);
		}

		glEnd();
	}
	//Edge
	glBindTexture(GL_TEXTURE_2D,wheelEdgeTexture);
	glBegin(GL_QUAD_STRIP);
	for (k=0;k<=360;k+=10){
		glNormal3f(Cos(k),Sin(k),0);
		glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),1);
		glTexCoord2f(1,.5*k); glVertex3f(Cos(k),Sin(k),-1);
	}


	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

static void Cylinder(double x, double y, double z, double dx, double dy, double dz, double th,double xh)
{

	int i,k;
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,1,0,0);
	glRotated(xh,0,1,0);
	glScaled(dx,dy,dz);

	//front and back	
	for (i=1;i>=-1;i-=2){
		glNormal3f(0,0,i);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(.5,.5);
		glVertex3f(0,0,i);
		for (k=0;k<=360;k+=10){
			glTexCoord2f(0.5*Cos(k)+.5,.5*Sin(k)+.5);
			glVertex3f(i*Cos(k),Sin(k),i);
		}

		glEnd();
	}
	//Edge
	glBegin(GL_QUAD_STRIP);
	for (k=0;k<=360;k+=10){
		glNormal3f(Cos(k),Sin(k),0);
		glTexCoord2f(0,0.5*k); glVertex3f(Cos(k),Sin(k),1);
		glTexCoord2f(1,.5*k); glVertex3f(Cos(k),Sin(k),-1);
	}
	glEnd();
	glPopMatrix();

}
/* 
 *  *  Draw sky box
 *   */
static void Sky(double D)
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);

	//  Sides
	glBindTexture(GL_TEXTURE_2D,nightTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.00,.222); glVertex3f(-D,-D,-D);
	glTexCoord2f(0.25,.222); glVertex3f(+D,-D,-D);
	glTexCoord2f(0.25,.666); glVertex3f(+D,+D,-D);
	glTexCoord2f(0.00,.666); glVertex3f(-D,+D,-D);

	glTexCoord2f(0.25,.222); glVertex3f(+D,-D,-D);
	glTexCoord2f(0.50,.222); glVertex3f(+D,-D,+D);
	glTexCoord2f(0.50,.666); glVertex3f(+D,+D,+D);
	glTexCoord2f(0.25,.666); glVertex3f(+D,+D,-D);

	glTexCoord2f(0.50,.222); glVertex3f(+D,-D,+D);
	glTexCoord2f(0.75,.222); glVertex3f(-D,-D,+D);
	glTexCoord2f(0.75,.666); glVertex3f(-D,+D,+D);
	glTexCoord2f(0.50,.666); glVertex3f(+D,+D,+D);

	glTexCoord2f(0.75,.222); glVertex3f(-D,-D,+D);
	glTexCoord2f(1.00,.222); glVertex3f(-D,-D,-D);
	glTexCoord2f(1.00,.666); glVertex3f(-D,+D,-D);
	glTexCoord2f(0.75,.666); glVertex3f(-D,+D,+D);
	glEnd();

	//  Top and bottom
	glBindTexture(GL_TEXTURE_2D,nightTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.25,.666); glVertex3f(+D,+D,-D);
	glTexCoord2f(0.5,.666); glVertex3f(+D,+D,+D);
	glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
	glTexCoord2f(0.25,1); glVertex3f(-D,+D,-D);

	/*glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
	  glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
	  glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
	  glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);*/
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


static void Car(double x, double y, double z, double dx, double dy, double dz, double th, double rot,double color[3]){	

	int k;	
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);

	glNormal3f(1,0,0);
	//Cylinder(0,0,0,.1,2,16,0);
	glBegin(GL_QUADS);
	//Front Bumper
	glNormal3f(0,1,0);
	glColor3f(.2,.2,.2);
	glVertex3f(-.3,.5,0);//front bumper coming off car
	glVertex3f(16.3,.5,0);
	glVertex3f(16.1,.5,-1.6);
	glVertex3f(-.1,.5,-1.6);

	glNormal3f(0,-.1,-1);
	glVertex3f(-.1,.5,-1.6);//first downslop in bumper (mostly y)
	glVertex3f(16.1,.5,-1.6);
	glVertex3f(16.1,-.1,-1.4);
	glVertex3f(-.1,-.1,-1.4);

	glNormal3f(0,-1,-1);
	glVertex3f(-.1,-.1,-1.4);
	glVertex3f(16.1,-.1,-1.4);//second downslope 45 deg
	glVertex3f(16.1,-1.4,0);
	glVertex3f(-.1,-1.4,0);

	glNormal3f(0,-1,0); //in shadow
	glVertex3f(-.1,-1.4,0); //straight down bottom of front bumper
	glVertex3f(16.1,-1.4,0);
	glVertex3f(16.1,-1.7,0);
	glVertex3f(-.1,-1.7,0);

	//Front
	glNormal3f(0,0,-1);
	glColor3f(color[0],color[1],color[2]);
	glVertex3f(0,.5,0);
	glVertex3f(0,3.5,.1);//front quad (lights go here)
	glVertex3f(16,3.5,.1);
	glVertex3f(16,.5,0);

	glNormal3f(0,1,.04);	
	glVertex3f(0,3.5,.1);
	glVertex3f(16,3.5,.1);//top quad of front
	glVertex3f(16,4.2,17);
	glVertex3f(0,4.2,17);
	glEnd();


	//Right Side
	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
	glVertex3f(16,.5,0); //Quad to wheel from front
	glVertex3f(16,3.5,0);
	glVertex3f(16,3.733,5.666);
	glVertex3f(16,.5,5.666);
	glEnd();


	//Makes the front wheel space
	glBegin(GL_QUAD_STRIP);
	for (k=0;k<=180;k+=10){
		glNormal3f(0,-Sin(k),-Cos(k));
		glVertex3f(16.2,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
		glVertex3f(14,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();	

	//Next 3 fans cover some of the space
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(1,0,0);
	glVertex3f(16,3.7,5);
	for(k=90;k<=190;k+=10){
		glVertex3f(16,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(16,3.8,9);
	for(k=45;k<=135;k+=10){
		glVertex3f(16,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(16,3.9,13);
	for(k=0;k<=90;k+=10){
		glVertex3f(16,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
	glVertex3f(16.3,.5,5.666);//covers small space between wheel well and car
	glVertex3f(16.1,-1.4,5.666);
	glVertex3f(16.2,-1.4,6.1);
	glVertex3f(16.2,1.5,6.1);

	glNormal3f(1,0,0);
	glVertex3f(16.0,3.5,.1);
	glVertex3f(16.0,2.5,.1);
	glVertex3f(16.0,2.5,17);//covers triangle fans
	glVertex3f(16.0,4.2,17);

	glNormal3f(1,0,0);
	glVertex3f(16,4,13);
	glVertex3f(16,-1.5,13); //big right side through door
	glVertex3f(16,-1.5,33);
	glVertex3f(16,4,33);

	glNormal3f(1,0,0);
	glVertex3f(16,2.5,33); //covers triangle fans below for back wheel
	glVertex3f(16,4,33);
	glVertex3f(16,4,40);
	glVertex3f(16,2.5,40);
	glEnd();


	//Back right wheel well
	glBegin(GL_QUAD_STRIP);
	for (k=0;k<=180;k+=10){
		glNormal3f(16,-Sin(k),-Cos(k));
		glVertex3f(16.2,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
		glVertex3f(16,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	glNormal3f(1,0,0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(16,3.7,32);
	for(k=90;k<=190;k+=10){
		glVertex3f(16,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(16,3.8,36);
	for(k=45;k<=135;k+=10){
		glVertex3f(16,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(16,3.9,40);
	for(k=0;k<=90;k+=10){
		glVertex3f(16,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	//Back right side
	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
	glVertex3f(16,4,40);
	glVertex3f(16,-1,40);
	glVertex3f(15,-1,46);
	glVertex3f(15,4,46);
	glEnd();

	//Left Side
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glVertex3f(0,.5,0); //Quad to wheel from front
	glVertex3f(0,3.5,0);
	glVertex3f(0,3.733,5.666);
	glVertex3f(0,.5,5.666);
	glEnd();

	//Makes the front wheel space
	glBegin(GL_QUAD_STRIP);
	for (k=0;k<=180;k+=10){
		glNormal3f(0,-Sin(k),-Cos(k));
		glVertex3f(-.2,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
		glVertex3f(2,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();	

	//Next 3 fans cover some of the space
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(-1,0,0);
	glVertex3f(0,3.7,5);
	for(k=90;k<=190;k+=10){
		glVertex3f(0,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,3.8,9);
	for(k=45;k<=135;k+=10){
		glVertex3f(0,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,3.9,13);
	for(k=0;k<=90;k+=10){
		glVertex3f(0,3.5*Sin(k)-1,3.5*Cos(k)+9.5);
	}
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glVertex3f(-.3,.5,5.666);//covers small space between wheel well and car
	glVertex3f(-.1,-1.4,5.666);
	glVertex3f(-.2,-1.4,6.1);
	glVertex3f(-.2,1.5,6.1);

	glNormal3f(-1,0,0);
	glVertex3f(0,3.5,.1);
	glVertex3f(0,2.5,.1);
	glVertex3f(0,2.5,17);//covers triangle fans
	glVertex3f(0,4.2,17);

	glNormal3f(-1,0,0);
	glVertex3f(0,4,13);
	glVertex3f(0,-1.5,13); //big left side through door
	glVertex3f(0,-1.5,33);
	glVertex3f(0,4,33);

	glNormal3f(-1,0,0);
	glVertex3f(0,2.5,33); //covers triangle fans below for back wheel
	glVertex3f(0,4,33);
	glVertex3f(0,4,40);
	glVertex3f(0,2.5,40);
	glEnd();

	//Back left wheel well
	glBegin(GL_QUAD_STRIP);
	for (k=0;k<=180;k+=10){
		glNormal3f(0,-Sin(k),-Cos(k));
		glVertex3f(-.2,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
		glVertex3f(2,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);

	glNormal3f(-1,0,0);
	glVertex3f(0,3.7,32);
	for(k=90;k<=190;k+=10){
		glVertex3f(0,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,3.8,36);
	for(k=45;k<=135;k+=10){
		glVertex3f(0,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,3.9,40);
	for(k=0;k<=90;k+=10){
		glVertex3f(0,3.5*Sin(k)-1,3.5*Cos(k)+36.5);
	}
	glEnd();

	//Back left side
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glVertex3f(0,4,40);
	glVertex3f(0,-1,40);
	glVertex3f(1,-1,46);
	glVertex3f(1,4,46);
	glEnd();


	//Right Bumper Front
	glColor3f(.2,.2,.2);
	glBegin(GL_TRIANGLES);
	glNormal3f(1,-.1,-.1);
	glVertex3f(16.1,.5,-1.6);//fills in gap at front of side bumper
	glVertex3f(16.1,-.1,-1.4);
	glVertex3f(16.12,.5,-1.4);

	glNormal3f(1,.1,0);
	glVertex3f(16.1,-1.7,0);//low part of front side bumper
	glVertex3f(16.1,-1.4,0);
	glVertex3f(16.1,-1.4,5.666);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(1,.1,0);
	glVertex3f(16.1,-1.4,0);
	glVertex3f(16.1,-.1,-1.4);//connect front bumer to side
	glVertex3f(16.12,.5,-1.4);
	glVertex3f(16.3,.5,0);

	glVertex3f(16.3,.5,0);
	glVertex3f(16.3,.5,5.666);//connect side bumper to wheel
	glVertex3f(16.1,-1.4,5.666);
	glVertex3f(16.1,-1.4,0);

	glNormal3f(0,1,0);
	glVertex3f(16.3,.5,0);
	glVertex3f(16.3,.5,5.666);//connects top of bumper to car 
	glVertex3f(16,.5,5.666);
	glVertex3f(16,.5,0);

	//right Middle low bumper
	glNormal3f(1,0,0);
	glVertex3f(16.1,-1.4,13);
	glVertex3f(16.1,-1.4,33);
	glVertex3f(16.1,-.8,33);
	glVertex3f(16.1,-.8,13);


	//right back bumper
	glNormal3f(1,0,0);
	glVertex3f(16.5,-.2,40);
	glVertex3f(16.5,-1.8,40);
	glVertex3f(15.5,-1.8,46.5);
	glVertex3f(15.5,-.2,46.5);

	glNormal3f(0,1,0);
	glVertex3f(15.5,-.2,46.5);
	glVertex3f(15,-.2,46.5);
	glVertex3f(16,-.2,40);
	glVertex3f(16.5,-.2,40);
	glEnd();


	//Right Top
	glBegin(GL_QUADS);
	glNormal3f(1,.7,0);
	glVertex3f(16,4,17); //front right bar connecting windows
	glVertex3f(16,4,18);
	glNormal3f(0,.7,-1);
	glVertex3f(14.5,8,22);
	glVertex3f(14,8,22);


	glNormal3f(0,.7,1);	
	glVertex3f(14.5,8,34); //back right bar connecting windows
	glVertex3f(14,8,34);
	glNormal3f(1,0,0);
	glVertex3f(16,4,39);
	glVertex3f(16,4,38);


	glNormal3f(1,.2,0);
	glVertex3f(14.5,8,31);//window seperator
	glVertex3f(14.5,8,30.4);
	glVertex3f(16,4,30.4);
	glVertex3f(16,4,31);
	glEnd();


	//Left Bumper Front
	glColor3f(.2,.2,.2);
	glBegin(GL_TRIANGLES);
	glNormal3f(-1,-.1,-.1);
	glVertex3f(-.1,.5,-1.6);//fills in gap at front of side bumper
	glVertex3f(-.1,-.1,-1.4);
	glVertex3f(-.12,.5,-1.4);

	glNormal3f(-1,.1,0);
	glVertex3f(-.1,-1.7,0);//low part of front side bumper
	glVertex3f(-.1,-1.4,0);
	glVertex3f(-.1,-1.4,5.666);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1,.1,0);
	glVertex3f(-.1,-1.4,0);
	glVertex3f(-.1,-.1,-1.4);//connect front bumer to side
	glVertex3f(-.12,.5,-1.4);
	glVertex3f(-.3,.5,0);

	glVertex3f(-.3,.5,0);
	glVertex3f(-.3,.5,5.666);//connect side bumper to wheel
	glVertex3f(-.1,-1.4,5.666);
	glVertex3f(-.1,-1.4,0);

	glNormal3f(0,1,0);
	glVertex3f(-.3,.5,0);
	glVertex3f(-.3,.5,5.666);//connects top of bumper to car 
	glVertex3f(0,.5,5.666);
	glVertex3f(0,.5,0);


	//Left Middle low bumper
	glNormal3f(-1,0,0);
	glVertex3f(-.1,-1.4,13);
	glVertex3f(-.1,-1.4,33);
	glVertex3f(-.1,-.8,33);
	glVertex3f(-.1,-.8,13);


	//Left back bumper
	glNormal3f(-1,0,0);
	glVertex3f(-.5,-.2,40);
	glVertex3f(-.5,-1.8,40);
	glVertex3f(.5,-1.8,46.5);
	glVertex3f(.5,-.2,46.5);

	glNormal3f(0,-1,0);
	glVertex3f(.5,-.2,46.5);
	glVertex3f(1,-.2,46.5);
	glVertex3f(0,-.2,40);
	glVertex3f(-.5,-.2,40);
	glEnd();


	//Left Top
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glVertex3f(0,4,17); //front left bar connecting windows
	glVertex3f(0,4,18);
	glVertex3f(1.5,8,22);
	glVertex3f(2,8,22);

	glNormal3f(0,.7,1);
	glVertex3f(1.5,8,34); //back left bar connecting windows
	glVertex3f(2,8,34);
	glNormal3f(-1,.7,0);
	glVertex3f(0,4,39);
	glVertex3f(0,4,38);

	glNormal3f(-1,.2,0);
	glVertex3f(1.5,8,31);//window seperator
	glVertex3f(1.5,8,30.4);
	glVertex3f(0,4,30.4);
	glVertex3f(0,4,31);

	//Top
	glNormal3f(0,1,0);
	glVertex3f(14.5,8,22);//main top quad on bottom
	glVertex3f(1.5,8,22);
	glVertex3f(1.5,8,34);
	glVertex3f(14.5,8,34);

	glNormal3f(-.6,1,0);
	glVertex3f(1.5,8,22);//left top
	glVertex3f(1.5,8,34);
	glVertex3f(8,9,33);
	glVertex3f(8,9,23);

	glNormal3f(.6,1,0);
	glVertex3f(8,9,23);
	glVertex3f(8,9,33);//right top
	glVertex3f(14.5,8,34);
	glVertex3f(14.5,8,22);

	glEnd();

	glNormal3f(0,.3,-1);
	glBegin(GL_TRIANGLES);
	glVertex3f(14.5,8,22);
	glVertex3f(8,9,23);//front top
	glVertex3f(1.5,8,22);


	glVertex3f(14.5,8,34);
	glVertex3f(8,8,33);//back top
	glVertex3f(1.5,8,34);

	glEnd();

	//Back
	glBegin(GL_QUADS);
	glColor3f(color[0],color[1],color[2]);
	glNormal3f(0,1,0);
	glVertex3f(0,4,39);
	glVertex3f(16,4,39);
	glVertex3f(15,4,46);//top back quad
	glVertex3f(1,4,46);

	glNormal3f(0,0,1);
	glVertex3f(1,4,46);//back back quad
	glVertex3f(1,-1,46);
	glVertex3f(15,-1,46);
	glVertex3f(15,4,46);

	//Back lights
	glColor3f(1,.1,.1);
	glNormal3f(0,0,1);
	glVertex3f(2,2.75,46.01);
	glVertex3f(2,.75,46.01);
	glVertex3f(6,.75,46.01);//left light
	glVertex3f(6,2.75,46.01);

	glVertex3f(14,2.75,46.01);
	glVertex3f(14,.75,46.01);
	glVertex3f(10,.75,46.01);//right light
	glVertex3f(10,2.75,46.01);

	//Back bumper

	glColor3f(.2,.2,.2);
	glNormal3f(0,0,1);
	glVertex3f(.5,-1.8,46.5);
	glVertex3f(.5,-.2,46.5);
	glVertex3f(15.5,-.2,46.5);
	glVertex3f(15.5,-1.8,46.5);

	glNormal3f(0,1,0);
	glVertex3f(15.5,-.2,46.5);
	glVertex3f(15.5,-.2,46);
	glVertex3f(.5,-.2,46);
	glVertex3f(.5,-.2,46.5);
	//Left Door
	glColor3f(0,0,0);
	glNormal3f(-1,0,0);
	glVertex3f(0,4,17.5);
	glVertex3f(0,4,17.6);//front side
	glVertex3f(-.1,-.8,17.6);
	glVertex3f(-.1,-.8,17.5);

	glVertex3f(-.1,-.8,17.6);
	glVertex3f(-.1,-.7,17.6);
	glVertex3f(-.1,-.7,30.6);//bottom
	glVertex3f(-.1,-.8,30.6);

	glVertex3f(-.1,-.8,30.7);//back
	glVertex3f(-.1,-.8,30.6);
	glVertex3f(0,4,30.6);
	glVertex3f(0,4,30.7);

	glVertex3f(-.05,2.7,26.5);
	glVertex3f(-.05,3.2,26.5);//handle
	glVertex3f(-.05,3.2,28.5);
	glVertex3f(-.05,2.7,28.5);

	//Right Door
	glColor3f(0,0,0);
	glNormal3f(1,0,0);
	glVertex3f(0,4,17.5);
	glVertex3f(0,4,17.6);//front side
	glVertex3f(-.1,-.8,17.6);
	glVertex3f(-.1,-.8,17.5);

	glVertex3f(-.1,-.8,17.6);
	glVertex3f(-.1,-.7,17.6);
	glVertex3f(-.1,-.7,30.6);//bottom
	glVertex3f(-.1,-.8,30.6);

	glVertex3f(-.1,-.8,30.7);//back
	glVertex3f(-.1,-.8,30.6);
	glVertex3f(0,4,30.6);
	glVertex3f(0,4,30.7);

	glVertex3f(-.05,2.7,26.5);
	glVertex3f(-.05,3.2,26.5);//handle
	glVertex3f(-.05,3.2,28.5);
	glVertex3f(-.05,2.7,28.5);

	//Grill
	glColor3f(0,0,0);
	glNormal3f(0,0,-1);
	glVertex3f(4,.7,0);
	glVertex3f(12,.7,0);
	glVertex3f(12,2.7,0);//background black
	glVertex3f(4,2.7,0);

	//Grill grate (vertical)
	glColor3f(.7,.7,.7);
	glVertex3f(4,.7,-0.1);
	glVertex3f(4,2.7,-.1);
	glVertex3f(4.32,2.7,-0.1);
	glVertex3f(4.32,.7,-0.1);

	glVertex3f(4.64,.7,-0.1);
	glVertex3f(4.64,2.7,-.1);
	glVertex3f(4.96,2.7,-0.1);
	glVertex3f(4.96,.7,-0.1);

	glVertex3f(5.28,.7,-0.1);
	glVertex3f(5.28,2.7,-.1);
	glVertex3f(5.6,2.7,-0.1);
	glVertex3f(5.6,.7,-0.1);

	glVertex3f(5.92,.7,-0.1);
	glVertex3f(5.92,2.7,-.1);
	glVertex3f(6.24,2.7,-0.1);
	glVertex3f(6.24,.7,-0.1);

	glVertex3f(6.56,.7,-0.1);
	glVertex3f(6.56,2.7,-.1);
	glVertex3f(6.88,2.7,-0.1);
	glVertex3f(6.88,.7,-0.1);

	glVertex3f(7.20,.7,-0.1);
	glVertex3f(7.20,2.7,-.1);
	glVertex3f(7.52,2.7,-0.1);
	glVertex3f(7.52,.7,-0.1);

	glVertex3f(7.84,.7,-0.1);
	glVertex3f(7.84,2.7,-.1);
	glVertex3f(8.16,2.7,-0.1);
	glVertex3f(8.16,.7,-0.1);

	glVertex3f(8.48,.7,-0.1);
	glVertex3f(8.48,2.7,-.1);
	glVertex3f(8.8,2.7,-0.1);
	glVertex3f(8.8,.7,-0.1);

	glVertex3f(9.12,.7,-0.1);
	glVertex3f(9.12,2.7,-.1);
	glVertex3f(9.44,2.7,-0.1);
	glVertex3f(9.44,.7,-0.1);

	glVertex3f(9.76,.7,-0.1);
	glVertex3f(9.76,2.7,-.1);
	glVertex3f(10.08,2.7,-0.1);
	glVertex3f(10.08,.7,-0.1);

	glVertex3f(10.4,.7,-0.1);
	glVertex3f(10.4,2.7,-.1);
	glVertex3f(10.72,2.7,-0.1);
	glVertex3f(10.72,.7,-0.1);

	glVertex3f(11.04,.7,-0.1);
	glVertex3f(11.04,2.7,-.1);
	glVertex3f(11.36,2.7,-0.1);
	glVertex3f(11.36,.7,-0.1);

	glVertex3f(11.68,.7,-0.1);
	glVertex3f(11.68,2.7,-.1);
	glVertex3f(12,2.7,-0.1);
	glVertex3f(12,.7,-0.1);

	glVertex3f(12,.7,-.1);
	glVertex3f(12,.8,-.1);//bottom of grill
	glVertex3f(4,.8,-.1);
	glVertex3f(4,.7,-.1);

	glVertex3f(12,2.7,-.1);//top of grill
	glVertex3f(12,2.6,-.1);
	glVertex3f(4,2.6,-.1);
	glVertex3f(4,2.7,-.1);
	glEnd();

	//Lights	
	glColor3f(1,1,1);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(.5,.5); //Right light
	glVertex3f(14,1.75,.01);
	for (k=0;k<=360;k+=10){
		glVertex3f(Cos(k)+14,Sin(k)+1.8,-.01);
	}
	glEnd();


	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(.5,.5); //left light
	glVertex3f(2,1.75,.01);
	for (k=0;k<=360;k+=10){
		glVertex3f(Cos(k)+2,Sin(k)+1.8,-.01);
	}
	glEnd();
	//Right front window
	glBegin(GL_QUADS);
	glColor3f(.1,.1,.1);
	glNormal3f(1,.2,0);
	glVertex3f(16,4,18);
	glVertex3f(14.5,8,22);
	glVertex3f(14.5,8,30.4);
	glVertex3f(16,4,30.4);

	//Back Right window
	glVertex3f(16,4,38);
	glVertex3f(14.5,8,34);
	glVertex3f(14.5,8,31);
	glVertex3f(16,4,31);

	//Left front window
	glColor3f(.1,.1,.1);
	glNormal3f(-1,.2,0);
	glVertex3f(0,4,18);
	glVertex3f(1.5,8,22);
	glVertex3f(1.5,8,30.4);
	glVertex3f(0,4,30.4);

	//Back Left window
	glVertex3f(0,4,38);
	glVertex3f(1.5,8,34);
	glVertex3f(1.5,8,31);
	glVertex3f(0,4,31);

	//Front window

	glNormal3f(0,1,-1);
	glVertex3f(16,4,17);
	glVertex3f(14,8,22);
	glVertex3f(2,8,22);
	glVertex3f(0,4,17);

	//Back window	
	glNormal3f(0,1,1);
	glVertex3f(14.5,8,34);
	glVertex3f(16,4,39);
	glVertex3f(0,4,39);
	glVertex3f(1.5,8,34);;

	glEnd();

	Wheel(15.0,-1,9.5,3.05,3.05,1,90,2*zh);
	Wheel(15.0,-1,36.5,3.05,3.05,1,90,2*zh);

	Wheel(1.0,-1,9.5,3.05,3.05,1,90,2*zh);
	Wheel(1.0,-1,36.5,3.05,3.05,1,90,2*zh);

	glPopMatrix();


}




static void LightPostCone(double x, double y, double z, double dx, double dy, double dz, double th)
{

	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,1,0,0);
	glRotated(10,0,1,0);
	glScaled(dx,dy,dz);
	glutSolidCone(3,3,10,10);
	glPopMatrix();	

}


static void Lightpost(double x, double y, double z, double dx, double dy, double dz, double th, int whichLight)
{

	int k;
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);

	glColor3f(.1,.1,.1);
	for (k=0;k<=160;k+=1){
		Cylinder(Cos(k),2+Sin(k),0,.1,.1,.1,90,0);
	}

	glColor3f(.47,.23,.08);
	Cylinder(1,0,0,.2,.2,2,90,0);

	glColor3f(.1,.1,.1);
	LightPostCone(-1,2,0,.2,.2,.2,270);

	//  Light switch
	//  Translate intensity to color vectors
	float Ambient[]   = {0.02*ambient ,0.02*ambient ,0.02*ambient ,1.0};
	float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
	float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	//  Light position
	float Position[]  = {-1,2,0,.2};
	float yellow[] = {1.0,1.0,0.0,1.0};
	float Direction[] = {0,-1,0,0};
	//  Draw light position as ball (still no lighting here)
	glColor3f(1,1,1);

	//  Location of viewer for specular calculations
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
	//  glColor sets ambient and diffuse color materials
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//  Set specular colors
	glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
	glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
	//  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
	//  Enable lighting
	glEnable(GL_LIGHTING); 
	switch(whichLight){
		case 1:
			//  Enable light 0
			glEnable(GL_LIGHT1);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT1,GL_POSITION,Position);
			//  Set spotlight parameters
			glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,Direction);
			glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,20);
			glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,3);
			//  Set attenuation
			glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION ,100/100.0);
			glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION   ,0/100.0);
			glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0/100.0);
  			break;
		case 2:
			//  Enable light 2
			glEnable(GL_LIGHT2);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT2,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT2,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT2,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT2,GL_POSITION,Position);
			//  Set spotlight parameters
			glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,Direction);
			glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,20);
			glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,3);
			//  Set attenuation
			glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION ,100/100.0);
			glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION   ,0/100.0);
			glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0/100.0);
  			break;

		case 3:
			//  Enable light 0
			glEnable(GL_LIGHT3);
			///  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT3,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT3,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT3,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT3,GL_POSITION,Position);
			//  Set spotlight parameters
			glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,Direction);
			glLightf(GL_LIGHT3,GL_SPOT_CUTOFF,20);
			glLightf(GL_LIGHT3,GL_SPOT_EXPONENT,3);
			//  Set attenuation
			glLightf(GL_LIGHT3,GL_CONSTANT_ATTENUATION ,100/100.0);
			glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION   ,0/100.0);
			glLightf(GL_LIGHT3,GL_QUADRATIC_ATTENUATION,0/100.0);
  			break;

		case 4:
			//  Enable light 0
			glEnable(GL_LIGHT4);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT4,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT4,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT4,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT4,GL_POSITION,Position);
			//  Set spotlight parameters
			glLightfv(GL_LIGHT4,GL_SPOT_DIRECTION,Direction);
			glLightf(GL_LIGHT4,GL_SPOT_CUTOFF,20);
			glLightf(GL_LIGHT4,GL_SPOT_EXPONENT,3);
			//  Set attenuation
			glLightf(GL_LIGHT4,GL_CONSTANT_ATTENUATION ,100/100.0);
			glLightf(GL_LIGHT4,GL_LINEAR_ATTENUATION   ,0/100.0);
			glLightf(GL_LIGHT4,GL_QUADRATIC_ATTENUATION,0/100.0);
  			break;

		case 5:
			//  Enable light 0
			glEnable(GL_LIGHT5);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT5,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT5,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT5,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT5,GL_POSITION,Position);
			//  Set spotlight parameters
			glLightfv(GL_LIGHT5,GL_SPOT_DIRECTION,Direction);
			glLightf(GL_LIGHT5,GL_SPOT_CUTOFF,20);
			glLightf(GL_LIGHT5,GL_SPOT_EXPONENT,3);
			//  Set attenuation
			glLightf(GL_LIGHT5,GL_CONSTANT_ATTENUATION ,100/100.0);
			glLightf(GL_LIGHT5,GL_LINEAR_ATTENUATION   ,0/100.0);
			glLightf(GL_LIGHT5,GL_QUADRATIC_ATTENUATION,0/100.0);
  			break;

		case 6:
			//  Enable light 0
			glEnable(GL_LIGHT6);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT6,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT6,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT6,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT6,GL_POSITION,Position);
			//  Set spotlight parameters
			glLightfv(GL_LIGHT6,GL_SPOT_DIRECTION,Direction);
			glLightf(GL_LIGHT6,GL_SPOT_CUTOFF,20);
			glLightf(GL_LIGHT6,GL_SPOT_EXPONENT,3);
			//  Set attenuation
			glLightf(GL_LIGHT6,GL_CONSTANT_ATTENUATION ,100/100.0);
			glLightf(GL_LIGHT6,GL_LINEAR_ATTENUATION   ,0/100.0);
			glLightf(GL_LIGHT6,GL_QUADRATIC_ATTENUATION,0/100.0);
  			break;
		
		case 7:
			//  Enable light 0
			glEnable(GL_LIGHT7);
			//  Set ambient, diffuse, specular components and position of light 0
			glLightfv(GL_LIGHT7,GL_AMBIENT ,Ambient);
			glLightfv(GL_LIGHT7,GL_DIFFUSE ,Diffuse);
			glLightfv(GL_LIGHT7,GL_SPECULAR,Specular);
			glLightfv(GL_LIGHT7,GL_POSITION,Position);
			//  Set spotlight parameters
			glLightfv(GL_LIGHT7,GL_SPOT_DIRECTION,Direction);
			glLightf(GL_LIGHT7,GL_SPOT_CUTOFF,20);
			glLightf(GL_LIGHT7,GL_SPOT_EXPONENT,3);
			//  Set attenuation
			glLightf(GL_LIGHT7,GL_CONSTANT_ATTENUATION ,100/100.0);
			glLightf(GL_LIGHT7,GL_LINEAR_ATTENUATION   ,0/100.0);
			glLightf(GL_LIGHT7,GL_QUADRATIC_ATTENUATION,0/100.0);
  			break;

	}
	



	glColor3f(1,1,1);
	ball(-1,2,0,.2);

	glPopMatrix();
}

static void FireHydrant(double x, double y, double z, double dx, double dy, double dz, double th)
{

	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);

	glColor3f(.2,.2,.2);	
	Cylinder(0,0,0,.7,.7,.1,90,0);
	glColor3f(1,0,0);
	Cylinder(0,.8,0,.4,.4,.8,90,0);
	Cylinder(0,1.6,0,.65,.65,.1,90,0);

	ball(0,1.7,0,.4);
	Cylinder(-.4,1,0,.3,.3,.1,0,90);
	Cylinder(0,1.1,-.5,.2,.2,.1,0,0);
	Cylinder(0,1.1,.5,.2,.2,.1,0,0);
	Cylinder(0,1.1,0,.05,.05,.68,0,0);
	Cylinder(-.5,1,0,.05,.05,.05,0,90);
	Cylinder(0,2.1,0,.05,.05,.05,90,0);
	glPopMatrix();
	glColor3f(1,1,1);

}

static void BikeSeat(double x, double y, double z, double dx, double dy, double dz, double th)
{

	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);
	glBegin(GL_POLYGON);
	glColor3f(.05,.05,.05);
	glNormal3f(0,-1,0);
	glVertex3f(0,-.15,-1);
	glVertex3f(-.35,-.15,-.3);
	glVertex3f(-.1,-.15,0);
	glVertex3f(.1,-.15,0);
	glVertex3f(.35,-.15,-.3);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3f(-1,1,-.5);
	glVertex3f(0,0,-1);
	glVertex3f(-.05,.2,-.3);	
	glVertex3f(-.35,0,-.3);

	glNormal3f(-1,1,.5);
	glVertex3f(-.05,.2,-.3);	
	glVertex3f(-.35,0,-.3);
	glVertex3f(-.1,0,0);

	glNormal3f(1,1,-.5);
	glVertex3f(0,0,-1);
	glVertex3f(.05,.2,-.3);	
	glVertex3f(.35,0,-.3);

	glNormal3f(1,1,.5);
	glVertex3f(.05,.2,-.3);	
	glVertex3f(.35,0,-.3);
	glVertex3f(.1,0,0);

	glNormal3f(0,1,-.5);
	glVertex3f(0,0,-1);
	glVertex3f(.05,.2,-.3);
	glVertex3f(-.05,.2,-.3);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0,.8,1);
	glVertex3f(.05,.2,-.3);
	glVertex3f(-.05,.2,-.3);
	glVertex3f(-.1,0,0);
	glVertex3f(.1,0,0);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1,0,-.5);
	glVertex3f(0,0,-1);
	glVertex3f(0,-.15,-1);
	glVertex3f(-.35,-.15,-.3);
	glVertex3f(-.35,0,-.3);

	glNormal3f(-1,0,.5);
	glVertex3f(-.35,0,-.3);
	glVertex3f(-.35,-.15,-.3);
	glVertex3f(-.1,-.15,0);
	glVertex3f(-.1,0,0);

	glNormal3f(1,0,-.5);
	glVertex3f(0,0,-1);
	glVertex3f(0,-.15,-1);
	glVertex3f(.35,-.15,-.3);
	glVertex3f(.35,0,-.3);

	glNormal3f(1,0,.5);
	glVertex3f(.35,0,-.3);
	glVertex3f(.35,-.15,-.3);
	glVertex3f(.1,-.15,0);
	glVertex3f(.1,0,0);

	glNormal3f(0,0,1);
	glVertex3f(.1,-.15,0);
	glVertex3f(.1,0,0);
	glVertex3f(-.1,0,0);
	glVertex3f(-.1,-.15,0);
	glEnd();
	glPopMatrix();
}


static void Bicycle(double x, double y, double z, double dx, double dy, double dz, double th,double color[3],double rot)
{

	int k;
	glPushMatrix();

	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glRotated(rot,0,0,1);
	glScaled(dx,dy,dz);
	
	glColor3f(color[0],color[1],color[2]);
	Cylinder(0,.13,.05,.05,.05,.72,108,0);
	Cylinder(0,.47,-.75,.05,.05,.9,5,0);
	Cylinder(0,.55,-1.67,.05,.05,.3,115,0);
	

	//glColor3f(.7,.7,.7);
	Cylinder(0,-.1,-.95,.05,.05,.95,30,0);

	glColor3f(.05,.05,.05);
	for (k=0;k<=360;k+=1){
		Cylinder(0,(.6*Sin(k))-.4,(.6*Cos(k))+.75,.05,.05,.05,90,0);
		if (k%12 == 0){
			glColor3f(.7,.7,.7);
			glBegin(GL_QUADS);
			glVertex3f(-.02,-.4,.75);
			glVertex3f(-.02,(.6*Sin(k))-.4,(.6*Cos(k))+.75);
			glVertex3f(.02,(.6*Sin(k))-.4,(.6*Cos(k))+.75);
			glVertex3f(.02,-.4,.75);
			glEnd();
			glColor3f(.05,.05,.05);
		}
	}
	for (k=0;k<=360;k+=1){
		Cylinder(0,(.6*Sin(k))-.4,(.6*Cos(k))-2,.05,.05,.05,90,0);
		if (k%12 == 0){
			glColor3f(.7,.7,.7);
			glBegin(GL_QUADS);
			glVertex3f(-.02,-.4,-2);
			glVertex3f(-.02,(.6*Sin(k))-.4,(.6*Cos(k))-2);
			glVertex3f(.02,(.6*Sin(k))-.4,(.6*Cos(k))-2);
			glVertex3f(.02,-.4,-2);
			glEnd();
			glColor3f(.05,.05,.05);
		}
	}
	
	//front wheel cylinders
	glColor3f(color[0],color[1],color[2]);
	Cylinder(0,-.4,-2,.03,.03,.16,90,90);
	Cylinder(.14,-.04,-1.9,.03,.03,.35,107,0);
	Cylinder(-.14,-.04,-1.9,.03,.03,.35,107,0);
	Cylinder(0,.31,-1.79,.03,.03,.165,0,90);
	//Cylinder(.07,.2,-1.72,.03,.03,.1,100,25);
	//Cylinder(-.07,.2,-1.72,.03,.03,.1,100,-25);*/
	
	//back wheel cylinders
	Cylinder(0,-.4,.75,.03,.03,.165,90,90);
	Cylinder(0,.4,.15,.03,.03,.165,90,90);
	Cylinder(.14,0,.44,.03,.03,.49,53,0);
	Cylinder(-.14,0,.44,.03,.03,.49,53,0);
	Cylinder(0,-.545,-.175,.03,.03,.165,90,90);
	Cylinder(.14,-.47,.265,.03,.03,.46,-10,0);
	Cylinder(-.14,-.47,.265,.03,.03,.46,-10,0);

	//Handlebars
	Cylinder(0,.82,-1.55,.05,.05,.6,0,90);
	glColor3f(.05,.05,.05);
	Cylinder(.51,.82,-1.55,.06,.06,.1,0,90);
	Cylinder(-.51,.82,-1.55,.06,.06,.1,0,90);

   	//pedal circle
	Cylinder(.188,-.5,-.2,.25,.25,.01,0,90);
	
	//pedals
	glColor3f(.4,.4,.4);
	Cylinder(0,-.545,-.175,.02,.02,.3,90,90);
	Cylinder(.3,-.645,-.175,.02,.02,.1,90,0);
	Cylinder(-.3,-.445,-.175,.02,.02,.1,90,0);
	glColor3f(0,0,0);
	glBegin(GL_QUADS);
	//left pedal
	glVertex3f(.32,-.715,-.085);
	glVertex3f(.32,-.775,-.085);
	glVertex3f(.32,-.775,-.265);
	glVertex3f(.32,-.715,-.265);

	glVertex3f(.32,-.715,-.085);
	glVertex3f(.32,-.775,-.085);
	glVertex3f(.55,-.775,-.085);
	glVertex3f(.55,-.715,-.085);

	glVertex3f(.32,-.775,-.085);
	glVertex3f(.55,-.775,-.085);
	glVertex3f(.55,-.775,-.265);
	glVertex3f(.32,-.775,-.265);

	glVertex3f(.55,-.715,-.085);
	glVertex3f(.32,-.715,-.085);
	glVertex3f(.32,-.715,-.265);
	glVertex3f(.55,-.715,-.265);

	glVertex3f(.32,-.775,-.265);
	glVertex3f(.55,-.775,-.265);
	glVertex3f(.55,-.715,-.265);
	glVertex3f(.32,-.715,-.265);

	glVertex3f(.55,-.715,-.265);
	glVertex3f(.55,-.775,-.265);
	glVertex3f(.55,-.775,-.085);
	glVertex3f(.55,-.715,-.085);

	//right pedal
	glVertex3f(-.32,-.315,-.085);
	glVertex3f(-.32,-.375,-.085);
	glVertex3f(-.32,-.375,-.265);
	glVertex3f(-.32,-.315,-.265);

	glVertex3f(-.32,-.315,-.085);
	glVertex3f(-.32,-.375,-.085);
	glVertex3f(-.55,-.375,-.085);
	glVertex3f(-.55,-.315,-.085);

	glVertex3f(-.32,-.375,-.085);
	glVertex3f(-.55,-.375,-.085);
	glVertex3f(-.55,-.375,-.265);
	glVertex3f(-.32,-.375,-.265);

	glVertex3f(-.55,-.315,-.085);
	glVertex3f(-.32,-.315,-.085);
	glVertex3f(-.32,-.315,-.265);
	glVertex3f(-.55,-.315,-.265);

	glVertex3f(-.32,-.375,-.265);
	glVertex3f(-.55,-.375,-.265);
	glVertex3f(-.55,-.315,-.265);
	glVertex3f(-.32,-.315,-.265);

	glVertex3f(-.55,-.315,-.265);
	glVertex3f(-.55,-.375,-.265);
	glVertex3f(-.55,-.375,-.085);
	glVertex3f(-.55,-.315,-.085);

	glEnd();


	//Gears`
	glColor3f(.6,.6,.6);
	Cylinder(.17,-.4,.75,.2,.2,.009,0,90);	
	Cylinder(.19,-.4,.75,.15,.15,.009,0,90);
	Cylinder(.21,-.4,.75,.1,.1,.009,0,90);

	//chain
	glBegin(GL_QUADS);
		
	for (k=-90;k<=90;k+=10){
		glNormal3f(Cos(k),Sin(k),0);
		glVertex3f(.18,.2*Sin(k)-.4,.2*Cos(k)+.75);
		glVertex3f(.16,.201*Sin(k)-.4,.201*Cos(k)+.75);
	}
	glEnd();
	glBegin(GL_QUADS);//Top chain
	glNormal3f(0,1,0);
	glVertex3f(.18,-.2,.70);
	glVertex3f(.16,-.2,.70);
	glVertex3f(.16,-.20625,.65);
	glVertex3f(.18,-.20625,.65);
	glVertex3f(.18,-.2125,.60);
	glVertex3f(.16,-.2125,.60);
	glVertex3f(.16,-.21875,.55);
	glVertex3f(.18,-.21875,.55);
	glVertex3f(.18,-.225,.50);
	glVertex3f(.16,-.225,.50);
	glVertex3f(.16,-.23125,.45);
	glVertex3f(.18,-.23125,.45);
	glVertex3f(.18,-.2375,.40);
	glVertex3f(.16,-.2375,.40);
	glVertex3f(.16,-.24375,.35);
	glVertex3f(.18,-.24375,.35);
	glVertex3f(.18,-.25,.30);
	glVertex3f(.16,-.25,.30);
	glVertex3f(.16,-.25625,.25);
	glVertex3f(.18,-.25625,.25);
	glVertex3f(.18,-.2625,.20);
	glVertex3f(.16,-.2625,.20);
	glVertex3f(.16,-.26875,.15);
	glVertex3f(.18,-.26875,.15);
	glVertex3f(.18,-.275,.10);
	glVertex3f(.16,-.275,.10);
	glVertex3f(.16,-.28125,.5);
	glVertex3f(.18,-.28125,.5);
	glVertex3f(.18,-.2875,0);
	glVertex3f(.16,-.2875,0);
	glVertex3f(.16,-.29375,-.05);
	glVertex3f(.18,-.29375,-.05);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0,-1,0);
	glVertex3f(.18,-.6,.7);
	glVertex3f(.16,-.6,.7);
	glVertex3f(.16,-.6,.65);
	glVertex3f(.18,-.6,.65);
	glVertex3f(.18,-.6,.6);
	glVertex3f(.16,-.6,.6);
	glVertex3f(.16,-.6,.55);
	glVertex3f(.18,-.6,.55);
	glVertex3f(.18,-.6,.5);
	glVertex3f(.16,-.6,.5);
	glVertex3f(.16,-.6,.45);
	glVertex3f(.18,-.6,.45);
	glVertex3f(.18,-.6,.4);
	glVertex3f(.16,-.6,.4);
	glVertex3f(.16,-.6,.35);
	glVertex3f(.18,-.6,.35);
	glVertex3f(.18,-.6,.3);
	glVertex3f(.16,-.6,.3);
	glVertex3f(.16,-.6,.25);
	glVertex3f(.18,-.6,.25);
	glVertex3f(.18,-.6,.2);
	glVertex3f(.16,-.6,.2);
	glVertex3f(.16,-.6,.15);
	glVertex3f(.18,-.6,.15);
	glVertex3f(.18,-.6,.1);
	glVertex3f(.16,-.6,.1);
	glVertex3f(.16,-.6,.05);
	glVertex3f(.18,-.6,.05);
	glVertex3f(.18,-.6,0);
	glVertex3f(.16,-.6,0);
	glVertex3f(.16,-.6,-.05);
	glVertex3f(.18,-.6,-.05);
	glEnd();
	
	BikeSeat(0,.75,.45,.7,.7,.7,0);

	glColor3f(1,1,1);
	glPopMatrix();

}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);

	//  Undo previous transformations
	glLoadIdentity();

	double Ex = -2*dim*Sin(th)*Cos(ph);
	double Ey = +2*dim        *Sin(ph);
	double Ez = +2*dim*Cos(th)*Cos(ph);
	gluLookAt(Ex,Ey,Ez, 0,0,0 , 0,Cos(ph),0);

	//  Orthogonal - set world orientation


	//  Flat or smooth shading
	glShadeModel(GL_SMOOTH);

	//  Light switch
	//  Translate intensity to color vectors
	float Ambient[]   = {0.005*ambient ,0.005*ambient ,0.005*ambient ,1.0};
	float Diffuse[]   = {0.005*diffuse ,0.005*diffuse ,0.005*diffuse ,1.0};
	float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	//  Light position
	float Position[]  = {10,10,3,.0};
	//  Draw light position as ball (still no lighting here)
	glColor3f(1,1,1);
	//ball(Position[0],Position[1],Position[2] , 0.1);
	ball(10,10,3,.5);
	//  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
	//  Enable lighting
	glEnable(GL_LIGHTING);
	//  Location of viewer for specular calculations
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
	//  glColor sets ambient and diffuse color materials
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//  Enable light 0
	glEnable(GL_LIGHT0);
	//  Set ambient, diffuse, specular components and position of light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);
	//glDisable(GL_LIGHTING);
	double color[3];
	color[0] = 1; color[1] = 1; color[2] = 1;
	//set up light posts
	//oposite side
	//BikeSeat(0,2,0,1,1,1,0);
	
	Lightpost(.85,.6,0,.27,.27,.27,0,1);
	Lightpost(.85,.6,-4,.27,.27,.27,0,2);
	Lightpost(.85,.6,-8,.27,.27,.27,0,3);
	Lightpost(.85,.6,4,.27,.27,.27,0,4);

	//light posts on close side
	Lightpost(-4.85,.6,0,.27,.27,.27,180,5);
	Lightpost(-4.85,.6,4,.27,.27,.27,180,6);

	Road(-4.7,0,-12,.3,.3,.3,90);
	Sidewalk(1,.2,0,3,3,3,270,0);
	Sidewalk(1,.2,5,3,3,3,270,0);
	Sidewalk(-5,.2,-15,3,3,3,90,180);
	Sidewalk(-5,.2,-8,3,3,3,90,180);

	//Set up buildings
	Building(3.4,.2,0,1,.5,1,90,color,0);
	Building(3.4,1,0,1,.5,1,90,color,0);
	Building(3.4,-.8,2.5,1,.5,1,90,color,2);
	Building(3.4,.2,4.75,1,.5,1,90,color,3);
	Building(3.4,.2,-7,1,.5,1,90,color,1);
	Building(3.4,.2,-2.5,1,.5,1,90,color,0);
	Building(3.4,.2,-4.5,1,.5,1,90,color,0);
	Building(3.4,1,-2.5,1,.5,1,90,color,0);
	Building(3.4,1,-4.5,1,.5,1,90,color,0);

	Building(3.4,.2,-9.5,1,.5,1,90,color,2);
	Building(3.4,.2,-12,1,.5,1,90,color,0);

	//make car move
	Car(-3,.205,.05*(-150+zh),.06,.06,.06,180,.5*zh, car1Color[car1]);
	Car(-1,.205,-(.08*(-150+zh))+4.5,.06,.06,.06,0,zh, car2Color[car2]);


	

	color[0] = 1; color[1] =1; color[2] = 0;
	Bicycle(-5.5,.46,.5,.25,.25,.25,230,color,20);
	color[0] = 0; color[1] = 0; color[2] = 1;
	Bicycle(2.18,.465,1.1,.25,.25,.25,180,color,10);

	//set up trash cans
	Trash(1.3,.38,-.5,.15,.2,.15,0);
	Trash(1.3,.38,-7.8,.15,.2,.15,0);
	Trash(1.3,.38,4.3,.15,.2,.15,0);
	//near side trash cans
	Trash(-5.2,.38,.5,.15,.2,.15,0);

	FireHydrant(1.3,.2,-3.5,.18,.18,.18,0);
	FireHydrant(-5.2,.2,3.5,.18,.18,.18,180);
	//floor
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glColor3f(.1,.075,.03);
	glVertex3f(2.4,.2,-20);
	glVertex3f(2.4,.2,20);
	glVertex3f(5,.2,20);
	glVertex3f(5,.2,-20);
	glEnd();

	glDisable(GL_LIGHTING);
	Sky(20);


	//  Render the scene and make it visible
	ErrCheck("display");
	glFlush();
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
	//  Elapsed time in seconds
	double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	zh = fmod(90*t,360.0);
	if (zh > 352){
		if (car1 == 2)
			car1 = 0;
		else 
			car1++;
		if (car2 == 3)
			car2 = 0;
		else
			car2++;
	}
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
	if (ch == 27)
		exit(0);
	//  Animate if requested
	glutIdleFunc(move?idle:NULL);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Set projection
	Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800,800);
	glutCreateWindow("Eric Lindemann");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	wheelFrontTexture = LoadTexBMP("wheel.bmp");
	wheelEdgeTexture = LoadTexBMP("wheeledge.bmp");
	nightTexture = LoadTexBMP("night.bmp");
	roadTexture = LoadTexBMP("road.bmp");
	buildingTexture = LoadTexBMP("building.bmp");
	buildingTexture1 = LoadTexBMP("building1.bmp");
	buildingTexture2 = LoadTexBMP("building2.bmp");
	buildingTexture3 = LoadTexBMP("building3.bmp");
	sidewalkTexture = LoadTexBMP("sidewalk.bmp");
	trashTexture = LoadTexBMP("trash.bmp");
	glutIdleFunc(idle);
	//  Pass control to GLUT so it can interact with the user
	ErrCheck("init");
	glutMainLoop();
	return 0;
}
