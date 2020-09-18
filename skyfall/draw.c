#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "draw.h"
#include "image.h"

// Broj prepreki i pi za rotaciju igracevih ruku i nogu
#define PREPREKE 2000
#define PI 3.14

//Maksimalna duzina niske za rezultat
#define MAX 30

static GLuint textures[3];
#define FILENAME0 "sky1.bmp"
#define FILENAME1 "game_over.bmp"
#define FILENAME2 "poz2.bmp"

// Promenljiva za nasumicno kreiranje prepreka
static time_t t;

// Promeljive koje oznacavaju trenutnu poziciju igraca
static float fall_x_curr = 0;
static float fall_y_curr = 10;
static float fall_z_curr = 0;

// Promenljiva za ugao poeranja ruku i nogu
static float ugao = 0;

// Promenljive kriscene pri pozicioniranju tesktura za igru i game_over
static float priblizi = 0;
static int skaliraj_x = 0;
static int skaliraj_y = 0;

// Promeljive koje oznacavaju pocetnu poziciju igraca i koriscene na pocetku izrade projekta za laksu orjentaciju
// static float start_position_x = 0;
// static float start_position_y = 0;
// static float start_position_z = 0;

// Promenljive koje sluze za pomeranje prepreka
// static float move_heli_x;
static float move_cloud_x = 0;

static float x;
static float y;
static float z;

// Polozaj i vrsta prepreke
static float prepreke_po_x[PREPREKE];
static float prepreke_po_y[PREPREKE];
static int izbor_prepreke[PREPREKE];

// Indikator uspesnosti
static int ind_usp = 0;

// Pomeraj pozadine
static float tex_move_y = 0;

// Semafor promenljive
static int score = 0;
static int max_score = 0;
static float score_y = 10;

// Prethodno procitana prepreka po y koja je trebala da sluzi pri koriscenju dodecaedra torusa ili druhih objekata u vidu poena
// static int p4y = 0;

void tex_init() {
    Image *image;

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_MODULATE);

    image = image_init(0, 0);

    glGenTextures(3, textures);

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

    image_read(image, FILENAME1);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
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

    image_read(image, FILENAME2);

    glBindTexture(GL_TEXTURE_2D, textures[2]);
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

// Inicijalizacija senski osvetljenja tesktura i svega potrebno za dalji i koriscenje u igrici
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

// Za lakse orjentisanje prilikom izradjivanja projekta pozicioniranja objekata i igraca
// Linija po x osi je zelena po y osi je crvena i po z osi je plava
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

// Funkcija koja se izvrsava prilikom pritiskanja dugmenta 'R' ili u slucaju kolizije
void game_reset_function() {
    fall_x_curr = 0;
    fall_y_curr = 10;
    fall_z_curr = 0;
    tex_move_y = 0;
    score = 0;
    score_y = 10;
    ind_usp = 0;
}

// void draw_wire_sphere_start_position(){
//     glPushMatrix();
//         glColor3f(1, 0, 0);
//         glTranslatef(start_position_x, start_position_y, start_position_z);
//         glutWireSphere(0.2,40,40);
//     glPopMatrix();
// }

// Prvobitno za crtanje loptice al kasnije pretvorena u funkciju za pomeranje coveculjka
// U slucaju da dodje do ivica ekrana vraca se minimalno unazad da bi mogao ponovo da se krece
void draw_wire_sphere(float x2, float y2, float z2) {
    glPushMatrix();
        glColor3f(1, 0, 0);
        if(fall_x_curr > -19.49 && fall_x_curr < 19.49)
            fall_x_curr += x2;
        if(fall_x_curr > 19.49 )
            fall_x_curr -= 0.1;
        else if (fall_x_curr < -19.49 )
            fall_x_curr += 0.1;
        fall_y_curr += y2;
        fall_z_curr += z2;
        glTranslatef(fall_x_curr, fall_y_curr, fall_z_curr);
        // glutWireSphere(0.2,40,40);
    glPopMatrix();
}

