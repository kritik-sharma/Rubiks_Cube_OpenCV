#include "data.h"

#define INC_VAL     2.0f
#define SAFE_DELETE_ARRAY(x)    { if( x ){ delete [] x; x = NULL; } }

#ifndef M_PI
#define M_PI        3.14159265359
#endif

#ifndef GL_UNSIGNED_INT_8_8_8_8
#define GL_UNSIGNED_INT_8_8_8_8 GL_UNSIGNED_INT
#endif

// width and height of the window
GLsizei g_width = 500;
GLsizei g_height = 500;

// whether to animate
GLboolean g_rotate = GL_TRUE;

// light 0 position
GLfloat g_light_pos[4] = { 2.0f, 2.0f, 2.0f, 100.0f };


// clipping planes
//GLdouble eqn1[4] = { 0.0, 0.0, 1.0, 0.0 };

GLfloat g_inc = 0.0f;

// depth buffer
GLfloat * g_depth_light = NULL;
GLfloat * g_depth_view = NULL;

// keep track of the number of frames per second
int theFrameCount = 0;
int theLastFrameTime = 0;

void render();
void shadows();

void *font = GLUT_BITMAP_TIMES_ROMAN_24;
char defaultMessage[] = "Rotation Speed:";
char *message = defaultMessage;

void
output(int x, int y, char *string)
{
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

GLuint texture;

void gentexture(){
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    //to the edge of our shape. 
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

// load and generate the texture
int width, height, nrChannels;
unsigned char *data = stbi_load("apple.jpg", &width, &height, &nrChannels, 0);
if (data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
    std::cout << "Failed to load texture\n";
}
stbi_image_free(data);
}

int w1 = 0;
int h1 = 0;


void orthogonalStart() 
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-w1/2, w1/2, -h1/2, h1/2);
    glMatrixMode(GL_MODELVIEW);
}

void orthogonalEnd()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


//GLuint texture = 0;
void background()
{
    glBindTexture( GL_TEXTURE_2D, texture ); 

    orthogonalStart();

    // texture width/height
    const int iw = 500;
    const int ih = 500;

    glPushMatrix();
    glTranslatef( -iw/2, -ih/2, 0 );
    glBegin(GL_QUADS);
        glTexCoord2i(0,0); glVertex2i(0, 0);
        glTexCoord2i(1,0); glVertex2i(iw, 0);
        glTexCoord2i(1,1); glVertex2i(iw, ih);
        glTexCoord2i(0,1); glVertex2i(0, ih);
    glEnd();
    glPopMatrix();

    orthogonalEnd();
}

