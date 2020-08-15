#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "draw.h"
#include "image.h"

#define PREPREKE 1000

//Maksimalna duzina niske za instrukcije
#define MAX 30

static GLuint textures[1];
#define FILENAME0 "background.bmp"

// Promenljiva za nasumicno kreiranje prepreka
static time_t t;

// Promenljive za ispisivanje rezultata
static int max_fall = 0;
static int current_fall;

// Promeljive koje oznacavaju trenutnu poziciju igraca
static float fall_x_curr = 0;
static float fall_y_curr = 10;
static float fall_z_curr = 0;

// Promeljive koje oznacavaju pocetnu poziciju igraca
// static float start_position_x = 0;
// static float start_position_y = 0;
// static float start_position_z = 0;

// Promenljive koje sluze za pomeranje prepreka
static float move_heli_x;
static float move_cloud_x = 0;

static float x;
static float y;
static float z;

static float prepreke_po_x[PREPREKE];
static float prepreke_po_y[PREPREKE];
static int izbor_prepreke[PREPREKE];

void tex_init() {
    Image *image;

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_MODULATE);

    image = image_init(0, 0);

    glGenTextures(1, textures);

    image_read(image, FILENAME0);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    image_done(image);
}

void initialize() {
    srand(time(NULL));

    tex_init();

    GLfloat light_position[] = {0, 40, 0, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat light_ambient[] = { 0.4, 0.4, 0.4, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat ambient_coeffs[] = { 0.3, 0.5, 0.3, 1 };
    GLfloat diffuse_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat specular_coeffs[] = { 0.5, 0.5, 0,5, 1 };

    GLfloat shininess = 30;

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

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

void game_reset_function() {
    fall_x_curr = 0;
    fall_y_curr = 10;
    fall_z_curr = 0;
}

// void draw_wire_sphere_start_position(){
//     glPushMatrix();
//         glColor3f(1, 0, 0);
//         glTranslatef(start_position_x, start_position_y, start_position_z);
//         glutWireSphere(0.2,40,40);
//     glPopMatrix();
// }

void draw_wire_sphere(float x2, float y2, float z2) {
    glPushMatrix();
        glColor3f(1, 0, 0);
        fall_x_curr += x2;
        fall_y_curr += y2;
        fall_z_curr += z2;
        glTranslatef(fall_x_curr, fall_y_curr, fall_z_curr);
        glutWireSphere(0.2,40,40);
    glPopMatrix();
}

//Funckija za iscrtavanje oblaka
void draw_cloud(int x, int pos_x, int pos_y)
{
    pos_y = -pos_y;
    glPushMatrix();
        move_cloud_x += x;
        glTranslatef(move_cloud_x, 0, 0);
        glBegin(GL_POLYGON);
            glColor3f(0.9, 0.9, 0.9);
            glScalef(0.1, 0.1, 0.1);
            glVertex3f(2.2 + pos_x, 2.5 + pos_y, 0.0);
            glVertex3f(3.0 + pos_x, 2.0 + pos_y, 0.0);
            glVertex3f(8.0 + pos_x, 2.0 + pos_y, 0.0);
            glVertex3f(8.5 + pos_x, 2.5 + pos_y, 0.0);
            glVertex3f(8.0 + pos_x, 3.0 + pos_y, 0.0);
            glVertex3f(7.0 + pos_x, 3.5 + pos_y, 0.0);
            glVertex3f(6.5 + pos_x, 3.6 + pos_y, 0.0);
            glVertex3f(6.0 + pos_x, 4.0 + pos_y, 0.0);
            glVertex3f(4.0 + pos_x, 4.0 + pos_y, 0.0);
            glVertex3f(3.5 + pos_x, 3.6 + pos_y, 0.0);
            glVertex3f(3.1 + pos_x, 3.6 + pos_y, 0.0);
            glVertex3f(2.5 + pos_x, 3.1 + pos_y, 0.0);
        glEnd();
    glPopMatrix();
}

//Funkcija za iscrtavanje aviona
void draw_airplane()
{
    glPushMatrix();
        glColor3f(0.97, 0.70 , 0.50);

        //Telo aviona
        glBegin(GL_POLYGON);

            glVertex3f(4.0, -0.5, 0.0);
            glVertex3f(4.0, 0, 0.0);
            glVertex3f(3.2, 0.6, 0.0);
            glVertex3f(-2.85, 0.5, 0.0);
            glVertex3f(-3.5, -0.65, 0.0);
            glVertex3f(3.2, -0.8, 0.0);
        glEnd();

        //Rep aviona
        glBegin(GL_POLYGON);
            
            glVertex3f(-2.85, 0.5, 0.0);
            glVertex3f(-3.35, 1.6, 0.0);
            glVertex3f(-4.2, 1.67, 0.0);
            glVertex3f(-3.5, -0.65, 0.0);
        glEnd();
        
        //Krilo 1
        glBegin(GL_POLYGON);

            glVertex3f(1.2, -0.2, 0);
            glVertex3f(0.6, -0.2, -5);
            glVertex3f(-0.5, -0.2, -5);
            glVertex3f(-0.2, -0.2, 0);
        glEnd();

        glBegin(GL_POLYGON);

            glVertex3f(1.2, -0.2, 0);
            glVertex3f(0.2, -0.2, 5);
            glVertex3f(-0.4, -0.2, 5);
            glVertex3f(-0.2, -0.2, 0);
        glEnd();
    glPopMatrix();
}

void draw_UFO(int pos_x, int pos_y)
{
    pos_y = -pos_y;
    glPushMatrix();
        glTranslatef(0.0, -2.0, 0.0);
        glBegin(GL_POLYGON);
            glColor3f(0.5, 0.5, 0.5);
            glVertex3f(6.0 + pos_x, 0 + pos_y, 0.0);
            glVertex3f(2.0 + pos_x, 0 + pos_y, 0.0);
            glVertex3f(2.5 + pos_x, -0.5 + pos_y, 0.0);
            glVertex3f(5.5 + pos_x, -0.5 + pos_y, 0.0);
        glEnd();

        glBegin(GL_POLYGON);
            glColor3f(0.5, 0.5, 0.5);
            glVertex3f(6.0 + pos_x, 0 + pos_y, 0.0);
            glVertex3f(2.0 + pos_x, 0 + pos_y, 0.0);
            glVertex3f(2.5 + pos_x, 0.5 + pos_y, 0.0);
            glVertex3f(5.5 + pos_x, 0.5 + pos_y, 0.0);
        glEnd();

        glBegin(GL_POLYGON);
            glColor3f(0.9, 0.9, 0.9);
            glVertex3f(3.0 + pos_x, 0.5 + pos_y, 0.0);
            glVertex3f(3.0 + pos_x, 1.5 + pos_y, 0.0);
            glVertex3f(3.5 + pos_x, 2.0 + pos_y, 0.0);
            glVertex3f(4.5 + pos_x, 2.0 + pos_y, 0.0);
            glVertex3f(5.0 + pos_x, 1.5 + pos_y, 0.0);
            glVertex3f(5.0 + pos_x, 0.5 + pos_y, 0.0);
        glEnd();
    glPopMatrix();
}

void draw_moon()
{
    glPushMatrix();
        glColor3f(0.5, 0.5, 0.5);
        glTranslatef(-15, 10, 0);
        glutSolidSphere(0.8, 30, 30);
    glPopMatrix();
}

//Funckija za ispisivanje teksta na ekranu
//Kod preuzet sa interneta.
static void renderBitmapString(int x, int y,int z,void* font, char *string){
    //Duzina stringa.
    int len;

    //Iskljucujemo osvetljenje jer se u suprotnom nece lepo postaviti boja teksta.
    glDisable(GL_LIGHTING);
    
    glColor3f(1,1,1);
    glRasterPos3f(x,y,z);
    len = strlen(string);
    for (int i = 0; i < len; i++) 
    {
        glutBitmapCharacter(font, string[i]);
    }

    //Ponovo ukljucujemo osvetljenje.
    glEnable(GL_LIGHTING);
}

void draw_menu_before_starting(float *start_parameter)
{
    char instructions[MAX];

    if(!*start_parameter){
        sprintf(instructions, "Start on 'SPACE'.");
        int x = 10;
        int y = 10;
        int z = 0;
        renderBitmapString(x, y, z, GLUT_BITMAP_HELVETICA_18, instructions);
        sprintf(instructions, "Move left on 'a' or 'A'");
        renderBitmapString(x, y - 1, z, GLUT_BITMAP_HELVETICA_18, instructions);
        sprintf(instructions, "Move right on 'd' or 'D'");
        renderBitmapString(x, y - 2, z, GLUT_BITMAP_HELVETICA_18, instructions);
    }
}

// Kreiranje prepreka nasumicno
void pozicija_prepreka() {
    srand(time(&t));

    for (int i = 0 ; i < 1000 ; i++) {
        float pos = (float)rand() / (float)(RAND_MAX / 22);
        prepreke_po_x[i] = pos;
        prepreke_po_y[i] = i / 0.8;
        izbor_prepreke[i] = rand() % 8;
    }
}

void izbor_prepreka() {
    for (int i = 5 ; i < 1000 ; i++){
        if (izbor_prepreke[i] == 0 /*|| izbor_prepreke[i] == 3*/) {
            if (i % 2)
                draw_cloud(0, -prepreke_po_x[i], prepreke_po_y[i]);
            else   
                draw_cloud(0, prepreke_po_x[i], prepreke_po_y[i]);
        } else if (izbor_prepreke[i] == 7) {
            if (i % 2)
                draw_UFO(-prepreke_po_x[i], prepreke_po_y[i]);
            else   
                draw_UFO(prepreke_po_x[i], prepreke_po_y[i]);
        }
    }
}