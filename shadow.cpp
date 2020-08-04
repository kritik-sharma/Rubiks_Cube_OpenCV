// File: shadow.cpp
// Desc: 2 pass z buffer algorightm using opengl
//
// Autumn 2000 - Ge Wang, Christina Hsu
//-----------------------------------------------------------------------------
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>




//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void idleFunc( );
void displayFunc( );
void reshapeFunc( GLsizei width, GLsizei height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void initialize( );

void render();
void shadows();



//-----------------------------------------------------------------------------
// global variables and #defines
//-----------------------------------------------------------------------------
#define INC_VAL     2.0f
#define SAFE_DELETE_ARRAY(x)    { if( x ){ delete [] x; x = NULL; } }

#ifndef M_PI
#define M_PI        3.14159265359
#endif

#ifndef GL_UNSIGNED_INT_8_8_8_8
#define GL_UNSIGNED_INT_8_8_8_8 GL_UNSIGNED_INT
#endif

// width and height of the window
GLsizei g_width = 480;
GLsizei g_height = 360;

// whether to animate
GLboolean g_rotate = GL_TRUE;

// light 0 position
GLfloat g_light_pos[4] = { 2.0f, 2.0f, 2.0f, 1.0f };

// clipping planes
GLdouble eqn1[4] = { 0.0, 0.0, 1.0, 0.0 };

GLfloat g_inc = 0.0f;

// depth buffer
GLfloat * g_depth_light = NULL;
GLfloat * g_depth_view = NULL;

// keep track of the number of frames per second
int theFrameCount = 0;
int theLastFrameTime = 0;


void computeFPS ()
// post: compute frames per second and display in window's title bar
{
    theFrameCount++;
    int currentTime = glutGet(GLenum(GLUT_ELAPSED_TIME));
    if (currentTime - theLastFrameTime > 1000)
    {
        char s[16];
        sprintf(s, "FPS: %4.2f",
		theFrameCount * 1000.0 / (currentTime - theLastFrameTime));
	glutSetWindowTitle(s);

        theLastFrameTime = currentTime;
        theFrameCount = 0;
    }
}



//-----------------------------------------------------------------------------
// Name: main( )
// Desc: entry point
//-----------------------------------------------------------------------------
int main( int argc, char ** argv )
{
    // initialize GLUT
    glutInit( &argc, argv );
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( g_width, g_height );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow( "Shadow Sample" );

    // set the idle function - called when idle
    glutIdleFunc( g_rotate ? idleFunc : NULL );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );

    // do our own initialization
    initialize();

    // let GLUT handle the current thread from here
    glutMainLoop();

    return 0;
}




//-----------------------------------------------------------------------------
// Name: initialize( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize()
{
    // set the GL clear color - use when the color buffer is cleared
    glClearColor( 0.0f, 0.0f,0.0f, 1.0f );
    // set the shading model to 'smooth'
    glShadeModel( GL_SMOOTH );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    // set the front faces of polygons
    glFrontFace( GL_CCW );
    // set fill mode
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    // set the line width
    glLineWidth( 2.0f );

    // enable lighting
    glEnable( GL_LIGHTING );
    // enable lighting for front
    glLightModeli( GL_FRONT, GL_TRUE );
    // material have diffuse and ambient lighting 
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    // enable color
    glEnable( GL_COLOR_MATERIAL );

    // enable light 0
    glEnable( GL_LIGHT0 );

    printf( "press [L|R] mouse button to rotate\n" );
}




//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( GLsizei w, GLsizei h )
{
    // save the new window size
    g_width = w; g_height = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( 45.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0 );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
    // position the view point
    gluLookAt( 0.0f, 1.2f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

    SAFE_DELETE_ARRAY( g_depth_light );
    SAFE_DELETE_ARRAY( g_depth_view );

    g_depth_light = new GLfloat[g_width * g_height];
    g_depth_view = new GLfloat[g_width * g_height];
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
    case 'Q':
    case 'q':
        exit( 1 );
        break;
    }
}




//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // rotate
        if( state == GLUT_DOWN )
            g_inc -= INC_VAL;
        else
            g_inc += INC_VAL;
    }   
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        if( state == GLUT_DOWN )
            g_inc += INC_VAL;
        else
            g_inc -= INC_VAL;
    }
    else
        g_inc = 0.0f; 
    
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    computeFPS();
    // render the scene
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
    static GLfloat angle = -14.0;
    GLint buffer = GL_NONE;
    GLfloat * p = g_light_pos;
    glGetIntegerv( GL_DRAW_BUFFER, &buffer );

    // clear the color and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix( );
    glRotatef( angle += g_inc, 0.0f, 1.0f, 0.0f );
    
    // set the position of the light
    glLightfv( GL_LIGHT0, GL_POSITION, g_light_pos );

    // switch to viewpoint of light
    glPushMatrix();
        // disable drawing into color buffer
        glDrawBuffer( GL_NONE );

        // set the camera to the viewpoint of the light
        glLoadIdentity();
        gluLookAt( p[0], p[1], p[2], 0, 0, 0, 0, 1, 0 );

        // draw scene
        render();

        // save the depth buffer
        glReadPixels( 0, 0, g_width, g_height, GL_DEPTH_COMPONENT,
                      GL_FLOAT, g_depth_light );

        // enable drawing into color buffer
        glDrawBuffer( (GLenum)buffer );
    glPopMatrix( );

    // clear the depth buffer
    glClear( GL_DEPTH_BUFFER_BIT );
    // draw scene
    render( );

    // draw the shadow
    shadows( );
    glPopMatrix( );
    
    glFlush( );
    glutSwapBuffers( );
}





//-----------------------------------------------------------------------------
// Name: render( )
// Desc: draws scene
//-----------------------------------------------------------------------------
void render( )
{
    static GLfloat angle = 0.0f;
    
    glColor3f( .4f, 1.0f, .4f );
    glutSolidSphere( 0.6, 12, 12 );

    glPushMatrix();
      glTranslatef( .6f, .35f, .6f );
      glColor3f( 1.0f, .7f, .7f );
      glutSolidCube( .2 );
    glPopMatrix();

    glPushMatrix();
      glTranslatef( .7f, .85f, .7f );
      glRotatef( angle += 1, 0.0, 1.0, 0.0 );
      glTranslatef( 0.0f, -.2f, 0.0f );
      glRotatef( -90, 1.0f, 0.0f, 0.0f );
      glColor3f( 1.0f, 1.0f, .4f );
      glutWireCone( .2, .4, 8, 8 );
    glPopMatrix();

    glPushMatrix();
      glTranslatef( -.9f, -0.9f, -.1f );
      glRotatef( 90, -0.5f, .5f, 0.15f );
      glRotatef( angle, 0.0, 0.0, 1.0 );
      glColor3f( 1.0f, .4f, 1.0f );
      glutWireTorus( .2, .5, 8, 8 );
    glPopMatrix();
}




//-----------------------------------------------------------------------------
// Name: shadows( )
// Desc: draws shadow
//-----------------------------------------------------------------------------
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
    GLuint pixel[2] = { 0xffffffff };
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
      gluLookAt( p[0], p[1], p[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
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