#include<gl/glut.h>
//#include<gl/glu.h>
#include<gl/gl.h>
#include<cmath>
#include<iostream>
#include<vector>
#include <fstream>
#include <sstream>
#include <string>
// #include<SOIL2.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/*float P0[2], P3[2];
float P1[2] = { 400,200 };
float P2[2] = { 500,300 };*/
//std::vector<float[]> dataPoints;
std::vector<std::vector<float>> dataPoints;
std::vector<std::vector<float>> storedPoints;
int w = 2;
GLuint testTexture;
std::vector<std::vector<float>> normalPoints;


GLuint loadTexture(const char* path) {
    int w, h, channels;

    unsigned char* data = stbi_load(path, &w, &h, &channels, 0);
    if (!data) {
        std::cerr << "stb_image error: " << stbi_failure_reason() << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        w,
        h,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}




void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 900, 0, 900);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

    testTexture = loadTexture("D:/vizexperts/Computer-Graphics-main/OpenGL Programming/snake.png");
}
/*void ArbitraryPoints() {
    P0[0] = P1[0] - (P2[0] - P1[0]);
    P0[1] = P1[1] - (P2[1] - P1[1]);
    P3[0] = P2[0] - (P2[0] - P1[0]);
    P3[1] = P2[1] + (P2[1] - P1[1]);


    //P3[0] = 200;
    //P3[1] = 800;


    std::cerr << "P0 value is " << P0[0] << "," << P0[1] << std::endl;
    std::cerr << "P3 value is " << P3[0] << "," << P3[1] << std::endl;





}*/




void splinePoint(float t, const std::vector<float>& P0, const std::vector<float>& P1,
    const std::vector<float>& P2, const std::vector<float>& P3, float C[2]) {
    float t0 = 0.0;
    float t1 = t0 + sqrt(pow((P1[0] - P0[0]), 2) + pow((P1[1] - P0[1]), 2));
    float t2 = t1 + sqrt(pow((P2[0] - P1[0]), 2) + pow((P2[1] - P1[1]), 2));
    float t3 = t2 + sqrt(pow((P3[0] - P2[0]), 2) + pow((P3[1] - P2[1]), 2));

    t = (t2 - t1) * t + t1;
    std::cerr << "value of t is" << t << std::endl;
    std::cerr << "value of t0 is" << t0 << std::endl;
    std::cerr << "value of t1 is" << t1 << std::endl;
    std::cerr << "value of t2 is" << t2 << std::endl;
    std::cerr << "value of t3 is" << t3 << std::endl;



    float A1[2] = {
        (t1 - t) / (t1 - t0) * P0[0] + (t - t0) / (t1 - t0) * P1[0],
        (((t1 - t) / (t1 - t0)) * P0[1]) + (((t - t0) / (t1 - t0)) * P1[1])

    };
    float A2[2] = {
        (t2 - t) / (t2 - t1) * P1[0] + (t - t1) / (t2 - t1) * P2[0],
        (((t2 - t) / (t2 - t1)) * P1[1]) + (((t - t1) / (t2 - t1)) * P2[1]),

    };
    float A3[2] = {
        (((t3 - t) / (t3 - t2)) * P2[0]) + (((t - t2) / (t3 - t2)) * P3[0]),
        (((t3 - t) / (t3 - t2)) * P2[1]) + (((t - t2) / (t3 - t2)) * P3[1])

    };

    float B1[2] = {
        (((t2 - t) / (t2 - t0)) * A1[0]) + (((t - t0) / (t2 - t0)) * A2[0]),
        (((t2 - t) / (t2 - t0)) * A1[1]) + (((t - t0) / (t2 - t0)) * A2[1])

    };

    float B2[2] = {
        (((t3 - t) / (t3 - t1)) * A2[0]) + (((t - t1) / (t3 - t1)) * A3[0]),
        (((t3 - t) / (t3 - t1)) * A2[1]) + (((t - t1) / (t3 - t1)) * A3[1])

    };

    /*C[2] = {
        (((t2 - t) / (t2 - t1)) * B1[0]) + (((t - t1) / (t2 - t1)) * B2[0]),
        (((t2 - t) / (t2 - t1)) * B1[1]) + (((t - t1) / (t2 - t1)) * B2[1])

    };*/
    C[0] = (((t2 - t) / (t2 - t1)) * B1[0]) + (((t - t1) / (t2 - t1)) * B2[0]);
    C[1] = (((t2 - t) / (t2 - t1)) * B1[1]) + (((t - t1) / (t2 - t1)) * B2[1]);

    //storedPoints.push_back({ C[0],C[1] });



    std::cerr << "A1 is" << "(" << A1[0] << "," << A1[1] << ")" << std::endl;

    std::cerr << "A2 is" << "(" << A2[0] << "," << A2[1] << ")" << std::endl;
    std::cerr << "A3 is" << "(" << A3[0] << "," << A3[1] << ")" << std::endl;
    std::cerr << "C is" << "(" << C[0] << "," << C[1] << ")" << std::endl;


    /*std::cerr << "t1 is" << t1 << std::endl;
    std::cerr << "t2 is" << t2 << std::endl;
    std::cerr << "t3 is" << t3 << std::endl;*/

    /*glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    glVertex2f(P0[0], P0[1]);
    glColor3f(0.0f, 1.0f, 0.0f);

    glVertex2f(P1[0], P1[1]);
    glVertex2f(P2[0], P2[1]);
    glColor3f(1.0f, 0.0f, 0.0f);

    glVertex2f(P3[0], P3[1]);
    glColor3f(0.0f, 0.0f, 0.0f);

    glVertex2f(A1[0], A1[1]);
    glVertex2f(A2[0], A2[1]);
    glVertex2f(A3[0], A3[1]);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(B1[0], B1[1]);
    glVertex2f(B2[0], B2[1]);
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex2f(C[0], C[1]);

    glEnd();*/


    //glutPostRedisplay();




}

