#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// Valori del Volume di Vista
float width;
float height;
float depth = -3.0f; // Traslazione lungo Z (simula zoom avanti/indietro)

// Angoli di rotazioni iniziali
float angleX = 50.0f;
float angleY = 50.f;

// Variabili gestione mouse
int lastX = 0;
int lastY = 0;
int mouseDown = 0;

void checkError(const char* label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s - OpenGL error %d: %s\n", label, error, gluErrorString(error));
    }
}

// Disegna una circonferenza piena sul piano XY con coordinata Z
void drawCircle(float centerX, float centerY, float zNear, float radius, int segments, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        for (int i = 0; i < segments; i++)
        {
            float angle = 2.0f * M_PI * i / segments;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            glVertex3f(centerX + x, centerY + y, zNear);
        }
    glEnd();
    checkError("drawCircle");
}

// Disegna le facce laterali del prisma
void drawQuad(float centerX, float centerY, float zNear, float zFar, float radius, int segments, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        for (int i = 0; i < segments; i++)
        {
            float angle1 = 2.0f * M_PI * i / segments;
            float angle2 = 2.0f * M_PI * (i + 1) / segments;

            float x1 = radius * cos(angle1);
            float y1 = radius * sin(angle1);

            float x2 = radius * cos(angle2);
            float y2 = radius * sin(angle2);

            glVertex3f(centerX + x1, centerY + y1, zNear);
            glVertex3f(centerX + x2, centerY + y2, zNear);
            glVertex3f(centerX + x2, centerY + y2, zFar);
            glVertex3f(centerX + x1, centerY + y1, zFar);
        }
    glEnd();
    checkError("drawQuad");
}

void drawScene(void)
{
    // Pulisce buffer colore e profondità
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Proiezione ortogonale
    // glOrtho(-1, 1, -1, 1, 1, 5);

    // Proiezione prospettica con frustum
    // glFrustum(-0.5, 0.5, -0.5, 0.5, 1, 5);

    // Proiezione prospettica con campo visivo (FOV)
    gluPerspective(60, 1, 1, 5);

    // Posizione e distanza dell'osservatore dalla scena (zoom)
    glTranslatef(0.0, 0.0, depth);
    
    // Calcolo della scala in base alla profondità
    float scaleFactor = 1.0f + (depth + 2.0f) * 0.3f;
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Rotazione del prisma lungo gli assi X e Y
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Modifica della profondità del prisma
    float depthFactor = 1.0f + (depth + 3.0f) * 0.5f;
    float zNear = -0.5f * depthFactor; // Base più vicina all'osservatore
    float zFar = 0.5f * depthFactor; // Base più lontana

    // Disegno della base inferiore
    drawCircle(0.2, 0.2, zNear, 0.5f, 8, 1.0, 0.0, 0.0);

    // Disegno facce laterali del prisma
    drawQuad(0.2f, 0.2f, zNear, zFar, 0.5f, 8, 0.0f, 1.0f, 0.0f);

    // Disegno della base superiore
    drawCircle(0.2, 0.2, zFar, 0.5f, 8, 0.0, 0.0, 1.0);

    glFlush();
    checkError("drawScene");
}

// Zoom con tastiera
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '+':
           depth += 0.2f;
           break;
        case '-':
            depth -= 0.2f;
            break;
        case 27:
            exit(0);
            break;
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseDown = 1;
            lastX = x;
            lastY = y;
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
        angleY += (x - lastX); // Rotazione Y
        angleX += (y - lastY); // Rotazione X

        lastX = x;
        lastY = y;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH); // Inizializzazione per l'uso del Depth Buffer
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Prism");

    printf("==== Prism Controls ====\n");
    printf("+ / -       -> Zoom in / out.\n");
    printf("Mouse drag  -> Rotate the prism along X and Y axes.\n");
    printf("ESC         -> Exit the program.\n\n");

    // Abilita il test di profondità
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}