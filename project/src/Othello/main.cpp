//
//  main.cpp
//  Week 10 - 3D Objects
//
//    This one does the standard computer graphics setup of
//    a 3D scene.
//
//  Created by Jean-Yves Hervé on 2023-10-30.
//

#include <vector>
#include <memory>
#include <iostream>
#include "drawingUtilities.h"
#include "common.h"
#include "Quad3D.h"
#include "QuadMesh3D.h"
#include "Cylinder3D.h"
#include "Board.hpp"
#include "Disc3D.h"


using namespace std;
using namespace othello;
using namespace graphics3d;


void drawOrbit(void);
void cameraToWorld(void);
void setupCamera(void);
void myDisplayFunc(void);
void myResizeFunc(int w, int h);
void myMouseFunc(int b, int s, int x, int y);
void myKeyboardFunc(unsigned char c, int x, int y);
void myTimerFunc(int dt);
void myInit(void);


//    Camera intrinsic parameters
float gFoV = 45.f,            //    vertical field of view in degree
      gNearZ = 1.0f,            //    Position of the clipping planes along the camera's
      gFarZ = 50.0f;            //        optical axis (Z axis)
            
//    Camera to world transformation (extrinsic parameters)
float gTx = 0.25f,
      gTy = -0.2f,
      gTz = -2.f,
      gRoll = 0.0f,
      gPitch =-60.f,
      gYaw = 10.f;
                        
RenderingMode renderingMode = RenderingMode::SmoothShadingRender;

bool drawReferenceFrames = false;

int winWidth = 1200, winHeight = 900;

GLfloat ambient[4];
GLfloat diffuse[4];
GLfloat specular[4];
GLfloat emission[4];
GLfloat shiny;


//    ambient, diffuse, specular, emission
Material gray1{0.7f, 0.7f, 0.7f, 1.f, 0.7f, 0.7f, 0.7f, 1.f, 0.7f, 0.7f, 0.7f, 1.f, 0.7f, 0.7f, 0.7f, 1.f, 0.3f};
Material gray2{0.3f, 0.3f, 0.3f, 1.f, 0.3f, 0.3f, 0.3f, 1.f, 0.3f, 0.3f, 0.3f, 1.f, 0.2f, 0.2f, 0.2f, 1.f, 0.3f};
                
Material red1{0.9f, 0.1f, 0.1f, 1.f, 0.9f, 0.1f, 0.1f, 1.f, 0.9f, 0.1f, 0.1f, 1.f, 0.9f, 0.1f, 0.1f, 1.f, 0.3f};
Material specRed1{0.9f, 0.1f, 0.1f, 1.f, 0.4f, 0.05f, 0.05f, 1.f, 0.9f, 0.2f, 0.2f, 1.f, 0.2f, 0.0f, 0.0f, 1.f, 0.6f};


float Board::pixelToWorldRatio;
float Board::worldToPixelRatio;
float Board::drawInPixelScale;


vector<shared_ptr<GraphicObject3D> > objList;


//    Move from the camera to the world reference frame:  Start from the camera and apply
//    a series of transformations to end up in the world reference frame, where the drawing
//    will take place
void cameraToWorld(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //    Move to the origin of the world reference frame
    glTranslatef(gTx, gTy, gTz);
    //    Apply rotations.  Here Roll, Yaw, Pitch
    glRotatef(gRoll, 0.0, 0.0, 1.0);
    glRotatef(gYaw, 0.0, 1.0, 0.0);
    glRotatef(gPitch, 1.0, 0.0, 0.0);
}


void myDisplayFunc(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    Move from the camera to the world reference frame
    cameraToWorld();
    
    glPushMatrix();

    //    Now I am in the world reference frame.
    //    I draw axes so that we can see what the local frame looks like.
//    if (drawReferenceFrames)
    drawReferenceFrame();
    
    for (auto obj : objList)
        obj->draw();
        
    //    back to camera reference frame
    glPopMatrix();

    //    We were drawing into the back buffer, now it should be brought
    //    to the forefront.
    glutSwapBuffers();
}

