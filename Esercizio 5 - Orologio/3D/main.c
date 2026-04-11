#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// Valori del Volume di Vista
float width;
float height;
float depth = -2.0f; // Traslazione lungo Z (simula zoom avanti/indietro)

// Rotazione iniziale scena
float angleX = 0.0f;
float angleY = 0.0f;

// Stato cronometro
int running = 0; // 0 = stop, 1 = start
float seconds = 0.0f; // Tempo totale in secondi

// Angoli lancetta dei secondi e dei minuti
float secondAngle = 0.0f;
float minuteAngle = 0.0f;

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
void drawColoredCircle(float centerX, float centerY, float zNear, float radius, int segments, float r, float g, float b)
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
    checkError("drawColoredCircle");
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

void drawLine(float centerX, float centerY, float angle, float length, float z, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_LINES);
        float x = length * cos(angle + M_PI / 2.0f);
        float y = length * sin(angle + M_PI / 2.0f);

        glVertex3f(centerX, centerY, z);
        glVertex3f(centerX + x, centerY + y, z);
    glEnd();
    checkError("drawLine");
}

void drawNumber(float radius, float z)
{
    for (int i = 1; i <= 12; i++)
    {
        // Calcolo angolo per ogni numero
        float angle = M_PI / 2.0f - i * (2.0f * M_PI / 12.0f);

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos3f(x - 0.015f, y - 0.015f, z);

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
    glClearColor(0.2, 0.0, 0.0, 1.0);

    // Pulisce buffer colore e profondità
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Proiezione prospettica con campo visivo (FOV)
    gluPerspective(60, 1, 1, 10);

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
    float zNear = -0.05f * depthFactor; // Base più vicina all'osservatore
    float zFar = 0.05f * depthFactor; // Base più lontana

    // Disegno della base superiore
    drawColoredCircle(0.0f, 0.0f, zFar, 0.5f, 50, 1.0, 1.0, 1.0);

    // Disegno facce laterali del prisma
    drawQuad(0.0f, 0.0f, zNear, zFar, 0.5f, 50, 0.0f, 0.0f, 0.0f);

    // Disegno della base inferiore
    drawColoredCircle(0.0f, 0.0f, zNear, 0.5f, 50, 0.0, 0.0, 0.0);

    // Lancetta secondi
    drawLine(0.0f, 0.0f, secondAngle, 0.4f, -zNear + 0.09f, 1.0f, 0.0f, 0.0f);

    // Lancetta minuti
    drawLine(0.0f, 0.0f, minuteAngle, 0.4, -zNear + 0.09f, 0.0f, 0.0f, 0.0f);

    // Disegna i numeri da 1 a 12 sul quadrante
    drawNumber(0.45f, -zNear + 0.09f);

    glFlush();
    checkError("drawScene");
}

// Zoom con tastiera
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 's': // Start
            running = 1;
            break;
        case 'p': // Stop
            running = 0;
            break;
        case 'r': // Reset
            running = 0;
            seconds = 0;
            secondAngle = 0;
            minuteAngle = 0;
            break;
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
        angleY += (x - lastX) * 0.5f; // Rotazione Y
        angleX += (y - lastY) * 0.5f; // Rotazione X

        lastX = x;
        lastY = y;

        glutPostRedisplay();
    }
}

void update(int value)
{
    if (running)
    {
        seconds += 1.0f;

        // Aggiorna lancette
        secondAngle = -(seconds / 60.0f) * 2.0f * M_PI;
        minuteAngle = -(seconds / 3600.f) * 2.0f * M_PI;
    }

    glutPostRedisplay();
    glutTimerFunc(1000, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH); // Inizializzazione per l'uso del Depth Buffer
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Stopwatch");

    printf("==== 3D Stopwatch Controls ====\n");
    printf("s           -> Start.\n");
    printf("p           -> Stop.\n");
    printf("r           -> Reset.\n");
    printf("+ / -       -> Zoom in / out.\n");
    printf("Mouse drag  -> Rotate the stopwatch along X and Y axes.\n");
    printf("ESC         -> Exit the program.\n\n");

    // Abilita il test di profondità
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(1000, update, 0);

    glutMainLoop();
    return 0;
}