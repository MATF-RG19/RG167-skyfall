#ifndef DRAW_H
#define DRAW_H

void game_reset_function();
void draw_wire_sphere(float x, float y, float z);
void coordinacion_system_draw();
// void draw_wire_sphere_start_position();
void draw_cloud(int x, int pos_x, int pos_y);
void tex_init();
void initialize();
void draw_semaphore_before_starting();
static void renderBitmapString();
void draw_moon();
// void draw_airplane();
void izbor_prepreka();
void pozicija_prepreka();
void draw_UFO();
int draw_background(float y);
void draw_torus(float x, float y);
void draw_person();
// void draw_game_over();
int kolizija();
void draw_cube(float x, float y);

#endif