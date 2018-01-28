#ifndef ONIONPAGE_H
#define ONIONPAGE_H

#include "OnionBase.h"

class OnionPage
{
    public:
        OnionPage(GLfloat heightIn, int resolutionIn);
        ~OnionPage();

        GLfloat* getPositions(glm::mat4 trans = glm::mat4(1.0f), GLfloat progress = 1.0f);
        GLfloat* getUV() { return uvCoords; };

        void printPoints();
    private:
        //A
        void addCorner(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
        void addVertex(GLfloat x, GLfloat y);
        //C
        int cornerIndex;
        //G
        void generateUV();
        //H
        GLfloat height;
        //P
        std::vector<glm::vec2> points;
        //R
        int resolution;
        GLfloat* returnValue;
        //U
        GLfloat* uvCoords;
};

#endif // ONIONPAGE_H