void shadows( )
{
    GLdouble mv_mat[16], proj_mat[16];
    GLint viewport[4];
    GLdouble objx, objy, objz;
    GLdouble depth;

    GLfloat * p = g_light_pos;
    GLdouble mv_light[16];
    GLdouble winx, winy, winz;
    GLdouble depth_2;
    
    // color of pixels in shadow
    GLuint pixel[2] = { 0x00000000 };
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // get the modelview, project, and viewport
    glGetDoublev( GL_MODELVIEW_MATRIX, mv_mat );
    glGetDoublev( GL_PROJECTION_MATRIX, proj_mat );
    glGetIntegerv( GL_VIEWPORT, viewport );
    // get the current depth buffer
    glReadPixels( 0, 0, g_width, g_height, GL_DEPTH_COMPONENT,
                  GL_FLOAT, g_depth_view );

    // get the transformation from light view
    glPushMatrix();
      glLoadIdentity( );
      gluLookAt( p[1], p[0], p[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
      glGetDoublev( GL_MODELVIEW_MATRIX, mv_light );
    glPopMatrix();

    // set the project matrix to orthographic
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0.0, (GLfloat)g_width, 0.0, (GLfloat)g_height);

    // set the modelview matrix to identity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();

    // go through every pixel in frame buffer
    for( GLint y = 0; y < g_height; y++ )
        for( GLint x = 0; x < g_width; x++ )
        {
            // depth at pixel

            //std::cout <<g_depth_view[0]<<"\n";
            //std::cout <<"ab\n";
            depth = g_depth_view[ y * g_width + x ];

            // on the far plane of frustum - don't calculate
            if( depth > .99 )
                continue;

            // get world coordinate from x, y, depth
            gluUnProject( x, y, depth, mv_mat, proj_mat, viewport,
                          &objx, &objy, &objz );

            // get light view screen coordinate and depth
            gluProject( objx, objy, objz, mv_light, proj_mat,
                        viewport, &winx, &winy, &winz );

            // make sure within the screen
            if( winx >= g_width || winy >= g_height || winx < 0 || winy < 0 )
                continue;

            // get the depth value from the light
            depth_2 = g_depth_light[ GLint(winy) * g_width + GLint(winx) ];

            // is something between the light and the pixel?
            if( (winz - depth_2) > .01 )
            {
                glRasterPos2i( x, y );
                glDrawPixels( 1, 1, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, &pixel );
            }
        }
        
    // restore modelview transformation
    glPopMatrix();

    // restore projection
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void render()

{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
  glEnable( GL_TEXTURE_2D );

  glColor3fv(color[0]);

	glPushMatrix();
    glRotatef(25.0+p,1.0,0.0,0.0);
    glRotatef(-30.0+q,0.0,1.0,0.0);
    glRotatef(0.0+r,0.0,0.0,1.0);


if(rotation==0)
{

colorcube1();
colorcube2();
colorcube3();
colorcube4();
colorcube5();
colorcube6();
colorcube7();
colorcube8();
colorcube9();
colorcube10();
colorcube11();
colorcube12();
colorcube13();
colorcube14();
colorcube15();
colorcube16();
colorcube17();
colorcube18();
colorcube19();
colorcube20();
colorcube21();
colorcube22();
colorcube23();
colorcube24();
colorcube25();
colorcube26();
colorcube27();
}
if(rotation==1)
{


colorcube1();
colorcube2();
colorcube3();
colorcube4();
colorcube6();
colorcube7();
colorcube12();
colorcube13();
colorcube14();
colorcube15();
colorcube20();
colorcube21();
colorcube22();
colorcube23();
colorcube24();
colorcube25();
colorcube26();
colorcube27();

if(inverse==0)
{glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"Top");
	glPopMatrix();
glRotatef(-theta,0.0,1.0,0.0);
}
else
{glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"TopInverted");
	glPopMatrix();
glRotatef(theta,0.0,1.0,0.0);
}
colorcube5();
colorcube8();
colorcube9();
colorcube10();
colorcube11();
colorcube16();
colorcube17();
colorcube18();
colorcube19();

}

if(rotation==2)
{
colorcube1();
colorcube2();
colorcube3();
colorcube5();
colorcube6();
colorcube7();
colorcube8();
colorcube10();
colorcube11();
colorcube12();
colorcube14();
colorcube15();
colorcube16();
colorcube17();
colorcube20();
colorcube21();
colorcube24();
colorcube25();
if(inverse==0)
{
glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"Right");
	glPopMatrix();
glRotatef(-theta,1.0,0.0,0.0);
}
else
{
	glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"RightInverted");
	glPopMatrix();
glRotatef(theta,1.0,0.0,0.0);
}
colorcube4();
colorcube9();
colorcube13();
colorcube18();
colorcube19();
colorcube22();
colorcube23();
colorcube26();
colorcube27();
}



if(rotation==3)
{
colorcube1();
colorcube2();
colorcube3();
colorcube4();
colorcube5();
colorcube7();
colorcube8();
colorcube9();
colorcube11();
colorcube12();
colorcube13();
colorcube15();
colorcube16();
colorcube18();
colorcube20();
colorcube22();
colorcube24();
colorcube26();
if(inverse==0)
{
	glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"Front");
	glPopMatrix();
glRotatef(-theta,0.0,0.0,1.0);
}

else
{
	glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"FrontInverted");
	glPopMatrix();
glRotatef(theta,0.0,0.0,1.0);
}

colorcube6();
colorcube10();
colorcube14();
colorcube17();
colorcube19();
colorcube21();
colorcube23();
colorcube25();
colorcube27();
}

if(rotation==4)
{
colorcube1();
colorcube2();
colorcube4();
colorcube5();
colorcube6();
colorcube7();
colorcube9();
colorcube10();
colorcube11();
colorcube13();
colorcube14();
colorcube15();
colorcube18();
colorcube19();
colorcube22();
colorcube23();
colorcube26();
colorcube27();
if(inverse==0)
{glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"Left");
	glPopMatrix();
glRotatef(theta,1.0,0.0,0.0);
}
else
{glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"LeftInverted");
     glPopMatrix();
	glRotatef(-theta,1.0,0.0,0.0);
}
colorcube3();
colorcube8();
colorcube12();
colorcube16();
colorcube17();
colorcube20();
colorcube21();
colorcube24();
colorcube25();
}


