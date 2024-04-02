// #include <GL/glut.h>
// #include <cmath>

// const float PI = 3.14159265358979323846;
// const float epsilon = 1.0;  // Valeur de l'énergie de potentiel minimale
// const float sigma = 1.0;    // Valeur de la distance à laquelle l'énergie potentielle est nulle

// // Calculer le potentiel de Lennard-Jones pour une distance donnée r
// float calculateLennardJonesPotential(float r) {
//     return 4 * epsilon * (pow(sigma / r, 12) - pow(sigma / r, 6));
// }

// void drawCurve() {
//     glClear(GL_COLOR_BUFFER_BIT);
//     glColor3f(1.0, 1.0, 1.0);  // Couleur blanche

//     // Dessiner l'axe X
//     glBegin(GL_LINES);
//     glVertex2f(0.0, 0.0);
//     glVertex2f(10.0, 0.0);
//     glEnd();

//     // Dessiner l'axe Y
//     glBegin(GL_LINES);
//     glVertex2f(0.0, -2.0);
//     glVertex2f(0.0, 10.0);
//     glEnd();

//     glColor3f(0.0, 1.0, 0.0);  // Couleur verte pour le potentiel de Lennard-Jones

//     glBegin(GL_LINE_STRIP);    // Dessiner une ligne
//     for(float r = 0.1; r <= 10.0; r += 0.1) {
//         glVertex2f(r, calculateLennardJonesPotential(r));  // Dessiner le point (r, V(r))
//     }
//     glEnd();

//     glFlush();
// }

// void init() {
//     glClearColor(0.0, 0.0, 0.0, 0.0);  // Couleur de fond noire
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluOrtho2D(0.0, 10.0, -2.0, 10.0);  // Définir les limites de la fenêtre
// }

// int main(int argc, char** argv) {
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//     glutInitWindowSize(500, 500);  // Taille de la fenêtre
//     glutCreateWindow("Potentiel de Lennard-Jones en fonction de la distance");
//     init();
//     glutDisplayFunc(drawCurve);  // Définir la fonction de dessin
//     glutMainLoop();
//     return 0;
// }


#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <sstream>

const float PI = 3.14159265358979323846;
const float epsilon = 1.0;  // Valeur de l'énergie de potentiel minimale
const float sigma = 1.0;    // Valeur de la distance à laquelle l'énergie potentielle est nulle
const float rCut = 3;      // Distance minimale pour laquelle le potentiel est nul

// Calculer le potentiel de Lennard-Jones pour une distance donnée r
float calculateLennardJonesPotential(float r) {
    if (r <= rCut) {
        return 4 * epsilon * (pow(sigma / r, 12) - pow(sigma / r, 6));
    } else {
        return 0.0;
    }
}

void drawCurve() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.7, 0.7, 0.7);  // Couleur gris clair pour les axes

    // Dessiner l'axe X
    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0);
    glVertex2f(10.0, 0.0);
    glEnd();

    // Dessiner l'axe Y
    glBegin(GL_LINES);
    glVertex2f(0.0, -2.0);
    glVertex2f(0.0, 10.0);
    glEnd();

    // Ajouter des graduations sur l'axe X
    glColor3f(0.5, 0.5, 0.5);  // Couleur gris foncé pour les graduations
    for (float r = 1.0; r <= 10.0; r += 1.0) {
        glBegin(GL_LINES);
        glVertex2f(r, -0.1);
        glVertex2f(r, 0.1);
        glEnd();
    }

    // Ajouter des graduations sur l'axe Y
    for (float V = -1.0; V <= 5.0; V += 1.0) {
        glBegin(GL_LINES);
        glVertex2f(-0.1, V);
        glVertex2f(0.1, V);
        glEnd();
    }

    // Dessiner le potentiel de Lennard-Jones
    glColor3f(0.0, 0.5, 0.8);  // Couleur bleu pour le potentiel de Lennard-Jones
    glBegin(GL_LINE_STRIP);    // Dessiner une ligne
    for (float r = 0.1; r <= 10.0; r += 0.1) {
        glVertex2f(r, calculateLennardJonesPotential(r));  // Dessiner le point (r, V(r))
    }
    glEnd();

    // Ajouter des étiquettes aux axes
    glColor3f(1.0, 1.0, 1.0);  // Couleur blanche pour les étiquettes
    glRasterPos2f(10.2, 0.0);
    std::string xLabel = "Distance (r)";
    for (char c : xLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }

    glRasterPos2f(0.2, 5.0);
    std::string yLabel = "Potentiel (V)";
    for (char c : yLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }

    // Ajouter une légende pour les paramètres
    std::ostringstream legend;
    legend << "Paramètres:";
    legend << " epsilon=" << epsilon << ", sigma=" << sigma;
    glColor3f(1.0, 1.0, 1.0);  // Couleur blanche pour la légende
    glRasterPos2f(0.2, 8.5);
    for (char c : legend.str()) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }

    glFlush();
}

void init() {
    glClearColor(0.1, 0.1, 0.1, 0.0);  // Couleur de fond gris foncé
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 10.0, -2.0, 10.0);  // Définir les limites de la fenêtre
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);  // Taille de la fenêtre
    glutCreateWindow("Potentiel de Lennard-Jones");
    init();
    glutDisplayFunc(drawCurve);  // Définir la fonction de dessin
    glutMainLoop();
    return 0;
}
