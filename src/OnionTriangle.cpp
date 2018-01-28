#include "OnionTriangle.h"

OnionTriangle::OnionTriangle(GLfloat x1, GLfloat y1,
                      GLfloat x2, GLfloat y2,
                      GLfloat x3, GLfloat y3)
{
    myPoints.push_back(glm::vec4(x1,y1,0,1));
    myPoints.push_back(glm::vec4(x2,y2,0,1));
    myPoints.push_back(glm::vec4(x3,y3,0,1));
}

OnionTriangle::OnionTriangle(GLfloat x1, GLfloat y1, GLfloat z1,
                      GLfloat x2, GLfloat y2, GLfloat z2,
                      GLfloat x3, GLfloat y3, GLfloat z3)
{
    myPoints.push_back(glm::vec4(x1,y1,z1,1));
    myPoints.push_back(glm::vec4(x2,y2,z2,1));
    myPoints.push_back(glm::vec4(x3,y3,z3,1));
}