if(rotation==5)
{
colorcube1();
colorcube2();
colorcube3();
colorcube4();
colorcube5();
colorcube6();
colorcube8();
colorcube9();
colorcube10();
colorcube12();
colorcube13();
colorcube14();
colorcube17();
colorcube19();
colorcube21();
colorcube23();
colorcube25();
colorcube27();
if(inverse==0)
{glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"Back");
	glPopMatrix();
glRotatef(theta,0.0,0.0,1.0);

}
else
{
	glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"BackInverted");
	glPopMatrix();
	glRotatef(-theta,0.0,0.0,1.0);
}
colorcube7();
colorcube11();
colorcube15();
colorcube16();
colorcube18();
colorcube20();
colorcube22();
colorcube24();
colorcube26();
}

if(rotation==6)
{
colorcube1();
colorcube3();
colorcube4();
colorcube5();
colorcube6();
colorcube7();
colorcube8();
colorcube9();
colorcube10();
colorcube11();
colorcube16();
colorcube17();
colorcube18();
colorcube19();
colorcube20();
colorcube21();
colorcube22();
colorcube23();

if(inverse==0)
{glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"Bottom");
	glPopMatrix();
	glRotatef(theta,0.0,1.0,0.0);
}
else
{glPushMatrix();
glColor3fv(color[0]);
	output(-11,6,"BottomInverted");
	glPopMatrix();

glRotatef(-theta,0.0,1.0,0.0);
}
colorcube2();
colorcube12();
colorcube13();
colorcube14();
colorcube15();
colorcube24();
colorcube25();
colorcube26();
colorcube27();

}
glPopMatrix();


/*glPushMatrix();
glTranslatef(-.5,-4,0);
glScalef(speed/4.5,1.0,1.0);
glTranslatef(0.5,4,0);
polygon(5,216,217,218,219);

glPopMatrix();
*/

}

void display( )
{
    static GLfloat angle = 0.0;
    GLint buffer = GL_NONE;
    GLfloat * p = g_light_pos;
    glGetIntegerv( GL_DRAW_BUFFER, &buffer );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix( );
    glRotatef( angle += g_inc, 0.0f, 1.0f, 0.0f );
    
    glLightfv( GL_LIGHT0, GL_POSITION, g_light_pos );   //GL_DIFFUSE

    glPushMatrix();
        glDrawBuffer( GL_NONE );

        glLoadIdentity();
        gluLookAt( p[0], p[1], p[2], 0, 0, 0, 0, 1, 0 );

        render();

        glReadPixels( 0, 0, g_width, g_height, GL_DEPTH_COMPONENT,
                      GL_FLOAT, g_depth_light );

        glDrawBuffer( (GLenum)buffer );
    glPopMatrix( );

    glClear( GL_DEPTH_BUFFER_BIT );
    render( );
    glPopMatrix( );
    
    glFlush( );
    glutSwapBuffers( );
}


void transpose(char a)
{

 if(a=='r')
	{
	int temp;
	temp=right[0][0];
	right[0][0]=right[2][0];
	right[2][0]=right[2][2];
	right[2][2]=right[0][2];
	right[0][2]=temp;
	temp=right[1][0];
    right[1][0]=right[2][1];
	right[2][1]=right[1][2];
	right[1][2]=right[0][1];
	right[0][1]=temp;
 }
	if(a=='t')
	{
	int temp;
	temp=top[0][0];
	top[0][0]=top[2][0];
	top[2][0]=top[2][2];
	top[2][2]=top[0][2];
	top[0][2]=temp;
	temp=top[1][0];
    top[1][0]=top[2][1];
	top[2][1]=top[1][2];
	top[1][2]=top[0][1];
	top[0][1]=temp;
}
		if(a=='f')
	{
	int temp;
	temp=front[0][0];
	front[0][0]=front[2][0];
	front[2][0]=front[2][2];
	front[2][2]=front[0][2];
	front[0][2]=temp;
	temp=front[1][0];
    front[1][0]=front[2][1];
	front[2][1]=front[1][2];
	front[1][2]=front[0][1];
	front[0][1]=temp;
	}
	if(a=='l')
	{
	int temp;
	temp=left[0][0];
	left[0][0]=left[2][0];
	left[2][0]=left[2][2];
	left[2][2]=left[0][2];
	left[0][2]=temp;
	temp=left[1][0];
    left[1][0]=left[2][1];
	left[2][1]=left[1][2];
	left[1][2]=left[0][1];
	left[0][1]=temp;
	}
	if(a=='k')
	{
	int temp;
	temp=back[0][0];
	back[0][0]=back[2][0];
	back[2][0]=back[2][2];
	back[2][2]=back[0][2];
	back[0][2]=temp;
	temp=back[1][0];
    back[1][0]=back[2][1];
	back[2][1]=back[1][2];
	back[1][2]=back[0][1];
	back[0][1]=temp;
	}

		if(a=='b')
	{
	int temp;
	temp=bottom[0][0];
	bottom[0][0]=bottom[2][0];
	bottom[2][0]=bottom[2][2];
	bottom[2][2]=bottom[0][2];
	bottom[0][2]=temp;
	temp=bottom[1][0];
    bottom[1][0]=bottom[2][1];
	bottom[2][1]=bottom[1][2];
	bottom[1][2]=bottom[0][1];
	bottom[0][1]=temp;
	}
}


