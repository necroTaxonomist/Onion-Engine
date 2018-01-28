#include "OnionPage.h"

#define PI 3.1416f

OnionPage::OnionPage(GLfloat heightIn, int resolutionIn):
    cornerIndex(-1),
    height(heightIn),
    resolution(resolutionIn),
    returnValue(NULL)
{
    addVertex(0,0);
    addVertex(0,height);

    bool cornerFound = false;

    for (int i = 1; i < resolution; ++i)
    {
        GLfloat t = i/(GLfloat)resolution;

        addVertex(t,0);

        GLfloat m = glm::tan(PI/2 * (1 - t/2));
        GLfloat atEndY = m * (1 - t);

        if (atEndY <= height)
        {
            if (cornerFound)
            {
                addCorner(1,height,t,0);
                cornerFound = true;
            }
            addVertex(1, atEndY);
        }
        else
        {
            GLfloat atTopX = t + height / m;
            addVertex(atTopX, height);
        }
    }

    generateUV();
}

OnionPage::~OnionPage()
{
    delete uvCoords;
}

void OnionPage::addCorner(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    #pragma warning( suppress : 4267 )
    cornerIndex = points.size();
    points.push_back(glm::vec2(x1,y1));
    points.push_back(glm::vec2(x2,y2));
}

void OnionPage::addVertex(GLfloat x, GLfloat y)
{
    points.push_back(glm::vec2(x,y));
}

void OnionPage::generateUV()
{
    uvCoords = new GLfloat[2*points.size()];
    for (int i = 0; i < points.size(); ++i)
    {
        uvCoords[2*i] = (points[i]).x;
        uvCoords[2*i+1] = 1.0f - ((points[i]).y / height);
    }
}

GLfloat* OnionPage::getPositions(glm::mat4 trans, GLfloat progress)
{
    int curlUntil;
    //calculate curlUntil

    if (returnValue != NULL)
        delete returnValue;
    returnValue = new GLfloat[points.size()*3];

    for (int i = 0; i < points.size(); ++i)
    {
        returnValue[3*i] = (points[i]).x;
        returnValue[3*i+1] = (points[i]).y;
        returnValue[3*i+2] = 0;
    }

    return returnValue;
}

void OnionPage::printPoints()
{
    if (returnValue == NULL)
        getPositions();
    /* as matrix
    printf("[");
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < points.size(); ++c)
        {
            printf("%f ", returnValue[3*c+r]);
        }
        if (r == 2)
            printf("]");
        printf("\n");
    }
    */

    for (int i = 0; i < points.size(); ++i)
    {
        printf("(%f,%f,%f) ", returnValue[3*i],
                              returnValue[3*i+1],
                              returnValue[3*i+2]);
    }
    printf("\n");
}