void drawNormal(
    float t,
    const std::vector<float>& P0,
    const std::vector<float>& P1,
    const std::vector<float>& P2,
    const std::vector<float>& P3,
    float normalLen = 2.0f
) {
    float eps = 0.001f;

    float C1[2], C2[2];
    splinePoint(t - eps, P0, P1, P2, P3, C1);
    splinePoint(t + eps, P0, P1, P2, P3, C2);

    // Tangent vector
    float tx = C2[0] - C1[0];
    float ty = C2[1] - C1[1];

    float mag = sqrt(tx * tx + ty * ty);
    if (mag == 0) return;

    tx /= mag;
    ty /= mag;

    // Normal vector (perpendicular)
    float nx = -ty;
    float ny =  tx;

    float C[2];
    splinePoint(t, P0, P1, P2, P3, C);

    // Draw normal line (up & down)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);

    //what these next two lines does?
    float startX = C[0] + nx * normalLen;
    float startY = C[1] + ny * normalLen;
    float endX = C[0] - nx * normalLen;
    float endY = C[1] - ny * normalLen;

    normalPoints.push_back({startX, startY});
    normalPoints.push_back({endX, endY});



    glVertex2f(C[0] + nx * normalLen, C[1] + ny * normalLen);
    glVertex2f(C[0] - nx * normalLen, C[1] - ny * normalLen);

    glEnd();
   
}

void drawSpline(const std::vector<float>& P0, const std::vector<float>& P1,
    const std::vector<float>& P2, const std::vector<float>& P3, int segments) {
    glEnable(GL_POINT_SMOOTH);

    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(10.0f);
    for (int i = 0;i <= segments;i++) {
        float t = static_cast<float>(i) / static_cast<float>(segments);
        float C[2];
        splinePoint(t, P0, P1, P2, P3, C);
        glVertex2f(static_cast<int>(C[0]), static_cast<int>(C[1]));
    }
    for (int i = 0; i <= 10; i++) {
        float t = i / 10.0f;
        drawNormal(t, P0, P1, P2, P3, 10.0f);
        //if()
    }
    


    glEnd();
}

void ReadPoints() {
    //std::ifstream inputFile("points.txt"); 
    std::ifstream inputFile("d:\\vizexperts\\Computer-Graphics-main\\OpenGL Programming\\points.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file points.txt" << std::endl;

    }

    //std::vector<std::vector<int>> dataPoints;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::vector<float> pair;
        int x, y;

        if (ss >> x && ss.ignore(1, ',') && ss >> y) {
            pair.push_back(x * 100);
            pair.push_back(y * 100);
            dataPoints.push_back(pair);
        }
        else {
            std::cerr << "Invalid format in line: " << line << std::endl;
        }
    }

    // Accessing the coordinates:
    for (int i = 0; i < dataPoints.size(); ++i) {
        std::cerr << "Coordinate " << i << ": (" << dataPoints[i][0] << ", " << dataPoints[i][1] << ")\n";
    }
}