// Funckija za iscrtavanje oblaka
void draw_cloud(int x, int pos_x, int pos_y)
{
    // Igrica u padu svi objekti su ispod x ose
    pos_y = -pos_y;
    glPushMatrix();
        move_cloud_x += x;
        glTranslatef(move_cloud_x, -5, 0);
        glBegin(GL_POLYGON);
            glColor3f(0.9, 0.9, 0.9);
            glScalef(0.1, 0.1, 0.1);
            // Za iscrtavanje levo od y ose
            if (pos_x > 0) {
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
            }
            // Za iscrtavanje desno od y ose
            else {
                glVertex3f(-2.2 + pos_x, 2.5 + pos_y, 0.0);
                glVertex3f(-3.0 + pos_x, 2.0 + pos_y, 0.0);
                glVertex3f(-8.0 + pos_x, 2.0 + pos_y, 0.0);
                glVertex3f(-8.5 + pos_x, 2.5 + pos_y, 0.0);
                glVertex3f(-8.0 + pos_x, 3.0 + pos_y, 0.0);
                glVertex3f(-7.0 + pos_x, 3.5 + pos_y, 0.0);
                glVertex3f(-6.5 + pos_x, 3.6 + pos_y, 0.0);
                glVertex3f(-6.0 + pos_x, 4.0 + pos_y, 0.0);
                glVertex3f(-4.0 + pos_x, 4.0 + pos_y, 0.0);
                glVertex3f(-3.5 + pos_x, 3.6 + pos_y, 0.0);
                glVertex3f(-3.1 + pos_x, 3.6 + pos_y, 0.0);
                glVertex3f(-2.5 + pos_x, 3.1 + pos_y, 0.0);
            }
        glEnd();
    glPopMatrix();
}

//Funkcija za iscrtavanje aviona (Neiskoriscena)
// void draw_airplane()
// {
//     glPushMatrix();
//         glColor3f(0.97, 0.70 , 0.50);

//         //Telo aviona
//         glBegin(GL_POLYGON);

//             glVertex3f(4.0, -0.5, 0.0);
//             glVertex3f(4.0, 0, 0.0);
//             glVertex3f(3.2, 0.6, 0.0);
//             glVertex3f(-2.85, 0.5, 0.0);
//             glVertex3f(-3.5, -0.65, 0.0);
//             glVertex3f(3.2, -0.8, 0.0);
//         glEnd();

//         //Rep aviona
//         glBegin(GL_POLYGON);
            
//             glVertex3f(-2.85, 0.5, 0.0);
//             glVertex3f(-3.35, 1.6, 0.0);
//             glVertex3f(-4.2, 1.67, 0.0);
//             glVertex3f(-3.5, -0.65, 0.0);
//         glEnd();
        
//         //Krilo 1
//         glBegin(GL_POLYGON);

//             glVertex3f(1.2, -0.2, 0);
//             glVertex3f(0.6, -0.2, -5);
//             glVertex3f(-0.5, -0.2, -5);
//             glVertex3f(-0.2, -0.2, 0);
//         glEnd();

//         glBegin(GL_POLYGON);

//             glVertex3f(1.2, -0.2, 0);
//             glVertex3f(0.2, -0.2, 5);
//             glVertex3f(-0.4, -0.2, 5);
//             glVertex3f(-0.2, -0.2, 0);
//         glEnd();
//     glPopMatrix();
// }

