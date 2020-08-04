#include "data.h"

void *font = GLUT_BITMAP_TIMES_ROMAN_24;
char defaultMessage[] = "Rotation Speed:";
char *message = defaultMessage;


void output(int x, int y, char *string)
{
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

GLuint texture;

void gentexture(int i){
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  int width, height, nrChannels;
  unsigned char *data;
  std::string filename;
  
  filename = "images/image_part_00";
  filename+=i+'0';
  filename+=".jpg";
  std::cout << filename<<"\n";
  data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  else
  {
      std::cout << "Failed to load texture\n";
  }
  stbi_image_free(data);
}

void display()

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
	glFlush();
	glutSwapBuffers();

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
  w1 = w;
  h1 = h;
    
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
	glOrtho(-10.0,10.0,-10.0*(GLfloat)h/(GLfloat)w, 10.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
	else
	glOrtho(-10.0*(GLfloat)w/(GLfloat)h, 10.0*(GLfloat)w/(GLfloat)h,-10.0,10.0,-10.0,10.0);
	glMatrixMode(GL_MODELVIEW);
}
void mymenu(int id)
{
	if(rotationcomplete==1)
	{rotationcomplete=0;
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
int main(int argc, char** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize (500, 500);
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

//for(int i=1;i<=6;++i)
  //gentexture(1);
  //gentexture2(7);

glutAddMenuEntry("Exit",13);
glutAttachMenu(GLUT_RIGHT_BUTTON);
glutKeyboardFunc(keyboard);
glutDisplayFunc (display);
glEnable(GL_DEPTH_TEST);
glutMainLoop();
//return 0;
}