//    This callback function is called when the window is resized
//    (generally by the user of the application).
//
void myResizeFunc(int w, int h)
{
    winWidth = w;
    winHeight = h;
    
    //    update projection transformations
    glViewport(0, 0, w, h);
    
    // the aspect ratio probably changed, so we need to update
    //    the camera
    setupCamera();
    
    //    When it's done, request a refresh of the display
    glutPostRedisplay();
}


//    This function is called when a mouse event occurs.  This event, of type s
//    (up, down, dragged, etc.), occurs on a particular button of the mouse.
//
void myMouseFunc(int b, int s, int x, int y)
{
    switch (b)
    {
        case GLUT_LEFT_BUTTON:
            if (s == GLUT_DOWN)
            {
                //    do something
            }
            else if (s == GLUT_UP)
            {
                // do something else
            }
            break;


        default:
            break;
    }
}

//    This callback function is called when a keyboard event occurs
//
void myKeyboardFunc(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 27:
            exit(0);
            break;
        
        /*
        //    Toggles on/off rendering the reference frames
        case 'r':
        case 'R':
            drawReferenceFrames = !drawReferenceFrames;
            break;
            
        //    Toggles on/off wireframe mode
        case 'x':
        case 'X':
            switch(renderingMode)
            {
                //    If we were in wireframe mode, we switch to flat shading
                case RenderingMode::WireframeRender:
                    renderingMode = RenderingMode::FlatShadingRender;
                    break;
                //    Else we switch to wireframe
                case RenderingMode::FlatShadingRender:
                case RenderingMode::SmoothShadingRender:
                    renderingMode = RenderingMode::WireframeRender;
                    break;
            }
            updateRenderingMode(renderingMode);
            glutPostRedisplay();
            break;
            

        //    Toggles on/off the Flat shading model
        case 'f':
        case 'F':
            switch(renderingMode)
            {
                //    If we were in wireframe mode or smooth shading mode,
                //    we switch to flat shading
                case RenderingMode::WireframeRender:
                case RenderingMode::SmoothShadingRender:
                    renderingMode = RenderingMode::FlatShadingRender;
                    break;

                //    Else, we switch from flat to smooth shading
                case RenderingMode::FlatShadingRender:
                    renderingMode = RenderingMode::SmoothShadingRender;
                    break;
            }
            updateRenderingMode(renderingMode);
            glutPostRedisplay();
            break;

        //    Toggles on/off the Smooth shading model
        case 'z':
        case 'Z':
            switch(renderingMode)
            {
                //    If we were in wireframe mode or flat shading mode,
                //    we switch to smooth shading
                case RenderingMode::WireframeRender:
                case RenderingMode::FlatShadingRender:
                    renderingMode = RenderingMode::SmoothShadingRender;
                    break;

                //    else we switch from smooth to flat shading
                case RenderingMode::SmoothShadingRender:
                    renderingMode = RenderingMode::FlatShadingRender;
                    break;
            }
            updateRenderingMode(renderingMode);
            glutPostRedisplay();
            break;
    
        case 'y':
        case 'Y':
            gNearZ *= 0.9f;
            setupCamera();
            cout << "nearZ = " << gNearZ << endl;
            break;

        case 'h':
        case 'H':
            gNearZ *= 1.1f;
            setupCamera();
            cout << "nearZ = " << gNearZ << endl;
            break;
            
        case 'u':
        case 'U':
            gFarZ *= 0.9f;
            setupCamera();
            cout << "nearZ = " << gFarZ << endl;
            break;

        case 'j':
        case 'J':
            gFarZ *= 1.1f;
            setupCamera();
            cout << "nearZ = " << gFarZ << endl;
            break;
       */
            
        // move the camera forward
        case 'w':
        case 'W':
            gTz += 0.1f; // Adjust the value as needed
            break;

        // move the camera back
        case 's':
        case 'S':
            gTz -= 0.1f; // Adjust the value as needed
            break;

        // move camera left
        case 'a':
        case 'A':
            gTx += 0.1f; // Adjust the angle as needed
            break;

        // move right
        case 'd':
        case 'D':
            gTx -= 0.1f; // Adjust the angle as needed
            break;

        // move the camera up
        case 'e':
        case 'E':
            gTy -= 0.1f; // Adjust the value as needed
            break;

        // move the camera down
        case 'q':
        case 'Q':
            gTy += 0.1f; // Adjust the value as needed
            break;

        // Tilt the camera up (increase pitch)
        case 'r':
        case 'R':
            gPitch += 5.0f; // Adjust the angle as needed
            break;

        // Tilt the camera down (decrease pitch)
        case 'f':
        case 'F':
            gPitch -= 5.0f; // Adjust the angle as needed
            break;

        default:
            break;
    }
}


