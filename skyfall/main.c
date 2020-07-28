#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "draw.h"
#include "image.h"

#define TIMER_ID 0
#define TIMER_INTERVAL 100

static int animation_ongoing = 0;

static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);

static float x = 0;
static float y = 0;
static float z = 0;

static float background_color_g = 0.1;
static float background_color_b = 0.4;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Skyfall");

    // tex_initialization();

    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}

//Beskonacno padanje loptice
static void on_timer(int value)
{
    if (value != TIMER_ID)
        return;

    y += -1;

    background_color_b += 0.005;
    if (background_color_g < 0.65)
        background_color_g += 0.001;

    draw_wire_sphere(0, -1, 0);


    //Ponovno iscrtavanje prozora
    glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

//Pomeranje coveculjka po x osi desno
static void on_timer_d(int value)
{
    if (value != TIMER_ID)
        return;

    if (animation_ongoing)
        draw_wire_sphere(1, 0, 0);
    

    //Ponovno iscrtavanje prozora
    glutPostRedisplay();
}

//Pomeranje coveculjka po x osi levo
static void on_timer_a(int value)
{
    if (value != TIMER_ID)
        return;

    if (animation_ongoing)
        draw_wire_sphere(-1, 0, 0);

    //Ponovno iscrtavanje prozora
    glutPostRedisplay();
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 27:
            exit(0);
            break;
        //Reset
        case 'r':
        case 'R':
            animation_ongoing = 0;
            break;
        //Pomeranje coveculjka po x osi desno
        case 'd':
        case 'D':
            glutTimerFunc(TIMER_INTERVAL, on_timer_d, TIMER_ID);
            break;
        //Pomeranje coveculjka po x osi levo
        case 'a':
        case 'A':
            glutTimerFunc(TIMER_INTERVAL, on_timer_a, TIMER_ID);
            break;
        case ' ':
            if (!animation_ongoing) {
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
                animation_ongoing = 1;
            }
            break;
    }
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,background_color_g,background_color_b,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 12 + y, 20,
              0, y, 0,
              0, 1, 0);

    coordinacion_system_draw();

    draw_wire_sphere(0, 0, 0);
    draw_cloud();

    glutSwapBuffers();
}

static void on_reshape(int width, int height){

    glViewport(0,0,width,height);
    

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 1, 1000);
    glutFullScreen();
}
