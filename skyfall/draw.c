#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "draw.h"
#include "image.h"

static GLuint textures[1];
#define FILENAME1 "background.bmp"

static int max_fall = 0;
static int current_fall;

static float fall_x_prev;
static float fall_y_prev;
static float fall_z_prev;

static float fall_x_curr = 0;
static float fall_y_curr = 0;
static float fall_z_curr = 0;

static float start_position_x = 0;
static float start_position_y = 0;
static float start_position_z = 0;

static float x2;
static float y2;
static float z2;

// void tex_initialization() {
//     Image *image;

//     glEnable(GL_TEXTURE_2D);

//     glTexEnvf(GL_TEXTURE_ENV,
//               GL_TEXTURE_ENV_MODE,
//               GL_MODULATE);

//     image = image_init(0, 0);

//     image_read(image, FILENAME1);

//     glGenTextures(1, textures);

//     glBindTexture(GL_TEXTURE_2D, textures[0]);
//     glTexParameteri(GL_TEXTURE_2D,
//                     GL_TEXTURE_WRAP_S, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D,
//                     GL_TEXTURE_WRAP_T, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D,
//                     GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D,
//                     GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
//                  image->width, image->height, 0,
//                  GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

//     glBindTexture(GL_TEXTURE_2D, 0);

//     image_done(image);
// }

//za lakse orjentisanje prilikom izradjivanja projekta
void coordinacion_system_draw() {
    glPushMatrix();
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
            glVertex3f(200, 0, 0);
            glVertex3f(-200, 0, 0);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
            glVertex3f(0, -200, 0);
            glVertex3f(0, 200, 0);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glColor3f(0, 0, 1);
        glBegin(GL_LINES);
            glVertex3f(0, 0, 200);
            glVertex3f(0, 0, -200);
        glEnd();
    glPopMatrix();
}

void draw_wire_sphere_start_position(){
    glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(start_position_x, start_position_y, start_position_z);
        glutWireSphere(0.2,40,40);
    glPopMatrix();
}

void draw_wire_sphere(int x2, int y2, int z2) {
    glPushMatrix();
        glColor3f(1, 0, 0);
        fall_x_curr += x2;
        fall_y_curr += y2;
        fall_z_curr += z2;
        glTranslatef(fall_x_curr, fall_y_curr, fall_z_curr);
        glutWireSphere(0.2,40,40);
    glPopMatrix();
}

float x=0.0,y=0.0,x1=5.0;

void update()
{
    x+=0.01;
    x1-=0.02;
    if(x>6)
        {
            x=-6;
            x1=4;
    }
}

void draw_cloud()
{
    glPushMatrix();
    glTranslatef(4,-3,0);
    glBegin(GL_POLYGON);
        
        glColor3f(0.9,0.9,0.9);
        
        glVertex3f(-1.0,1.0,-5.0);
        glVertex3f(0.0,2.0,-5.0);
        glVertex3f(-2.0,2.0,-5.0);
        glVertex3f(1.0,1.0,-5.0);
        
    glEnd();
    glPopMatrix();

    glPushMatrix();

    glTranslatef(4,-3,0);
    glBegin(GL_POLYGON);
        
        glColor3f(0.9,0.9,0.9);
        
        glVertex3f(1.0,0.7,-5.0);

        glVertex3f(1.5,1.0,-5.0);

        glVertex3f(0.7,1.5,-5.0);

        glVertex3f(0.0,2.0,-5.0);

        glVertex3f(-0.7,1.5,-5.0);

        glVertex3f(-1.4,1.6,-5.0);

        glVertex3f(-1.7,1.0,-5.0);

        glVertex3f(-1.5,0.7,-5.0);

        glVertex3f(-1.0,0.5,-5.0);
        
    glEnd();

    glPopMatrix();

    // glBegin(GL_POLYGON);
        
    //     glColor3f(1.0,1.0,1.5);
        
    //     glVertex3f(-2.0,-2.0,-5.0);
    //     glVertex3f(-1.0,-1.5,-5.0);
    //     //glVertex3f(0.0,0.0,-5.0);
    //     glVertex3f(2.0,-2.0,-5.0);
    //     glVertex3f(1.2,-1.5,-5.0);
        
    // glEnd();

    update();
}

void draw_terrain() {


}