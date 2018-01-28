#include "OnionWindow.h"

void OnionWindow::viewSetup()
{
    projMatrix = glm::mat4(1.0f);
    setViewPosition(0, 0, 1);
    setViewAngle(0.0f, 0.0f, 0.0f);
}

void OnionWindow::setViewPosition(GLfloat x, GLfloat y, GLfloat z)
{
    viewPosition = glm::vec3(x,y,z);
    calcViewMatrix();
}
void OnionWindow::moveViewPosition(GLfloat dx, GLfloat dy, GLfloat dz)
{
    viewPosition += glm::vec3(dx,dy,dz);
    calcViewMatrix();
}
void OnionWindow::strafeViewPosition(GLfloat dx, GLfloat dz)
{
    GLfloat dxRel = dx * glm::cos(viewAngle.x) + dz * glm::sin(viewAngle.x);
    GLfloat dzRel = -dx * glm::sin(viewAngle.x) +  + dz * glm::cos(viewAngle.x);
    viewPosition += glm::vec3(dxRel,0,dzRel);
    calcViewMatrix();
}

void OnionWindow::setViewAngle(GLfloat y, GLfloat p, GLfloat r)
{
    viewAngle = glm::vec3(y,p,r) * 0.01745329251994f;
    calcViewMatrix();
}
void OnionWindow::moveViewAngle(GLfloat dy, GLfloat dp, GLfloat dr)
{
    viewAngle += glm::vec3(dy,dp,dr) * 0.01745329251994f;
    calcViewMatrix();
}
void OnionWindow::viewLookAt(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat dx = x - viewPosition.x;
    GLfloat dz = z - viewPosition.z;
    GLfloat dy = y - viewPosition.y;
    GLfloat horDist = sqrt(dx * dx + dz * dz);

    GLfloat newYaw = glm::atan(-dx, -dz);
    GLfloat newPitch = glm::atan(dy, horDist);
    //printf("%f,%f\n",newYaw,newPitch);

    viewAngle = glm::vec3(newYaw,newPitch,0);
    calcViewMatrix();
}


void OnionWindow::setOrthographic(GLfloat w, GLfloat h)
{
    projFOV = 0;

    projMatrix = glm::scale(glm::vec3(2 / w, 2 / h, 0));
}
void OnionWindow::setPerspective(GLfloat fov, GLfloat near, GLfloat far)
{
    if (fov >= 180)
        fov = 179.0f;
    else if (fov < 0)
        fov = 0.0f;

    projFOV = fov;

    GLfloat tangent = glm::tan(glm::radians(fov / 2));

    projMatrix = glm::mat4(1.0f);

    projMatrix[0][0] = 2 / tangent;

    projMatrix[1][1] = ((float)width / (float)height) * (2 / tangent);

    projMatrix[2][2] = -(far + near) / (far - near);

    projMatrix[2][3] = -2 * far * near / (far - near);

    projMatrix[3][2] = -1.0f;

    projMatrix[3][3] = 0.0f;
}
void OnionWindow::setPerspective(GLfloat fov)
{
    if (fov >= 180)
        fov = 179.0f;
    else if (fov < 0)
        fov = 0.0f;

    projFOV = fov;

    GLfloat tangent = glm::tan(glm::radians(fov / 2));

    projMatrix[0][0] = 2 / tangent;

    projMatrix[1][1] = 2 * ((float)width / (float)height) / tangent;
}

void OnionWindow::calcViewMatrix()
{
    viewMatrix = glm::translate(-viewPosition);
    viewMatrix = glm::rotate(
                             -viewAngle.x,
                             glm::vec3(0,1,0)
                             ) * viewMatrix;
    viewMatrix = glm::rotate(
                             -viewAngle.y,
                             glm::vec3(1,0,0)
                             ) * viewMatrix;
    viewMatrix = glm::rotate(
                             -viewAngle.z,
                             glm::vec3(0,0,-1)
                             ) * viewMatrix;
}

glm::vec2 OnionWindow::getScreenPoint(glm::vec3 point)
{
    glm::vec4 pointHomo = glm::vec4(point,1);
    pointHomo = projMatrix * viewMatrix * pointHomo;
    glm::vec2 pointCart(pointHomo.x / pointHomo.w, pointHomo.y / pointHomo.w);
    return pointCart.xy;
}

GLfloat OnionWindow::getZ(glm::vec3 point)
{
    glm::vec4 pointHomo = glm::vec4(point,1);
    pointHomo = viewMatrix * pointHomo;
    return pointHomo.z;
}
