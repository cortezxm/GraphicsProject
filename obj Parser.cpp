#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "Vertice.h"
#include "Objeto.h"
#include "Cara.h"
#include "Vector.h"


using namespace std;

void readData(ifstream* file, vector<Objeto>& objetos, vector<Vertice>& vertices);
void print(vector<Objeto> objetos, vector<Vertice> vertices);
void init(void);
void display(void);
void keyboardCallback(unsigned char key, int x, int y);

int indexObj = -1;

int numCameras = 3;

struct Color {
    float r;
    float g;
    float b;
};

vector<Objeto> objetos;
vector<Vertice> vertices;
vector<Color> colores;
vector<Vector> luces;

float posicionX2 = 0.0f;
float angulo = 90.0f;
float direccion = 0.7;

float P0[3] = { 4.0f, 0.0f, 0.0f };
float P1[3] = { 4.0f, 4.0f * sqrt(2) / 2, 0.0f };
float P2[3] = { -4.0f, 4.0f * sqrt(2) / 2, 0.0f };
float P3[3] = { -4.0f, 0.0f, 0.0f };

int currentCamera = 0;
float cameraPositions[][3] = {
    {35.0, 35.0, 20.0}, {-35.0, 25.0, 30.0}, {10.0, 15.0, 20.0} }
;
float lookAtPositions[][3] = {
    {0.0, 20.0, 0.0}, {0.0, 20.0, 0.0}, {0.0, 20.0, 0.0}
};

int main(int argc, char** argv)
{
    ifstream file1, file2, file3, file4, file5, file6, file7, file8, file9;

    file1.open("C:/Users/manue/Desktop/toilet.obj");
    file2.open("C:/Users/manue/Desktop/fish.obj");
    file3.open("C:/Users/manue/Desktop/cabinet1.obj");
    file4.open("C:/Users/manue/Desktop/carpet.obj");
    file5.open("C:/Users/manue/Desktop/sink1.obj");
    file6.open("C:/Users/manue/Desktop/sink2.obj");
    file7.open("C:/Users/manue/Desktop/wall1.obj");
    file8.open("C:/Users/manue/Desktop/wall2.obj");
    file9.open("C:/Users/manue/Desktop/floor.obj");


    if (!file1 || !file2) {
        cerr << "Error al abrir el archivo";
        return 1;
    }

    readData(&file1, objetos, vertices);
    readData(&file2, objetos, vertices);
    readData(&file3, objetos, vertices);
    readData(&file4, objetos, vertices);
    readData(&file5, objetos, vertices);
    readData(&file6, objetos, vertices);
    readData(&file7, objetos, vertices);
    readData(&file8, objetos, vertices);
    readData(&file9, objetos, vertices);

    //print(objetos, vertices);

    file1.close();
    file2.close();;
    file4.close();
    file5.close();
    file6.close();
    file7.close();
    file8.close();
    file9.close();


    for (int i = 0; i < objetos[0].caras.size(); i++) {
        Color color;
        color.r = static_cast<float>(rand()) / RAND_MAX;
        color.g = static_cast<float>(rand()) / RAND_MAX;
        color.b = static_cast<float>(rand()) / RAND_MAX;
        colores.push_back(color);
    }

    luces.push_back(Vector::direction(Vertice(0.0, -60.0, 0.0), Vertice(0.0, 0.0, 0.0)));
    luces.push_back(Vector::direction(Vertice(50.0, -60.0, 0.0), Vertice(0.0, 0.0, 0.0)));
    luces.push_back(Vector::direction(Vertice(0.0, -60.0, 50.0), Vertice(0.0, 0.0, 0.0)));
    luces.push_back(Vector::direction(Vertice(-50.0, -60.0, 0.0), Vertice(0.0, 0.0, 0.0)));
    luces.push_back(Vector::direction(Vertice(0.0, -60.0, -50.0), Vertice(0.0, 0.0, 0.0)));
    luces.push_back(Vector::direction(Vertice(-40.0, -60.0, 0.0), Vertice(-40.0, 0.0, 0.0)));
    luces.push_back(Vector::direction(Vertice(0.0, -60.0, 0.0), Vertice(-40.0, 0.0, 0.0)));


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Modelo");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCallback);
    //glutPostRedisplay();
    glutIdleFunc(display);
    glutMainLoop();

    return 0;   /* ISO C requires main to return int. */
}


