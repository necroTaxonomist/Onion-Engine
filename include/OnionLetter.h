#ifndef ONIONLETTER_H
#define ONIONLETTER_H

#include "OnionBase.h"

class OnionLetter
{
    public:
        static int numInstances;

        OnionLetter();
        ~OnionLetter();

        void addCurve(std::vector<glm::vec2>& curve, GLfloat resolution, GLfloat thickness, bool connected);
        void addCurve(GLfloat x0, GLfloat y0,
                      GLfloat x1, GLfloat y1,
                      GLfloat x2, GLfloat y2,
                      GLfloat x3, GLfloat y3,
                      GLfloat resolution,
                      GLfloat thickness,
                      bool connected);

        void addCurve(std::vector<glm::vec2>& leftCurve, std::vector<glm::vec2>& rightCurve, GLfloat resolution);
        void addCurve(GLfloat xL0, GLfloat yL0,
                      GLfloat xL1, GLfloat yL1,
                      GLfloat xL2, GLfloat yL2,
                      GLfloat xL3, GLfloat yL3,
                      GLfloat xR0, GLfloat yR0,
                      GLfloat xR1, GLfloat yR1,
                      GLfloat xR2, GLfloat yR2,
                      GLfloat xR3, GLfloat yR3,
                      GLfloat resolution);
        void addCurve();
        int newCurve();

        GLfloat* getSegment(unsigned int segmentIndex, int stopIndex = -1);
        GLfloat* getSegment(unsigned int segmentIndex, int stopIndex, glm::mat4 trans);

        unsigned int segmentSize(unsigned int segmentIndex);

        #pragma warning( suppress : 4267 )
        unsigned int numSegments() { return segments.size(); };

        unsigned int numVertices();

        void printVertices();

        GLfloat getWidth() { return width; };

    private:
        std::vector<GLfloat*> segments;
        std::vector<unsigned int> sizes;
        GLfloat* returnValue;
        GLfloat width;

        GLfloat angleDifference(GLfloat low, GLfloat high);
        GLfloat angleMidpoint(GLfloat low, GLfloat high);
        GLfloat clampAngle(GLfloat angle);
};

#endif // ONIONLETTER_H