void topc()
{
	transpose('t');
int temp1=front[0][0];
int temp2=front[0][1];
int temp3=front[0][2];

front[0][0]=right[0][0];
front[0][1]=right[0][1];
front[0][2]=right[0][2];

right[0][0]=back[0][0];
right[0][1]=back[0][1];
right[0][2]=back[0][2];

back[0][0]=left[0][0];
back[0][1]=left[0][1];
back[0][2]=left[0][2];

left[0][0]=temp1;
left[0][1]=temp2;
left[0][2]=temp3;

}

void frontc()
{
transpose('f');
int temp1=left[0][2];
int temp2=left[1][2];
int temp3=left[2][2];

left[0][2]=bottom[0][0];
left[1][2]=bottom[0][1];
left[2][2]=bottom[0][2];

bottom[0][0]=right[2][0];
bottom[0][1]=right[1][0];
bottom[0][2]=right[0][0];

right[2][0]=top[2][2];
right[1][0]=top[2][1];
right[0][0]=top[2][0];

top[2][2]=temp1;
top[2][1]=temp2;
top[2][0]=temp3;
}

void rightc()
{
	transpose('r');
	int temp1=top[0][2];
    int temp2=top[1][2];
	int temp3=top[2][2];

	top[0][2]=front[0][2];
	top[1][2]=front[1][2];
	top[2][2]=front[2][2];

	front[0][2]=bottom[0][2];
	front[1][2]=bottom[1][2];
	front[2][2]=bottom[2][2];


	bottom[0][2]=back[2][0];
	bottom[1][2]=back[1][0];
	bottom[2][2]=back[0][0];

	back[2][0]=temp1;
	back[1][0]=temp2;
	back[0][0]=temp3;

}

void leftc()
{
transpose('l');
	int temp1=front[0][0];
	int temp2=front[1][0];
	int temp3=front[2][0];

	front[0][0]=top[0][0];
	front[1][0]=top[1][0];
	front[2][0]=top[2][0];

	top[0][0]=back[2][2];
	top[1][0]=back[1][2];
	top[2][0]=back[0][2];

	back[2][2]=bottom[0][0];
	back[1][2]=bottom[1][0];
	back[0][2]=bottom[2][0];


	bottom[0][0]=temp1;
	bottom[1][0]=temp2;
	bottom[2][0]=temp3;
}

void backc()
{
	transpose('k');
	int temp1=top[0][0];
	int temp2=top[0][1];
	int temp3=top[0][2];

	top[0][0]=right[0][2];
	top[0][1]=right[1][2];
	top[0][2]=right[2][2];

	right[0][2]=bottom[2][2];
	right[1][2]=bottom[2][1];
	right[2][2]=bottom[2][0];

	bottom[2][2]=left[2][0];
	bottom[2][1]=left[1][0];
	bottom[2][0]=left[0][0];

	left[2][0]=temp1;
	left[1][0]=temp2;
	left[0][0]=temp3;
}


void bottomc()
{
	transpose('b');
	int temp1=front[2][0];
	int temp2=front[2][1];
	int temp3=front[2][2];

	front[2][0]=left[2][0];
	front[2][1]=left[2][1];
	front[2][2]=left[2][2];

	left[2][0]=back[2][0];
	left[2][1]=back[2][1];
	left[2][2]=back[2][2];

	back[2][0]=right[2][0];
	back[2][1]=right[2][1];
	back[2][2]=right[2][2];

	right[2][0]=temp1;
	right[2][1]=temp2;
	right[2][2]=temp3;

}


void spincube()