void calculatePosBezier() {
    posicionX2 += direccion;
    float t = posicionX2 / 10;

    if (t > 1) {
        t = 1;
        direccion = -0.7;
    }
    else if (t < 0) {
        t = 0;
        direccion = 0.7;
    }

    float B[3];
    for (int i = 0; i < 3; i++) {
        B[i] = pow(1 - t, 3) * P0[i]
            + 3 * pow(1 - t, 2) * t * P1[i]
            + 3 * (1 - t) * pow(t, 2) * P2[i]
            + pow(t, 3) * P3[i];
    }

    glTranslatef(B[0], B[1], B[2]);
}

void rotateObject() {
    float ejeX = 0.0f;
    float ejeY = 1.0f;
    float ejeZ = 0.0f;

    angulo += 4.0f;

    if (angulo > 360.0f) {
        angulo -= 360.0f;
    }

    float radianes = angulo * 3.1416 / 180.0f;
    float c = cos(radianes);
    float s = sin(radianes);


    float matrizRotacion[16] = {
        ejeX * ejeX * (1 - c) + c, ejeX * ejeY * (1 - c) - ejeZ * s, ejeX * ejeZ * (1 - c) + ejeY * s, 0,
        ejeY * ejeX * (1 - c) + ejeZ * s, ejeY * ejeY * (1 - c) + c, ejeY * ejeZ * (1 - c) - ejeX * s, 0,
        ejeZ * ejeX * (1 - c) - ejeY * s, ejeZ * ejeY * (1 - c) + ejeX * s, ejeZ * ejeZ * (1 - c) + c, 0,
        0, 0, 0, 1
    };

    glMultMatrixf(matrizRotacion);
}

Vector calculateFaceNormal(int id, int i) {
    Vector v1 = Vector::subtract(objetos[id].caras[i].verticesFace[1], objetos[id].caras[i].verticesFace[0]);
    Vector v2 = Vector::subtract(objetos[id].caras[i].verticesFace[2], objetos[id].caras[i].verticesFace[0]);

    Vector normal = Vector::crossProduct(v1, v2);
    normal.normalize();

    return normal;
}

void calculateFlatShadingIntensity(int id, int i, float& r, float& g, float& b) {
    Vector normal = calculateFaceNormal(id, i);

    float intensity = 0;

    for (int j = 0; j < luces.size(); j++) {
        luces[j].normalize();
        float lightIntensity = normal.x * luces[j].x + normal.y * luces[j].y + normal.z * luces[j].z;

        lightIntensity = std::max(lightIntensity, 0.0f);

        intensity += lightIntensity;
    }

    intensity /= luces.size();

    if (id == 0)
    {
        //toilet
        r = 1.0f * intensity;
        g = 1.0f * intensity;
        b = 1.0f * intensity;
    }
    else if (id == 1)
    {
        //fish
        r = 1.0f * intensity;
        g = 0.5f * intensity;
        b = 0.0f * intensity;
    }
    else if (id == 2)
    {
        //cabinet
        r = 0.9f * intensity;
        g = 0.5f * intensity;
        b = 0.4f * intensity;
    }
    else if (id == 3)
    {
        //carpet
        r = 0.5f * intensity;
        g = 0.5f * intensity;
        b = 1.0f * intensity;
    }
    else if (id == 4)
    {
        //sink1
        r = 1.0f * intensity;
        g = 1.0f * intensity;
        b = 1.0f * intensity;
    }
    else if (id == 5)
    {
        //sink2
        r = 1.0f * intensity;
        g = 1.0f * intensity;
        b = 1.0f * intensity;
    }
    else if (id == 6)
    {
        //wall1
        r = 0.5f * intensity;
        g = 0.5f * intensity;
        b = 1.0f * intensity;
    }
    else if (id == 7)
    {
        //wall2
        r = 0.5f * intensity;
        g = 0.5f * intensity;
        b = 1.0f * intensity;
    }
    else if (id == 8)
    {
        //floor
        r = 0.7f * intensity;
        g = 0.7f * intensity;
        b = 1.0f * intensity;
    }
}

void drawTriangles(int id) {
    for (int i = 0; i < objetos[id].caras.size(); i++) {

        float r, g, b;
        calculateFlatShadingIntensity(id, i, r, g, b);
        glColor3f(r, g, b);
        glBegin(GL_TRIANGLES);
        glVertex3f(objetos[id].caras[i].verticesFace[0].x, objetos[id].caras[i].verticesFace[0].y, objetos[id].caras[i].verticesFace[0].z);
        glVertex3f(objetos[id].caras[i].verticesFace[1].x, objetos[id].caras[i].verticesFace[1].y, objetos[id].caras[i].verticesFace[1].z);
        glVertex3f(objetos[id].caras[i].verticesFace[2].x, objetos[id].caras[i].verticesFace[2].y, objetos[id].caras[i].verticesFace[2].z);
        glEnd();
    }
}

