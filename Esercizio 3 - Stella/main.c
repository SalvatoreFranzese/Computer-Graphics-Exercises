#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

float angle = 0.0f;
float scale = 1.0f;
float xTranslate = 0.0f;
float yTranslate = 0.0f;

int lastX = 0;
int lastY = 0;

void checkError(const char* label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s - OpenGL error %d: %s\n", label, error, gluErrorString(error));
    }
}

void drawStar(int numberOfStarPoints, float outerRadius, float innerRadius)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f); // Centro della stella (tutti i triangoli partono da qui)

        // numberOfStarPoints * 2 : alternimao punta esterna e punta interna
        for (int i = 0; i <= numberOfStarPoints * 2; i++)
        {
            float angleStep = 2.0f * M_PI * i / (numberOfStarPoints * 2); // Divide il cerchio in (numberOfStarPoints * 2) sezioni
            float radius = (i % 2 == 0) ? outerRadius : innerRadius; // Alterniamo raggio esterno e raggio interno
            float x = radius * cos(angleStep);
            float y = radius * sin(angleStep);
            glVertex2f(x, y);
        }
    glEnd();
    checkError("drawStar");
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'w':
            yTranslate += 0.1f;
            glutPostRedisplay();
            break;
        case 'a':
            xTranslate -= 0.1f;
            glutPostRedisplay();
            break;
        case 's':
            yTranslate -= 0.1f;
            glutPostRedisplay();
            break;
        case 'd':
            xTranslate += 0.1f;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            angle -= 5.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            angle += 5.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            scale += 0.01f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            scale -= 0.01f;
            glutPostRedisplay();
            break;
    }    
}

void mouse(int button, int state, int x, int y)
{
    // Alla pressione di un tasto del mouse, mi salvo la posizione corrente
    if (state == GLUT_DOWN)
    {
        lastX = x;
        lastY = y;
    }
}

void motion(int x, int y)
{
    int dx = x - lastX; // Differenza orizzontale tra la posizione attuale e l'ultima
    int dy = y - lastY; // Differenza verticale tra la posizione attuale e l'ultima

    // Applichiamo un cambiamento solo se il mouse si è mosso di almeno 10 pixel
    if (abs(dx) > 10)
    {
        angle += dx * 0.5f; // Ruota la stella in base allo spostamento orizzontale
        lastX = x;
    }

    if (abs(dy) > 10)
    {
        scale += dy * 0.01f; // Scala la stella in base allo spostamento verticale
        lastY = y;
    }

    glutPostRedisplay();
}

void passiveMotion(int x, int y)
{
    // Prendiamo larghezza e altezza della finestra corrente
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // Calcoliamo il centro della finestra
    float halfWidth = windowWidth / 2.0f;
    float halfHeight = windowHeight / 2.0f;

    // Converte la posizione del mouse (pixel) in coordinate OpenGL nel range [-1, +1]
    xTranslate = (x - halfWidth) / halfWidth;
    yTranslate = (halfHeight - y) / halfHeight;

    glutPostRedisplay();
}

void drawScene(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity(); // Annulla tutte le trasformazioni fatte prima e torna al centro dello schermo (resetta la matrice corrente alla matrice identità)

    glTranslatef(xTranslate, yTranslate, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, 1.0f);

    drawStar(5, 0.5f, 0.1f);
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Star");

    printf("==== Star Controls ====\n");
    printf("WASD        -> Translate.\n");
    printf("← →         -> Rotate.\n");
    printf("↑ ↓         -> Scale.\n");
    printf("Mouse move  -> Translate (follow mouse).\n");
    printf("Mouse drag  -> Rotate + Scale.\n");
    printf("ESC         -> Exit the program.\n\n");

    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);

    glutMainLoop();
    return 0;
}