{   theta+=0.5+speed;
	 if(theta==360.0)
	theta-=360.0;
	if(theta>=90.0)
	{
		rotationcomplete=1;
	glutIdleFunc(NULL);

if(rotation==1&&inverse==0)
	{
	topc();
}
if(rotation==1&&inverse==1)
	{
	topc();
	topc();
	topc();
}

if(rotation==2&&inverse==0)
	{
   rightc();

   }
if(rotation==2&&inverse==1)
	{
rightc();
rightc();
rightc();

}
if(rotation==3&&inverse==0)
	{
	frontc();
}
if(rotation==3&&inverse==1)
	{
	frontc();
	frontc();
	frontc();
}
if(rotation==4&&inverse==0)
{
	leftc();
}
if(rotation==4&&inverse==1)
{
	leftc();
	leftc();
	leftc();

}
if(rotation==5&&inverse==0)
{
	backc();
}
if(rotation==5&&inverse==1)
{
	backc();
	backc();
	backc();
}
if(rotation==6&&inverse==0)
{
	bottomc();
}
if(rotation==6&&inverse==1)
{
	bottomc();
	bottomc();
	bottomc();

}

rotation=0;
theta=0;
}



glutPostRedisplay();
}


void
motion(int x, int y)
{
  if (moving) {

    q=q + (x - beginx);
    beginx = x;
    p=p + (y - beginy);
	beginy=y;
	glutPostRedisplay();
  }
}


void mouse(int btn,int state,int x,int y)
{
	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
	{
   moving = 1;
   beginx = x;
   beginy=y;

	}

}


static void keyboard(unsigned char key,int x,int y)
{
	if(key=='a'&&rotationcomplete==1)
	{     rotationcomplete=0;
          rotation=1;
		  inverse=0;
		  solve[++count]=1;
		  glutIdleFunc(spincube);

	}
	if(key=='q'&&rotationcomplete==1)
	{     rotationcomplete=0;
          rotation=1;
		  inverse=1;
		  solve[++count]=-1;
		  glutIdleFunc(spincube);

	}
	if(key=='s'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=2;
		inverse=0;
		solve[++count]=2;
	glutIdleFunc(spincube);
	}
if(key=='w'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=2;
		inverse=1;
		solve[++count]=-2;
	glutIdleFunc(spincube);
	}
if(key=='d'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=3;
		inverse=0;
		solve[++count]=3;
	glutIdleFunc(spincube);

	}
if(key=='e'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=3;
		inverse=1;
		solve[++count]=-3;
	glutIdleFunc(spincube);

	}

if(key=='f'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=4;
		inverse=0;
		solve[++count]=4;
	glutIdleFunc(spincube);

	}
if(key=='r'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=4;
		inverse=1;
		solve[++count]=-4;
	glutIdleFunc(spincube);

	}
if(key=='g'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=5;
		inverse=0;
		solve[++count]=5;
	glutIdleFunc(spincube);

	}
if(key=='t'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=5;
		inverse=1;
		solve[++count]=-5;
	glutIdleFunc(spincube);

	}

if(key=='h'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=6;
		inverse=0;
		solve[++count]=6;
	glutIdleFunc(spincube);

	}

if(key=='y'&&rotationcomplete==1)
	{rotationcomplete=0;
		rotation=6;
		inverse=1;
		solve[++count]=-6;
	glutIdleFunc(spincube);

	}
if(key=='2'&&rotationcomplete==1)
	{
p=p+2.0;
glutIdleFunc(spincube);
}
if(key=='8'&&rotationcomplete==1)
	{
p=p-2.0;
glutIdleFunc(spincube);
}
if(key=='6'&&rotationcomplete==1)
	{
q=q+2.0;
glutIdleFunc(spincube);
}
if(key=='4'&&rotationcomplete==1)
	{
q=q-2.0;
glutIdleFunc(spincube);
}

if(key=='9'&&rotationcomplete==1)
	{
r=r+2.0;
glutIdleFunc(spincube);
}

if(key=='1'&&rotationcomplete==1)
	{
r=r-2.0;
glutIdleFunc(spincube);
}
if(key=='5'&&rotationcomplete==1)
	{
p=0.0;
q=0.0;
r=0.0;
glutIdleFunc(spincube);
}

if(key=='n'&&rotationcomplete==1)
	{
		if(speed>=0.3)
		{
        speed=speed-0.3;
		speedmetercolor[speedmetercount--]=0;
		}
		glutPostRedisplay();

}


if(key=='o'&&rotationcomplete==1)
	{
		rotationcomplete=0;
	if(count>=0)
	{
		if(solve[count]<0)
			{
				rotation=-1*solve[count];
				inverse=0;
	glutIdleFunc(spincube);

			}
				if(solve[count]>0)
				{
				rotation=solve[count];
				inverse=1;
	glutIdleFunc(spincube);
				}


	--count;
}


  glutIdleFunc(spincube);


}

}

