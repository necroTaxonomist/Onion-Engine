#ifndef ONIONBASE_H_INCLUDED
#define ONIONBASE_H_INCLUDED

#include <stdio.h>
#include <string>
#include <vector>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Onion
{
    extern bool glfwInitted;
    extern bool glewInitted;

    bool onionInitGlfw(int antiAliasing=4, int glVersion=33);

    bool onionInitGlew();

    GLfloat onionGoodFOV(float monitorSize, float monitorDistance);

    GLuint onionLoadShaders(std::string vertexFilename, std::string fragmentFilename);

    void transform(GLfloat*& points, glm::mat4 trans, int numVertices);
    void translate(GLfloat*& points, GLfloat dx, GLfloat dy, GLfloat dz, int numVertices);
    void rotate(GLfloat*& points, GLfloat angle, GLfloat axisX, GLfloat axisY, GLfloat axisZ, int numVertices);

    GLfloat cubicBezier(GLfloat x0, GLfloat x1, GLfloat x2, GLfloat x3, GLfloat t);

    long long totalSize();
};

#endif
