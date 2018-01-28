#include "OnionWindow.h"

void OnionWindow::drawSetup()
{
    #pragma warning( suppress : 4244 )
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);
}

void OnionWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int b = 0; b < buffers.size(); ++b)
    {
        if (programUsage.at(b) == -1)
        {
            buffers.at(b)->render();
        }
        else
        {
            if (!projectionUsage.at(b))
            {
                if (verbose)
                    printf("drew buffer %d without matrix\n",b);
                buffers.at(b)->render(programs.at(programUsage.at(b)));
            }
            else
            {
                if (verbose)
                    printf("drew buffer %d with matrix\n",b);
                buffers.at(b)->render(programs.at(programUsage.at(b)), projMatrix * viewMatrix);
            }
        }
    }

    glfwSwapBuffers(window);
}

void OnionWindow::setBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    glClearColor(r, g, b, a);
}

int OnionWindow::addProgram(std::string vertexFilename, std::string fragmentFilename)
{
    GLuint programID = Onion::onionLoadShaders(vertexFilename, fragmentFilename);
    programs.push_back(programID);
    //printf("LOADED %d,", programID);
    #pragma warning( suppress : 4267 )
    return programs.size() - 1;
}

int OnionWindow::addBuffer(int numAttribs, int programIndex, bool usesProjectionMatrix)
{
    OnionBuffer* newBuffer = new OnionBuffer(numAttribs);

    buffers.push_back(newBuffer);
    projectionUsage.push_back(usesProjectionMatrix);
    programUsage.push_back(programIndex);
    #pragma warning( suppress : 4267 )
    return buffers.size() - 1;
}

void OnionWindow::defineBufferAttrib(int bufferIndex,GLuint index,GLint size,GLboolean normalized,GLsizei stride,const GLvoid* pointer)
{
    buffers.at(bufferIndex)->defineAttrib(index,size,normalized,stride,pointer);
}

void OnionWindow::setBufferBackground(int bufferIndex, GLuint bgTexture)
{
    buffers.at(bufferIndex)->setBackground(bgTexture);
}

void OnionWindow::drawSurface(int bufferIndex, OnionSurface& surface)
{
    buffers.at(bufferIndex)->setDrawTexture(surface.getTexture());
    buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP, surface.getNumVertices(), surface.getPositions(), surface.getUV());
}
void OnionWindow::drawSurface(int bufferIndex, OnionSurface& surface, glm::mat4 trans)
{
    buffers.at(bufferIndex)->setDrawTexture(surface.getTexture());
    buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP, surface.getNumVertices(), surface.getPositions(trans), surface.getUV());
}
void OnionWindow::drawSurface(int bufferIndex, OnionSurface& surface, GLfloat dx, GLfloat dy, GLfloat dz)
{
    buffers.at(bufferIndex)->setDrawTexture(surface.getTexture());
    buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP, surface.getNumVertices(), surface.getPositions(dx,dy,dz), surface.getUV());
}

void OnionWindow::drawSurfaceEmpty(int bufferIndex, OnionSurface& surface)
{
    buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP, surface.getNumVertices(), surface.getPositions());
}
void OnionWindow::drawSurfaceEmpty(int bufferIndex, OnionSurface& surface, glm::mat4 trans)
{
    buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP, surface.getNumVertices(), surface.getPositions(trans));
}

void OnionWindow::drawLetter(int bufferIndex, OnionLetter* letter, int numVertices)
{
    int curSegment = 0;
    #pragma warning( suppress : 4018 )
    if (numVertices < 0 || numVertices >= letter->numVertices())
    {
        #pragma warning( suppress : 4018 )
        for (int i = 0; i < letter->numSegments(); ++i)
            buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP,
                                          letter->segmentSize(i),
                                          letter->getSegment(i,-1));
    }
    else
    {
        while (numVertices > 0)
        {
            #pragma warning( suppress : 4018 )
            if (numVertices >= letter->segmentSize(curSegment))
            {
                buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP,
                                              letter->segmentSize(curSegment),
                                              letter->getSegment(curSegment,-1));
                numVertices -= letter->segmentSize(curSegment);
            }
            else
            {
                buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP,
                                              numVertices,
                                              letter->getSegment(curSegment,numVertices));
                numVertices = 0;
            }
            ++curSegment;
        }
    }
}

void OnionWindow::drawLetter(int bufferIndex, OnionLetter* letter, int numVertices, glm::mat4 trans)
{
    int curSegment = 0;
    #pragma warning( suppress : 4018 )
    if (numVertices < 0 || numVertices >= letter->numVertices())
    {
        #pragma warning( suppress : 4018 )
        for (int i = 0; i < letter->numSegments(); ++i)
            buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP,
                                          letter->segmentSize(i),
                                          letter->getSegment(i,-1, trans));
    }
    else
    {
        while (numVertices > 0)
        {
            #pragma warning( suppress : 4018 )
            if (numVertices >= letter->segmentSize(curSegment))
            {
                buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP,
                                              letter->segmentSize(curSegment),
                                              letter->getSegment(curSegment,-1,trans));
                numVertices -= letter->segmentSize(curSegment);
            }
            else
            {
                buffers.at(bufferIndex)->draw(GL_TRIANGLE_STRIP,
                                              numVertices,
                                              letter->getSegment(curSegment,numVertices,trans));
                numVertices = 0;
            }
            ++curSegment;
        }
    }
}


void OnionWindow::drawLetterPortion(int bufferIndex, OnionLetter* letter, GLfloat portion)
{
    int numVertices = (int)(letter->numVertices() * portion);
    if (numVertices % 2 != 0)
        ++numVertices;
    drawLetter(bufferIndex, letter, numVertices);
}

void OnionWindow::drawLetterPortion(int bufferIndex, OnionLetter* letter, GLfloat portion, glm::mat4 trans)
{
    int numVertices = (int)(letter->numVertices() * portion);
    if (numVertices % 2 != 0)
        ++numVertices;
    drawLetter(bufferIndex, letter, numVertices, trans);
}


void OnionWindow::drawVertices(int bufferIndex, GLenum drawType, int numVertices, GLfloat arr0[])
{
    buffers.at(bufferIndex)->draw(drawType, numVertices, arr0);
}

void OnionWindow::drawVertices(int bufferIndex, GLenum drawType, int numVertices, GLfloat arr0[], GLfloat arr1[])
{
    buffers.at(bufferIndex)->draw(drawType, numVertices, arr0, arr1);
}