void myreshape(int w,int h){
    g_width = w; g_height = h;
    
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
      glOrtho(-10.0,10.0,-10.0*(GLfloat)h/(GLfloat)w, 10.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
    else
      glOrtho(-10.0*(GLfloat)w/(GLfloat)h, 10.0*(GLfloat)w/(GLfloat)h,-10.0,10.0,-10.0,10.0);

    glMatrixMode(GL_MODELVIEW);

      SAFE_DELETE_ARRAY( g_depth_light );
      SAFE_DELETE_ARRAY( g_depth_view );

      g_depth_light = new GLfloat[g_width * g_height];
      g_depth_view = new GLfloat[g_width * g_height];

}

void mymenu(int id){
	if(rotationcomplete==1){
	 rotationcomplete=0;
    switch(id)
    {
        case 1:
    		rotation=1;
           	inverse=0;
    		solve[++count]=1;
    	    glutIdleFunc(spincube);
    	break;

    	case 2:
    		rotation=1;
    		inverse=1;
    		solve[++count]=-1;
    		glutIdleFunc(spincube);
    	break;

    	case 3:
    		rotation=2;
    		inverse=0;
    		solve[++count]=2;
    		glutIdleFunc(spincube);
    	break;

    	case 4:
    		rotation=2;
    		inverse=1;
    		solve[++count]=-2;
    		glutIdleFunc(spincube);
    	break;
    		case 5:
    		rotation=3;
    		inverse=0;
    		solve[++count]=3;
    		glutIdleFunc(spincube);
    	break;
    		case 6:
    		rotation=3;
    		inverse=1;
    		solve[++count]=-3;
    		glutIdleFunc(spincube);
    	break;
    		case 7:
    		rotation=4;
    		inverse=0;
    		solve[++count]=4;
    		glutIdleFunc(spincube);
    	break;
    		case 8:
    		rotation=4;
    		inverse=1;
    		solve[++count]=-4;
    		glutIdleFunc(spincube);
    	break;
    		case 9:
    		rotation=5;
    		inverse=0;
    		solve[++count]=5;
    		glutIdleFunc(spincube);
    	break;
    		case 10:
    		rotation=5;
    		inverse=1;
    		solve[++count]=-5;
    		glutIdleFunc(spincube);
    	break;
    		case 11:
    		rotation=6;
    		inverse=0;
    		solve[++count]=6;
    		glutIdleFunc(spincube);
    	break;
    		case 12:
    		rotation=6;
    		inverse=1;
    		solve[++count]=-6;
    		glutIdleFunc(spincube);
    	break;

    	 case 13:
    			exit(0);
    	break;

}
	}

}

void init()
{
    glClearColor( 0.0f, 0.0f,0.0f, 1.0f );
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glEnable( GL_LIGHTING );
    glLightModeli( GL_FRONT, GL_TRUE );
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    glEnable( GL_LIGHT0 );
}

int main(int argc, char** argv){

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (g_width, g_height);
  glutInitWindowPosition( 100, 100 );
  glutCreateWindow ("RUBIK'S CUBE");
  glutReshapeFunc (myreshape);
  glutIdleFunc(spincube);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutCreateMenu(mymenu);
  glutAddMenuEntry("Top                   :a",1);
  glutAddMenuEntry("Top Inverted    :q",2);
  glutAddMenuEntry("Right                 :s",3);
  glutAddMenuEntry("Right Inverted  :w",4);
  glutAddMenuEntry("Front                 :d",5);
  glutAddMenuEntry("Front Inverted  :e",6);
  glutAddMenuEntry("Left                   :f",7);
  glutAddMenuEntry("Left Inverted    :r",8);
  glutAddMenuEntry("Back                  :g",9);
  glutAddMenuEntry("Back Inverted   :t",10);
  glutAddMenuEntry("Bottom              :h",11);
  glutAddMenuEntry("Bottom Inverted :y",12);

gentexture();
init();

glutAddMenuEntry("Exit",13);
glutAttachMenu(GLUT_RIGHT_BUTTON);
glutKeyboardFunc(keyboard);
glutDisplayFunc (display);
glEnable(GL_DEPTH_TEST);
glutMainLoop();

}