void keyboardCallback(unsigned char key, int x, int y) {
    if (key == 'c') {
        // Cambiar a la siguiente cámara
        currentCamera = (currentCamera + 1) % numCameras; // Asegúrate de definir numCameras correctamente
        glutPostRedisplay(); // Solicita un redibujo de la escena
    }
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // Aplica la transformación de la cámara
    glLoadIdentity();
    gluLookAt(
        cameraPositions[currentCamera][0], cameraPositions[currentCamera][1], cameraPositions[currentCamera][2],
        lookAtPositions[currentCamera][0], lookAtPositions[currentCamera][1], lookAtPositions[currentCamera][2],
        0.0, 1.0, 0.0);

    // Dibuja los demás objetos
    drawTriangles(0);
    drawTriangles(2);
    drawTriangles(3);
    drawTriangles(4);
    drawTriangles(5);
    drawTriangles(6);
    drawTriangles(7);
    drawTriangles(8);

    // Calcula la posición del objeto que rota y se mueve en curva (drawTriangles(1))
    glPushMatrix();
    calculatePosBezier();
    rotateObject();
    drawTriangles(1);
    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}


void init(void)
{
    /*  select clearing (background) color       */
    glClearColor(1.0, 1.0, 1.0, 1.0);

    /*  initialize viewing values  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    //gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        35.0, 35.0, 20.0,
        0.0, 20.0, 0.0,
        0.0, 1.0, 0.0);

    glEnable(GL_DEPTH_TEST); 
    
}


void readData(ifstream* file, vector<Objeto>& objetos, vector<Vertice>& vertices) {

    char current;
    file->get(current);

    while (current != ' ' && !file->eof())
    {
        if (current == '#' || current == 's') {
            string line;
            getline(*file, line);
        }

        if (current == 'o') {
            indexObj++;
            file->get(current);
            objetos.push_back(Objeto());
            getline(*file, objetos[indexObj].name);
        }

        if (current == 'v')
        {
            string sx, sy, sz;
            file->get(current);
            file->get(current);

            while (current != ' ')
            {
                sx += current;
                file->get(current);
            }

            file->get(current);

            while (current != ' ')
            {
                sy += current;
                file->get(current);
            }

            file->get(current);

            while (current != '\n')
            {
                sz += current;
                file->get(current);
            }

            float x = stof(sx);
            float y = stof(sy);
            float z = stof(sz);

            Vertice v = Vertice(vertices.size() + 1, x, y, z);

            objetos[indexObj].vertices.push_back(v);
            vertices.push_back(Vertice(v));
        }

        if (current == 'f') {
            string s;
            int index;

            objetos[indexObj].caras.push_back(Cara());
            file->get(current);
            file->get(current);

            while (current != '\n' && !file->eof())
            {
                while (current != ' ' && current != '\n') {
                    s += current;
                    file->get(current);
                    if (file->eof()) { break; }
                }
                if (s.empty()) {
                    break;
                }
                index = (int)(stof(s));
                (objetos[indexObj].caras[objetos[indexObj].caras.size() - 1].verticesFace).push_back(objetos[indexObj].vertices[index - 1]);
                s = ' ';
                if (current == '\n') { break; }
                file->get(current);
            }
        }
        file->get(current);
    }
}

void print(vector<Objeto> objetos, vector<Vertice> vertices) {
    for (int k = 0; k < objetos.size(); k++)
    {
        cout << objetos[k].name << endl;
        for (int i = 0; i < vertices.size(); i++)
        {
            cout << "Vertice " << i + 1 << endl;
            cout << "x " << vertices[i].x << endl;
            cout << "y " << vertices[i].y << endl;
            cout << "z " << vertices[i].z << endl;
        }
        for (int i = 0; i < objetos[k].caras.size(); i++)
        {
            cout << "Cara " << i + 1 << endl;
            for (int j = 0; j < objetos[k].caras[i].verticesFace.size(); j++)
                cout << objetos[k].caras[i].verticesFace[j].id << " ";
            cout << "\n";
        }
        cout << "\n";
    }
}