#include <GL/glut.h>

static void on_display(void);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(500, 489);
    glutInitWindowPosition(500, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);

    glClearColor(0,0, 1, 0);

    glutMainLoop();

    return 0;
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 1, 0);
    glBegin(GL_POLYGON);
        glVertex3f(0.8, -0.8, 0);
        glVertex3f(-0.8, -0.8, 0);
        glVertex3f(-0.8, 0.8, 0);
        glVertex3f(0.8, 0.8, 0);
        
    glEnd();
    glutSwapBuffers();
}
