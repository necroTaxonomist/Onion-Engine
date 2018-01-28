#include "OnionSurface.h"

int OnionSurface::numInstances = 0;

OnionSurface::OnionSurface(GLfloat posIn[12], GLuint textureIn):
    texture(textureIn)
{
    for (int i = 0; i < 12; ++i)
    {
        positions[i] = posIn[i];
    }

    setRepeat(1.0f, 1.0f);
    ++numInstances;
}
OnionSurface::OnionSurface(GLfloat blX, GLfloat blY, GLfloat blZ,
                     GLfloat tlX, GLfloat tlY, GLfloat tlZ,
                     GLfloat brX, GLfloat brY, GLfloat brZ,
                     GLfloat trX, GLfloat trY, GLfloat trZ,
                     GLuint textureIn):
                         texture(textureIn)
{
    positions[0] = blX; positions[1] = blY; positions[2] = blZ;
    positions[3] = tlX; positions[4] = tlY; positions[5] = tlZ;
    positions[6] = brX; positions[7] = brY; positions[8] = brZ;
    positions[9] = trX; positions[10] = trY; positions[11] = trZ;

    setRepeat(1.0f, 1.0f);
    ++numInstances;
}

GLfloat* OnionSurface::getPositions(glm::mat4 trans)
{
    for (int i = 0; i < 12; ++i)
    {
        positionsTF[i] = positions[i];
    }

    Onion::transform(positionsTF, trans, 4);

    return positionsTF;
}
GLfloat* OnionSurface::getPositions(GLfloat dx, GLfloat dy, GLfloat dz)
{
    for (int i = 0; i < 12; ++i)
    {
        positionsTF[i] = positions[i];
    }

    Onion::translate(positionsTF, dx, dy, dz, 4);

    return positionsTF;
}

void OnionSurface::setTexture(GLuint textureIn)
{
    texture = textureIn;
}

void OnionSurface::setRepeat(GLfloat uRepeat, GLfloat vRepeat)
{
    uvCoords[0] = 0.0f; uvCoords[1] = vRepeat;
    uvCoords[2] = 0.0f; uvCoords[3] = 0.0f;
    uvCoords[4] = uRepeat; uvCoords[5] = vRepeat;
    uvCoords[6] = uRepeat; uvCoords[7] = 0.0f;
}

void OnionSurface::setUVRect(GLfloat leftX, GLfloat topY, GLfloat rightX, GLfloat bottomY)
{
    uvCoords[0] = leftX; uvCoords[1] = bottomY;
    uvCoords[2] = leftX; uvCoords[3] = topY;
    uvCoords[4] = rightX; uvCoords[5] = bottomY;
    uvCoords[6] = rightX; uvCoords[7] = topY;
}

OnionSurface::~OnionSurface()
{
    //dtor
}
