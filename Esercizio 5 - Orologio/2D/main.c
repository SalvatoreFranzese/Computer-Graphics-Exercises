#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// Angoli lancetta dei minuti e delle ore
float minuteAngle = 0.0f;
float hourAngle = 0.0f;

// Variabili gestione mouse
int lastX = 0;
int mouseDown = 0;

void checkError(const char* label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s - OpenGL error %d: %s\n", label, error, gluErrorString(error));
    }
}

void drawColoredCircle(float centerX, float centerY, float radius, int segments, float r, float g, float b)
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

void drawLine(float centerX, float centerY, float angle, float length, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_LINES);
        float x = length * cos(angle + M_PI / 2.0f);
        float y = length * sin(angle + M_PI / 2.0f);

        glVertex2f(centerX, centerY);
        glVertex2f(centerX + x, centerY + y);
    glEnd();
    checkError("drawLine");
}

void drawNumber(float radius)
{
    for (int i = 1; i <= 12; i++)
    {
        // Calcolo angolo per ogni numero
        float angle = M_PI / 2.0f - i * (2.0f * M_PI / 12.0f);

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(x - 0.015f, y - 0.015f);

        if (i >= 10)
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '1');
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '0' + (i - 10));
        }
        else
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, '0' + i);
        }
    }
}

void drawScene(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Quadrante dell'orologio
    drawColoredCircle(0.0f, 0.0f, 0.5, 50, 1.0f, 1.0f, 1.0f);

    // Lancetta ore
    drawLine(0.0f, 0.0f, hourAngle, 0.25f, 0.0f, 0.0f, 0.0f);

    // Lancetta minuti
    drawLine(0.0f, 0.0f, minuteAngle, 0.4, 0.0f, 0.0f, 0.0f);

    // Disegna i numeri da 1 a 12 sul quadrante
    drawNumber(0.45f);

    glFlush();
    checkError("drawScene");
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseDown = 1;
            lastX = x;
        }
        else
        {
            mouseDown = 0;
        }
    }
}

void motion(int x, int y)
{
    if (mouseDown)
    {
        minuteAngle += (x - lastX) * 0.01f;

        // Quando la lancetta dei minuti fa un giro completo (2π) -> muove lancetta delle ore
        if (minuteAngle >= 2 * M_PI)
        {
            minuteAngle -= 2 * M_PI;
            hourAngle += M_PI / 6.0f; // 30° = 1 ora
        }

        // Se la rotazione avviene in senso opposto -> la lancetta delle ore torna indietro
        if (minuteAngle < 0)
        {
            minuteAngle += 2 * M_PI;
            hourAngle -= M_PI / 6.0f; // Arretramento di un'ora
        }

        lastX = x;

        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Clock");

    printf("==== 2D Clock Controls  ====\n");
    printf("left mouse drag  -> Rotate minute.\n");
    printf("ESC              -> Exit the program.\n\n");

    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}