#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

void checkError(const char* label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s - OpenGL error %d: %s\n", label, error, gluErrorString(error));
    }
}

void drawCircle(float centerX, float centerY, float radius, int segments, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        for (int i = 0; i < segments; i++)
        {
            float angle = 2.0f * M_PI * i / segments;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            glVertex2f(centerX + x, centerY + y);
        }
    glEnd();
    checkError("drawColoredCircle");
}

void drawRadialGradientCircle(float centerX, float centerY, float radius, int segments)
{
    glColor3f(1.0, 0.0, 0.0); // Colore rosso
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY);

        for (int i = 0; i <= segments; i++)
        {
            float angle = 2.0f * M_PI * i / segments;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            
            float dist = sqrt(x * x + y * y) / radius; // Calcolo distanza dal centro del cerchio
            glColor3f(1.0 - dist, 0.0, dist); // Interpolazione radiale tra il colore rosso e blu
            
            glVertex2f(centerX + x, centerY + y);
        }

    glEnd();
    checkError("drawRadialGradientCircle");
}

// Esercizio 1
void drawGradientCircle(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawRadialGradientCircle(0.0, 0.0, 0.5, 20); // Cerchio di colore blu con rosso sfumato
    glFlush();
}

// Esercizio 2
void drawRadialGradientExagon(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawRadialGradientCircle(0.0, 0.0, 0.5, 6); // Cerchio di colore blu con rosso sfumato
    glFlush();
}

// Esercizio 3
void drawCircleWithInnerBlackCircle(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle(0.0, 0.0, 0.5, 20, 0.0, 0.0, 1.0); // Cerchio di colore blu
    drawCircle(0.0, 0.0, 0.2, 20, 0.0, 0.0, 0.0); // Cerchio di colore nero
    glFlush();
}

// Esercizio 4
void drawGradientCircleWithInnerBlackCircle(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawRadialGradientCircle(0.0, 0.0, 0.5, 20); // Cerchio di colore blu con rosso sfumato
    drawCircle(0.0, 0.0, 0.2, 20, 0.0, 0.0, 0.0); // Cerchio di colore nero
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '1':
            drawGradientCircle();
            glutPostRedisplay();
            break;
        case '2':
            drawRadialGradientExagon();
            glutPostRedisplay();
            break;
        case '3':
            drawCircleWithInnerBlackCircle();
            glutPostRedisplay();
            break;
        case '4':
            drawGradientCircleWithInnerBlackCircle();
            glutPostRedisplay();
            break;
        case 27: // ESC
            exit(0);
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle");

    printf("==== Circle Exercises ====\n");
    printf("Press a key to select an exercise:\n");
    printf("1    -> Exercise 1 - Circle with radial gradient.\n");
    printf("2    -> Exercise 2 - Hexagon with radial gradient.\n");
    printf("3    -> Exercise 3 - Blue circle with inner black circle.\n");
    printf("4    -> Exercise 4 - Radial gradient circle + inner black circle.\n");
    printf("ESC  -> Exit the program.\n\n");

    drawGradientCircle();

    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}