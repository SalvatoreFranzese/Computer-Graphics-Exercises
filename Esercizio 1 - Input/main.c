#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

// Colore di sfondo globale
float r = 0.0;
float g = 0.0;
float b = 0.0;

void redraw(void)
{
    glClearColor(r, b, b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case ' ': // Spazio

            // Genera un colore casuale
            r = (float)rand() / RAND_MAX;
            g = (float)rand() / RAND_MAX;
            b = (float)rand() / RAND_MAX;

            glutPostRedisplay(); // Forza il ridisegno
            break;

        case 27: // ESC
            exit(0);
    }
}

void specialKeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            printf("LEFT\n");
            break;

        case GLUT_KEY_RIGHT:
            printf("RIGHT\n");
            break;

        case GLUT_KEY_UP:
            printf("UP\n");
            break;

        case GLUT_KEY_DOWN:
            printf("DOWN\n");
            break;
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Input");
    glutDisplayFunc(redraw);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}