// Iscrtavanje neidentifikovanog leteceg objekta
void draw_UFO(int pos_x, int pos_y)
{
    // Zato sto se igrica izvrsava u padu pa svi objekti trebaju biti iscrtani ispod x ose
    pos_y = -pos_y;
    glPushMatrix();
        
        glTranslatef(0.0, -3.0, 0.0);
        
        // Za iscrtavanje levo od y ose
        if (pos_x > 0) {
            glBegin(GL_POLYGON);
                glColor3f(0.5, 0.5, 0.5);
                glVertex3f(2.0 + pos_x, 0 + pos_y, 0.0);
                glVertex3f(2.5 + pos_x, -0.5 + pos_y, 0.0);
                glVertex3f(5.5 + pos_x, -0.5 + pos_y, 0.0);
                glVertex3f(6.0 + pos_x, 0 + pos_y, 0.0);
            glEnd();

            glBegin(GL_POLYGON);
                glColor3f(0.5, 0.5, 0.5);
                glVertex3f(6.0 + pos_x, 0 + pos_y, 0.0);
                glVertex3f(2.0 + pos_x, 0 + pos_y, 0.0);
                glVertex3f(2.5 + pos_x, 0.5 + pos_y, 0.0);
                glVertex3f(5.5 + pos_x, 0.5 + pos_y, 0.0);
            glEnd();

            glBegin(GL_POLYGON);
                glColor3f(0.5f, 1.0f, 0.0f);
                glVertex3f(3.0 + pos_x, 0.5 + pos_y, 0.0);
                glVertex3f(3.0 + pos_x, 1.5 + pos_y, 0.0);
                glVertex3f(3.5 + pos_x, 2.0 + pos_y, 0.0);
                glVertex3f(4.5 + pos_x, 2.0 + pos_y, 0.0);
                glVertex3f(5.0 + pos_x, 1.5 + pos_y, 0.0);
                glVertex3f(5.0 + pos_x, 0.5 + pos_y, 0.0);
            glEnd();
        }
        // Za iscrtavanje desno od y ose
        else {
            glBegin(GL_POLYGON);
                glColor3f(0.5, 0.5, 0.5);
                glVertex3f(-2.0 + pos_x, 0 + pos_y, 0.0);
                glVertex3f(-2.5 + pos_x, -0.5 + pos_y, 0.0);
                glVertex3f(-5.5 + pos_x, -0.5 + pos_y, 0.0);
                glVertex3f(-6.0 + pos_x, 0 + pos_y, 0.0);
            glEnd();

            glBegin(GL_POLYGON);
                glColor3f(0.5, 0.5, 0.5);
                glVertex3f(-6.0 + pos_x, 0 + pos_y, 0.0);
                glVertex3f(-2.0 + pos_x, 0 + pos_y, 0.0);
                glVertex3f(-2.5 + pos_x, 0.5 + pos_y, 0.0);
                glVertex3f(-5.5 + pos_x, 0.5 + pos_y, 0.0);
            glEnd();

            glBegin(GL_POLYGON);
                glColor3f(0.5f, 1.0f, 0.0f);
                glVertex3f(-3.0 + pos_x, 0.5 + pos_y, 0.0);
                glVertex3f(-3.0 + pos_x, 1.5 + pos_y, 0.0);
                glVertex3f(-3.5 + pos_x, 2.0 + pos_y, 0.0);
                glVertex3f(-4.5 + pos_x, 2.0 + pos_y, 0.0);
                glVertex3f(-5.0 + pos_x, 1.5 + pos_y, 0.0);
                glVertex3f(-5.0 + pos_x, 0.5 + pos_y, 0.0);
            glEnd();
        }
    glPopMatrix();
}