void ApplyPoints() {
    dataPoints.push_back({ 0,0 });        //P0
    /*dataPoints.push_back({ 100,500 });    //P1
    dataPoints.push_back({ 300,650 });    //P2
    dataPoints.push_back({ 600,300 });    //P3
    dataPoints.push_back({ 700,700 });    //P4
    //input code above here
    */
    ReadPoints();
    dataPoints.push_back({ 900,900 });    //Pn

    for (int i = 0;i < dataPoints.size() - 3;i++) {
        drawSpline(dataPoints[i], dataPoints[i + 1], dataPoints[i + 2], dataPoints[i + 3], 1000);
        //glutPostRedisplay();

    }
    dataPoints.clear();
    //int i = 0;
    //drawSpline(dataPoints[0], dataPoints[1], dataPoints[4], dataPoints[5], 100);



}





// void display() {
//     glClear(GL_COLOR_BUFFER_BIT);
//     //splinePoint(0.5, P0, P1, P2, P3);
//     int segments = 100;
//     //drawSpline(P0, P1, P2, P3, segments);
//     ApplyPoints();
//     glutSwapBuffers();

// }

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    int segments = 100;
    ApplyPoints();


    glBindTexture(GL_TEXTURE_2D, testTexture);
    glColor3f(1,1,1); // IMPORTANT: don’t tint texture


    // glTexCoord2f(0, 0); glVertex2f(50, 50);
    // glTexCoord2f(1, 0); glVertex2f(250, 50);
    // glTexCoord2f(1, 1); glVertex2f(250, 250);
    // glTexCoord2f(0, 1); glVertex2f(50, 250);
    std::vector< float> bottomLeft, bottomRight, topLeft, topRight;
    

    // for(int i = 0; i < normalPoints.size(); i++) {
    //     // glTexCoord2f(normalPoints[i][0], normalPoints[i][1]);
    //     // glVertex2f(normalPoints[i][0], normalPoints[i][1]);
    //     if(i%4==0){
    //         topLeft = normalPoints[i];
    //     }
    //     else if(i%4==1){
    //         bottomLeft = normalPoints[i];

    //     }
    //     else if(i%4==2){
    //         topRight = normalPoints[i];

    //     }
    //     else{
    //         bottomRight = normalPoints[i];
    //         glBegin(GL_QUADS);
    //         //console.log("runninhg=sas");
    //         //print log mess
    //         std::cout << "runging=sas" << std::endl;

    //         glTexCoord2f(0, 0); glVertex2f(bottomLeft[0],bottomLeft[1]);
    //         glTexCoord2f(1, 0); glVertex2f(bottomRight[0],bottomRight[1]);
    //         glTexCoord2f(1, 1); glVertex2f(topRight[0],topRight[1]);
    //         glTexCoord2f(0, 1); glVertex2f(topLeft[0],topLeft[1]);
    //         glEnd();

    //     }
    // }

    for (int i = 0; i + 3 < normalPoints.size(); i += 2) {

        // current segment
        auto& bottomLeft  = normalPoints[i];
        auto& bottomRight = normalPoints[i + 1];
    
        // next segment
        auto& topLeft  = normalPoints[i + 2];
        auto& topRight = normalPoints[i + 3];
    
        glBegin(GL_QUADS);
    
        // glTexCoord2f(0, 0); glVertex2f(bottomLeft[0],  bottomLeft[1]);
        // glTexCoord2f(1, 0); glVertex2f(bottomRight[0], bottomRight[1]);
        // glTexCoord2f(1, 1); glVertex2f(topRight[0],    topRight[1]);
        // glTexCoord2f(0, 1); glVertex2f(topLeft[0],     topLeft[1]);

        glTexCoord2f(1, 0); glVertex2f(bottomLeft[0],  bottomLeft[1]);
        glTexCoord2f(1, 1); glVertex2f(bottomRight[0], bottomRight[1]);
        glTexCoord2f(0, 1); glVertex2f(topRight[0],    topRight[1]);
        glTexCoord2f(0, 0); glVertex2f(topLeft[0],     topLeft[1]);

    
        glEnd();
    }
    




    glutSwapBuffers();
}







void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'E':
    case 'e':
        w = 2;

        break;

    case 'P':
    case 'p':
        w = 3;
        break;
    case 'L':
    case 'l':
        w = 4;
        break;
    case 'K':
    case 'k':
        w = 15;
        break;
    case 'J':
    case 'j':
        w = 20;
        break;
    case 'H':
    case 'h':
        w = 25;
        break;


    }
    glutPostRedisplay();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);

    //ApplyPoints();
    //ArbitraryPoints();
    //ReadPoints();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Catmull-ROM-Spline");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();


    return 0;

}


