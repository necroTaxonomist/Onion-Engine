#ifndef ONIONWINDOW_H
#define ONIONWINDOW_H

#include "OnionSurface.h"
#include "OnionBuffer.h"
#include "OnionLetter.h"
#include "OnionPage.h"

class OnionWindow
{
    public:
        static int numInstances;

        OnionWindow(int w, int h, std::string name);
        ~OnionWindow();

        void update();

        GLFWwindow* getWindow() { return window; };

        //Draw Functions
        void setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a=1.0f);

        int addProgram(std::string vertexFilename, std::string fragmentFilename);

        int addBuffer(int numAttribs, int programIndex = -1, bool usesProjectionMatrix = true);
        void defineBufferAttrib(int bufferIndex,
                                GLuint index,
                                GLint size = 3,
                                GLboolean normalized = GL_FALSE,
                                GLsizei stride = 0,
                                const GLvoid * pointer = (void*)0);
        void setBufferBackground(int bufferIndex, GLuint bgTexture);

        void drawSurface(int bufferIndex, OnionSurface& surface);
        void drawSurface(int bufferIndex, OnionSurface& surface, glm::mat4 trans);
        void drawSurface(int bufferIndex, OnionSurface& surface, GLfloat dx, GLfloat dy, GLfloat dz);

        void drawSurfaceEmpty(int bufferIndex, OnionSurface& surface);
        void drawSurfaceEmpty(int bufferIndex, OnionSurface& surface, glm::mat4 trans);

        void drawLetter(int bufferIndex, OnionLetter* letter, int numVertices = -1);
        void drawLetter(int bufferIndex, OnionLetter* letter, int numVertices, glm::mat4 trans);

        void drawLetterPortion(int bufferIndex, OnionLetter* letter, GLfloat portion);
        void drawLetterPortion(int bufferIndex, OnionLetter* letter, GLfloat portion, glm::mat4 trans);

        void drawVertices(int bufferIndex, GLenum drawType, int numVertices, GLfloat arr0[]);
        void drawVertices(int bufferIndex, GLenum drawType, int numVertices, GLfloat arr0[], GLfloat arr1[]);

        //View Functions
        void setViewPosition(GLfloat x, GLfloat y, GLfloat z);
        void moveViewPosition(GLfloat dx, GLfloat dy, GLfloat dz);
        void strafeViewPosition(GLfloat dx, GLfloat dz);

        void setViewAngle(GLfloat y, GLfloat p, GLfloat r);
        void moveViewAngle(GLfloat dy, GLfloat dp, GLfloat dr);
        void viewLookAt(GLfloat x, GLfloat y, GLfloat z);

        void setOrthographic(GLfloat w, GLfloat h);
        void setPerspective(GLfloat fov, GLfloat near, GLfloat far);
        void setPerspective(GLfloat fov);

        //Get Functions
        GLfloat viewX() { return viewPosition.x; };
        GLfloat viewY() { return viewPosition.y; };
        GLfloat viewZ() { return viewPosition.z; };
        glm::vec4 viewPos() { return glm::vec4(viewPosition,1); };

        GLfloat viewYaw() { return viewAngle.x; };
        GLfloat viewPitch() { return viewAngle.y; };
        GLfloat viewRoll() { return viewAngle.z; };
        glm::vec3 viewRot() { return viewAngle; };
        glm::vec4 viewSightVector();

        GLfloat viewFOV() { return projFOV; };

        glm::vec2 getScreenPoint(glm::vec3 point);
        GLfloat getZ(glm::vec3 point);

        void setVerbose() { verbose = true; };

    private:
        GLFWwindow* window;
        int width;
        int height;

        //Draw Functions
        void drawSetup();
        void render();

        //Draw Variables
        std::vector<GLuint> programs;

        std::vector<OnionBuffer*> buffers;
        std::vector<bool> projectionUsage;
        std::vector<int> programUsage;

        //View Functions
        void viewSetup();

        //View Variables
        void calcViewMatrix();
        glm::vec3 viewPosition;
        glm::vec3 viewAngle;
        GLfloat projFOV;

        glm::mat4 viewMatrix;
        glm::mat4 projMatrix;

        bool verbose;
};



#endif // ONIONWINDOW_H