// Iscrtavanje meseca na pocetnom ekranu
void draw_moon()
{
    glPushMatrix();
        glColor3f(0.5, 0.5, 0.5);
        glTranslatef(-18, 10, 0);
        glutSolidSphere(1.1, 30, 30);
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

// Funkcija za iscrtavanje semafora
void draw_semaphore_before_starting(float *start_parameter)
{
    char instructions[MAX];

    if (score > max_score)
            max_score = score;

    if(!*start_parameter){
        int x = 16;
        float y = 10;
        int z = 0;
        sprintf(instructions, "Max Score: %d", max_score);
        renderBitmapString(x, y, z, GLUT_BITMAP_HELVETICA_18, instructions);
    }
}

// Kreiranje prepreka nasumicno po pozicijama i po oblicima
void pozicija_prepreka() {
    srand(time(&t));

    for (int i = 20 ; i < 2000 ; i++) {
        int pos = (int)rand() % 15;
        prepreke_po_x[i] = pos;
        prepreke_po_y[i] = i;
        izbor_prepreke[i] = rand() % 12;
    }
}

// Funckija koja na osnovu dodeljene vrednosti bira dal ce prepreka biti oblak, nlo ili torus
void izbor_prepreka() {
    for (int i = 20 ; i < 2000 ; i++){
        if (prepreke_po_y[i] > 20) {
            if (izbor_prepreke[i] == 1 || izbor_prepreke[i] == 11) {
                if (i % 2) {
                    draw_cloud(0, -prepreke_po_x[i], prepreke_po_y[i]);
                } else   
                    draw_cloud(0, prepreke_po_x[i], prepreke_po_y[i]);
            } else if (izbor_prepreke[i] == 7) {
                if (i % 2) {
                    draw_UFO(-prepreke_po_x[i], prepreke_po_y[i]);
                } else   
                    draw_UFO(prepreke_po_x[i], prepreke_po_y[i]);
            } 
            // else if (izbor_prepreke[i] == 5) {
            //     if (i % 2)
            //         draw_cube(-prepreke_po_x[i], prepreke_po_y[i]);
            //     else {
            //         draw_cube(prepreke_po_x[i], prepreke_po_y[i]);
            //     }
            // }
        }
    }
}

// Funckija za promeljivu indikatora uspesnosti i nastavka igre
// U slucaju kada noge coveculjka dodirnu vrh ili oblaka ili nlo-a(ufo) igrica se prekida
// Raspon crtanja oblaka je 2.2 plus pozicija na kojoj se nalazi do 8.5 plus pozicija na kojoj se nalazi po x osi
// Za crtanje ufo je na slican nacin samo sto je raspon crtanja 2 plus pozicija i 6 plus pozicija
// Posto se objekti moraju nalaziti se obe strane onda postoje i dve provere u kojima se radi provera od -8.5 do -2.2 za oblak i -6 do -2 za ufo plus pozicija po x
// Napravljena je i varijanta za prepreke koje su prvobitno trebale da budu poeni ali finalno broj poena zavisi od duzine pazada po promenljivoj fall_y_curr
int kolizija() {

    if (-fall_y_curr < 20)
            return 0;
    else
        score = -fall_y_curr;

    // Provera kolizije sa oblikom   
    if (izbor_prepreke[(int)-fall_y_curr] == 1 || izbor_prepreke[(int)-fall_y_curr] == 11) {
        if (prepreke_po_y[(int)-fall_y_curr] <= -fall_y_curr && prepreke_po_y[(int)-fall_y_curr] + 0.99 > -fall_y_curr) {
            if ((int)prepreke_po_y[(int)-fall_y_curr] % 2 == 0) {
                if (prepreke_po_x[(int)-fall_y_curr] + 2.2 <= fall_x_curr && prepreke_po_x[(int)-fall_y_curr] + 8.5 >= fall_x_curr) {
                    return 1;
                }
            } else if ((int)prepreke_po_y[(int)-fall_y_curr] % 2 == 1) {
                if (-prepreke_po_x[(int)-fall_y_curr] - 2.2 >= fall_x_curr && -prepreke_po_x[(int)-fall_y_curr] - 8.5 <= fall_x_curr) {
                    return 1;
                }
            }
        }
    // } else if (izbor_prepreke[(int)-fall_y_curr] == 5) {
    //     printf("%f %f %f\n", -fall_y_curr, prepreke_po_y[(int)-fall_y_curr]);
    //     if (prepreke_po_y[(int)-fall_y_curr] == -fall_y_curr && prepreke_po_y[(int)-fall_y_curr] + 0.99 > -fall_y_curr) {
    //         if ((int)prepreke_po_y[(int)-fall_y_curr] % 2 == 0) {
    //             if (prepreke_po_x[(int)-fall_y_curr] - 1 <= fall_x_curr && prepreke_po_x[(int)-fall_y_curr] + 1 >= fall_x_curr) {
    //                 score++;
    //                 printf("%d\n", score);
    //             }
    //         } else if ((int)prepreke_po_y[(int)-fall_y_curr] % 2 == 1) {
    //             if (-prepreke_po_x[(int)-fall_y_curr] - 1 <= fall_x_curr && -prepreke_po_x[(int)-fall_y_curr] + 1  >= fall_x_curr) {
    //                 score++;
    //                 printf("%d\n", score);
    //             }
    //         }
    //     }
    }
    // Provera kolizije sa nlo-om(ufo-om) 
    else if (izbor_prepreke[(int)-fall_y_curr] == 7) {
        if (prepreke_po_y[(int)-fall_y_curr] <= -fall_y_curr && prepreke_po_y[(int)-fall_y_curr] + 1 > -fall_y_curr) {
            if ((int)prepreke_po_y[(int)-fall_y_curr] % 2 == 0) {
                if (prepreke_po_x[(int)-fall_y_curr] + 2 <= fall_x_curr && prepreke_po_x[(int)-fall_y_curr] + 6 >= fall_x_curr) {
                    return 1;
                }
            } else if ((int)prepreke_po_y[(int)-fall_y_curr] % 2 == 1) {
                if (-prepreke_po_x[(int)-fall_y_curr] - 6 <= fall_x_curr && -prepreke_po_x[(int)-fall_y_curr] - 2  >= fall_x_curr) {
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

// Funckija za iscrtavanje pozadine koja moze biti pocetana ili za vreme igre ili game_over pozadina
int draw_background(float y5){
    tex_move_y -= y5/70;
    int lose = 1;

    float ivica = 1.0;

    if (!ind_usp)
        ind_usp = kolizija();
    glPushMatrix();
        glDisable(GL_LIGHTING);
        
        // Indikator na izbor pozadine u slucaju igranja ili game_over-a
        if (!ind_usp) {
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glTranslatef(0,0,-20);
            glScalef(125, 65, 0);
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
                    glNormal3f(1,1,2);
                    glTexCoord2f(0,0);
                    glVertex3f(-ivica/2, (-ivica/2) - tex_move_y, -ivica/2);
                    glTexCoord2f(0, ivica);
                    glVertex3f(-ivica/2, (ivica/2) - tex_move_y , -ivica/2);
                    glTexCoord2f(ivica , ivica);
                    glVertex3f(ivica/2, (ivica/2) - tex_move_y , -ivica/2);
                    glTexCoord2f(ivica, 0);
                    glVertex3f(ivica/2, (-ivica/2) - tex_move_y , -ivica/2);
            glEnd();
        } else {
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glTranslatef(0,0,-20 + 20.5);
            glScalef(55, 25, 0);
            glColor3f(1,1,1);
            glBegin(GL_QUADS);
                    glNormal3f(1,1,2);
                    glTexCoord2f(0,0);
                    glVertex3f(-ivica/2, (-ivica/2) - tex_move_y * 2.59 , -ivica/2);
                    glTexCoord2f(0, ivica);
                    glVertex3f(-ivica/2, (ivica/2) - tex_move_y * 2.59 , -ivica/2);
                    glTexCoord2f(ivica , ivica);
                    glVertex3f(ivica/2, (ivica/2) - tex_move_y * 2.59 , -ivica/2);
                    glTexCoord2f(ivica, 0);
                    glVertex3f(ivica/2, (-ivica/2) - tex_move_y * 2.59 , -ivica/2);
            glEnd();
        }
        glBindTexture(GL_TEXTURE_2D,0);
        glEnable(GL_LIGHTING);
    glPopMatrix();
    
    // Iscratavanje pocetne pozadine Skyfall
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glTranslatef(0,0,-5);
        glScalef(60,40, 0);
        glColor3f(1,1,1);

        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glBegin(GL_QUADS);
                    glNormal3f(1,1,2);
                    glTexCoord2f(0,0);
                    glVertex3f(-ivica/2, -ivica/2, 1.0);
                    glTexCoord2f(0, ivica);
                    glVertex3f(-ivica/2, ivica/2, 1.0);
                    glTexCoord2f(ivica , ivica);
                    glVertex3f(ivica/2, ivica/2, 1.0);
                    glTexCoord2f(ivica, 0);
                    glVertex3f(ivica/2, -ivica/2, 1.0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0);
        glEnable(GL_LIGHTING);
    glPopMatrix();
    
    return ind_usp;
}

// Torus za prikupljanje poena prilikom prolaska kroz isti
// void draw_torus(float xt, float yt)
// {
//     glPushMatrix();
//         glColor3f(0, 0, 1);
//         glRotatef(45, 0, 0, 0);
//         glTranslatef(xt, -yt, -yt);
//         glutSolidTorus(1.0f, 0.995f, 3, 50);
//     glPopMatrix();
// }

// Dodecaedar za prikupljanje poena prilikom prolaska kroz isti
// Zamisljena je prvo kao kocka pa dodecaedar ili torus ali je na kraju skor zavisio samo od pada
void draw_cube(float xt, float yt)
{
    glPushMatrix();
        glColor3f(0, 0, 1);
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(xt, -yt, -yt);
        glutWireCube(1);
    glPopMatrix();
}

// Iscrtavanje igraca tj coveculjka sa kojim je zamenjana loptica koja se prvobitno korsitila za proveru i lakse orjentisanje za pravljenje igrice
// Coveculjak je napravljen iz delova odvojenih i omoguceno je uz pomog pi vrednosti i promenljive ugao da mu se pomera ruke i noge levo desno i napred nazad
void draw_person()
{
    GLfloat material_diffuse1[] = {0.3, 0.45, 0.8, 1};
    GLfloat material_diffuse2[] = {0.8, 0.77, 0.1, 1};
    GLfloat material_diffuse3[] = {1, 0.4, 0.59, 0.9};

    // Ugao pomeranja nogu i ruku
    ugao += 1;
    if (ugao > 360)
        ugao = 0;

    glPushMatrix();
        glRotatef(180, 0, 0, 0);
        glTranslatef(0, -1, 0);
        
        //glava
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse3);
        glPushMatrix();
        glColor4f(0.75f, 0.45f, 0.0f, 0.0f);
        glTranslatef(fall_x_curr, fall_y_curr + 1.6, fall_z_curr);
        glutSolidSphere(0.3, 10, 10);
        glPopMatrix();
        //telo
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse2);
        glPushMatrix();
        glColor3f(0.0f, 0.5f, 1.0f);
        glTranslatef(fall_x_curr, fall_y_curr + .9, fall_z_curr);
        glScalef(.65, .8, .4);
        glutSolidCube(1);
        glPopMatrix();
        //noge
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse1);
        glPushMatrix();
        glColor4f(0.75f, 0.45f, 0.0f, 0.0f);
        glTranslatef(fall_x_curr + .15, fall_y_curr + .3, fall_z_curr);
        glScalef(.5, 1.7, .1);
        glPushMatrix();
        glRotatef(20 * sin(30 * ugao * PI / 180), 1, 0, 0);
        glutSolidCube(.5);
        glPopMatrix();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(fall_x_curr - .15, fall_y_curr + .3, fall_z_curr);

        glScalef(.5, 1.7, .1);
        glPushMatrix();
        glRotatef(20 * sin(30 * ugao * PI / 180 + PI), 1, 0, 0);
        glutSolidCube(.5);
        glPopMatrix();
        glPopMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse2);

        //ruke
        glPushMatrix();
        glColor4f(0.75f, 0.45f, 0.0f, 0.0f);
        glTranslatef(fall_x_curr - .35, fall_y_curr + 1, fall_z_curr);
        glRotatef(-60, 0, 0, 1);
        glScalef(.2, .5, 0.1);
        glPushMatrix();
        glRotatef(20 * sin(30 * ugao * PI / 180), 0, 2, 0);
        glutSolidCube(1);
        glPopMatrix();

        glPopMatrix();
        glPushMatrix();
        glTranslatef(fall_x_curr + .35, fall_y_curr + 1, fall_z_curr);
        glRotatef(60, 0, 0, 1);
        glScalef(.2, .5, 0.1);
        glPushMatrix();
        glRotatef(20 * sin(30 * ugao * PI / 180 + PI), 0, 2, 0);
        glutSolidCube(1);
        glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}