//    This function is called after (at least) the specified time has ellapsed
//    since it was last "primed" (pushed back on the queue).
//
void myTimerFunc(int dt)
{
    //    Set up next timer event
    glutTimerFunc(10, myTimerFunc, 0);

    glutPostRedisplay();
}


void myInit(void)
{
    GLfloat ambientLight[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat diffuseLight[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat positionLight[] = {0.0, 3.0, 3.0, 0.0};

    GLfloat lmodel_ambientLight[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat local_view[] = {0.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambientLight);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    updateRenderingMode(renderingMode);

    //     replaces the gluOrtho2D call.
    setupCamera();
    
 //   objList.push_back(make_shared<Quad3D>(1.f, 1.5f, Pose{-1.f, 0.f, 0.f, 15.f, 0.f, -15.f}));
    //objList.push_back(make_shared<QuadMesh3D>(1.f, 1.5f, 6, 8, Pose{-0.25f, 0.2f, 0.f, -15.f, 0.f, 15.f}));
    //objList.push_back(make_shared<QuadMesh3D>(1.f, 1.5f, 6, 8, 0.10f, Pose{-0.25f, 0.2f, 0.f, -15.f, 0.f, 15.f}));
    //objList.push_back(make_shared<Cylinder3D>(0.5f, 0.5f, 1.f, 12, 8, true, Pose{0.f, 2.f, -0.5f, 45.f, 45.f, 0.f}));
    
    
    
    objList.push_back(make_shared<Disc3D>("/Users/michaelfelix/Documents/GitHub/csc406/project/piece.obj", 1.0, 1.0, Pose{-1.f, 0.f, 0.f, 15.f, 0.f, -15.f}));
    objList.back()->setMaterial(gray1);
}

void setupCamera(void)
{
    glMatrixMode(GL_PROJECTION );
    glLoadIdentity();

    float foVxy = 1.0f * winWidth / winHeight;
    
        //    vertical field   hor to vert ratio
        //    of view
    gluPerspective(gFoV, foVxy, gNearZ, gFarZ);
    glMatrixMode( GL_MODELVIEW );
}


int main(int argc, char** argv)
{
    //    Initialize glut and create a new window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 40);
    glutCreateWindow("3D Graphic objects Demo");
    
    //    set up the callbacks
    glutDisplayFunc(myDisplayFunc);
    glutReshapeFunc(myResizeFunc);
    glutMouseFunc(myMouseFunc);
    glutKeyboardFunc(myKeyboardFunc);
    glutTimerFunc(10, myTimerFunc, 0);
    
    //    Now we can do application-level
    myInit();

    //    Now we enter the main loop of the program and to a large extend
    //    "loose control" over its execution.  The callback functions that
    //    we set up earlier will be called when the corresponding event
    //    occurs
    glutMainLoop();
    
    //    This will never be executed (the exit point will be in one of the
    //    call back functions).
    return 0;
}
