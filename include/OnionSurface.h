#ifndef ONIONSURFACE_H
#define ONIONSURFACE_H

#include "OnionBase.h"

class OnionSurface
{
    public:
        static int numInstances;

        OnionSurface(GLfloat posIn[18], GLuint textureIn);
        OnionSurface(GLfloat blX, GLfloat blY, GLfloat blZ,
                     GLfloat tlX, GLfloat tlY, GLfloat tlZ,
                     GLfloat brX, GLfloat brY, GLfloat brZ,
                     GLfloat trX, GLfloat trY, GLfloat trZ,
                     GLuint textureIn);
        ~OnionSurface();

        GLfloat* getPositions() { return positions; };
        GLfloat* getPositions(glm::mat4 trans);
        GLfloat* getPositions(GLfloat dx, GLfloat dy, GLfloat dz);

        GLfloat* getUV() { return uvCoords; };
        GLuint getTexture() { return texture; };
        int getNumVertices() { return 4; };

        void setTexture(GLuint textureIn);

        void setRepeat(GLfloat uRepeat, GLfloat vRepeat);

        void setUVRect(GLfloat leftX, GLfloat topY, GLfloat rightX, GLfloat bottomY);

    protected:
        GLfloat positions[12];
        GLfloat positionsTF[12];
        GLfloat uvCoords[8];
        GLuint texture;
};

#endif // ONIONSURFACE_H
