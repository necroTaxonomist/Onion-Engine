#include "OnionLetter.h"

#define PI 3.14159265359f

int OnionLetter::numInstances = 0;

OnionLetter::OnionLetter():
    returnValue(NULL),
    width(0.0f)
{
    ++numInstances;
}

OnionLetter::~OnionLetter()
{
    for (int i = 0; i < segments.size(); ++i)
    {
        delete[] segments[i];
    }
    delete[] returnValue;
    --numInstances;
}

void OnionLetter::addCurve(std::vector<glm::vec2>& leftCurve, std::vector<glm::vec2>& rightCurve, GLfloat resolution)
{
    #pragma warning( suppress : 4244 )
    segments.push_back(new GLfloat[4 * (resolution + 1)]);
    #pragma warning( suppress : 4267 )
    int cur = segments.size() - 1;
    #pragma warning( suppress : 4244 )
    sizes.push_back(2 * (resolution + 1));
    //printf("rightCurve[2].x = %f, rightCurve[2].y = %f\n", rightCurve.at(2).x, rightCurve.at(2).y);

    for (int i = 0; i <= resolution; ++i)
    {
        GLfloat leftX = Onion::cubicBezier(leftCurve.at(0).x,
                                           leftCurve.at(1).x,
                                           leftCurve.at(2).x,
                                           leftCurve.at(3).x,
                                           i / resolution);
        GLfloat leftY = Onion::cubicBezier(leftCurve.at(0).y,
                                           leftCurve.at(1).y,
                                           leftCurve.at(2).y,
                                           leftCurve.at(3).y,
                                           i / resolution);

        GLfloat rightX = Onion::cubicBezier(rightCurve.at(0).x,
                                            rightCurve.at(1).x,
                                            rightCurve.at(2).x,
                                            rightCurve.at(3).x,
                                            i / resolution);
        GLfloat rightY = Onion::cubicBezier(rightCurve.at(0).y,
                                            rightCurve.at(1).y,
                                            rightCurve.at(2).y,
                                            rightCurve.at(3).y,
                                            i / resolution);
        segments[cur][4*i] = leftX;
        segments[cur][4*i+1] = leftY;
        segments[cur][4*i+2] = rightX;
        segments[cur][4*i+3] = rightY;
        /*printf("%f: (%f,%f) (%f,%f)\n", i / resolution,
                                        segments[cur][4*i],
                                        segments[cur][4*i+1],
                                        segments[cur][4*i+2],
                                        segments[cur][4*i+3]);*/
        if (rightY >= 0 && rightX > width)
            width = rightX;
        if (leftY >= 0 && leftX > width)
            width = leftX;
    }
}

void OnionLetter::addCurve(GLfloat xL0, GLfloat yL0,
                           GLfloat xL1, GLfloat yL1,
                           GLfloat xL2, GLfloat yL2,
                           GLfloat xL3, GLfloat yL3,
                           GLfloat xR0, GLfloat yR0,
                           GLfloat xR1, GLfloat yR1,
                           GLfloat xR2, GLfloat yR2,
                           GLfloat xR3, GLfloat yR3,
                           GLfloat resolution)
{
    std::vector<glm::vec2> leftCurve;
    std::vector<glm::vec2> rightCurve;

    leftCurve.push_back(glm::vec2(xL0,yL0));
    leftCurve.push_back(glm::vec2(xL1,yL1));
    leftCurve.push_back(glm::vec2(xL2,yL2));
    leftCurve.push_back(glm::vec2(xL3,yL3));

    rightCurve.push_back(glm::vec2(xR0,yR0));
    rightCurve.push_back(glm::vec2(xR1,yR1));
    rightCurve.push_back(glm::vec2(xR2,yR2));
    rightCurve.push_back(glm::vec2(xR3,yR3));

    addCurve(leftCurve, rightCurve, resolution);
}

void OnionLetter::addCurve(std::vector<glm::vec2>& curve, GLfloat resolution, GLfloat thickness, bool connected)
{
    #pragma warning( suppress : 4244 )
    segments.push_back(new GLfloat[4 * (resolution + 1)]);
    #pragma warning( suppress : 4267 )
    int cur = segments.size() - 1;
    #pragma warning( suppress : 4244 )
    sizes.push_back(2 * (resolution + 1));
    //printf("rightCurve[2].x = %f, rightCurve[2].y = %f\n", rightCurve.at(2).x, rightCurve.at(2).y);

    GLfloat prevX, prevY, curX, curY, nextX, nextY;

    for (int i = 0; i <= resolution; ++i)
    {
        if (i == 0)
        {
            /*prevX = Onion::cubicBezier(curve.at(0).x, curve.at(1).x,
                                      curve.at(2).x, curve.at(3).x,
                                      (i-1) / resolution);
            prevY = Onion::cubicBezier(curve.at(0).y, curve.at(1).y,
                                      curve.at(2).y, curve.at(3).y,
                                      (i-1) / resolution);*/
            curX = Onion::cubicBezier(curve.at(0).x, curve.at(1).x,
                                      curve.at(2).x, curve.at(3).x,
                                      i / resolution);
            curY = Onion::cubicBezier(curve.at(0).y, curve.at(1).y,
                                      curve.at(2).y, curve.at(3).y,
                                      i / resolution);
            prevX = curX;
            prevY = curY;
        }
        else
        {
            prevX = curX;
            prevY = curY;
            curX = nextX;
            curY = nextY;
        }
        if (i == resolution)
        {
            nextX = curX;
            nextY = curY;
        }
        else
        {
            nextX = Onion::cubicBezier(curve.at(0).x, curve.at(1).x,
                                   curve.at(2).x, curve.at(3).x,
                                   (i+1) / resolution);
            nextY = Onion::cubicBezier(curve.at(0).y, curve.at(1).y,
                                       curve.at(2).y, curve.at(3).y,
                                       (i+1) / resolution);
        }


        GLfloat frontDir;
        GLfloat backDir;

        if (i == 0)
        {
            frontDir = glm::atan(nextY - curY, nextX - curX);
            backDir = clampAngle(frontDir + PI);
        }
        else if (i == resolution)
        {
            backDir = glm::atan(prevY - curY, prevX - curX);
            frontDir = clampAngle(backDir + PI);
        }
        else
        {
            frontDir = glm::atan(nextY - curY, nextX - curX);
            backDir = glm::atan(prevY - curY, prevX - curX);
        }

        GLfloat leftDir = angleMidpoint(frontDir, backDir);
        GLfloat rightDir = angleMidpoint(backDir, frontDir);

        //printf("front=%f back=%f\n", frontDir / PI, backDir / PI);
        //printf("left=%f right=%f\n", leftDir / PI, rightDir / PI);

        GLfloat leftX = curX + thickness/2 * glm::cos(leftDir);
        GLfloat leftY = curY + thickness/2 * glm::sin(leftDir);
        GLfloat rightX = curX + thickness/2 * glm::cos(rightDir);
        GLfloat rightY = curY + thickness/2 * glm::sin(rightDir);
        if (connected && i == 0 && cur >= 1)
        {
            GLfloat prevLeftX = segments[cur-1][2*segmentSize(cur-1)-4];
            GLfloat prevLeftY = segments[cur-1][2*segmentSize(cur-1)-3];
            GLfloat prevRightX = segments[cur-1][2*segmentSize(cur-1)-2];
            GLfloat prevRightY = segments[cur-1][2*segmentSize(cur-1)-1];

            GLfloat prevDir = glm::atan(prevRightY - prevLeftY, prevRightX - prevLeftX) + PI/2;
            //printf("prevDir=%f\nfrontDir=%f\n", prevDir, frontDir);
            //printf("angleDifference=%f\n", angleDifference(prevDir,frontDir));

            GLfloat angleDiff = clampAngle(angleDifference(prevDir,frontDir));

            if (angleDiff >= PI/2 && angleDiff <= 3*PI/2)
            {
                leftX = (leftX + prevRightX) / 2;
                leftY = (leftY + prevRightY) / 2;
                rightX = (rightX + prevLeftX) / 2;
                rightY = (rightY + prevLeftY) / 2;
                segments[cur-1][2*segmentSize(cur-1)-4] = rightX;
                segments[cur-1][2*segmentSize(cur-1)-3] = rightY;
                segments[cur-1][2*segmentSize(cur-1)-2] = leftX;
                segments[cur-1][2*segmentSize(cur-1)-1] = leftY;
            }
            else
            {
                //printf("this is happening\n");
                leftX = (leftX + prevLeftX) / 2;
                leftY = (leftY + prevLeftY) / 2;
                rightX = (rightX + prevRightX) / 2;
                rightY = (rightY + prevRightY) / 2;
                segments[cur-1][2*segmentSize(cur-1)-4] = leftX;
                segments[cur-1][2*segmentSize(cur-1)-3] = leftY;
                segments[cur-1][2*segmentSize(cur-1)-2] = rightX;
                segments[cur-1][2*segmentSize(cur-1)-1] = rightY;
            }


        }

        segments[cur][4*i] = leftX;
        segments[cur][4*i+1] = leftY;
        segments[cur][4*i+2] = rightX;
        segments[cur][4*i+3] = rightY;
        /*printf("%f: (%f,%f) (%f,%f)\n", i / resolution,
                                        segments[cur][4*i],
                                        segments[cur][4*i+1],
                                        segments[cur][4*i+2],
                                        segments[cur][4*i+3]);*/
        if (rightY >= 0 && rightX > width)
            width = rightX;
        if (leftY >= 0 && leftX > width)
            width = leftX;
    }
}

void OnionLetter::addCurve(GLfloat x0, GLfloat y0,
                           GLfloat x1, GLfloat y1,
                           GLfloat x2, GLfloat y2,
                           GLfloat x3, GLfloat y3,
                           GLfloat resolution,
                           GLfloat thickness,
                           bool connected)
{
    std::vector<glm::vec2> curve;

    curve.push_back(glm::vec2(x0,y0));
    curve.push_back(glm::vec2(x1,y1));
    curve.push_back(glm::vec2(x2,y2));
    curve.push_back(glm::vec2(x3,y3));

    addCurve(curve, resolution, thickness, connected);
}

GLfloat* OnionLetter::getSegment(unsigned int segmentIndex, int stopIndex)
{
    #pragma warning( suppress : 4018 )
    int len = sizes.at(segmentIndex);

    if (returnValue != NULL)
        delete returnValue;
    if (stopIndex < 0 || stopIndex >= len)
    {
        returnValue = new GLfloat[len*3];
        for (int i = 0; i < len; ++i)
        {
            returnValue[3*i] = segments[segmentIndex][2*i];
            returnValue[3*i+1] = segments[segmentIndex][2*i+1];
            returnValue[3*i+2] = 0.0f;
        }
        return returnValue;
    }
    else
    {
        returnValue = new GLfloat[stopIndex*3];
        for (int i = 0; i < stopIndex; ++i)
        {
            returnValue[3*i] = segments[segmentIndex][2*i];
            returnValue[3*i+1] = segments[segmentIndex][2*i+1];
            returnValue[3*i+2] = 0.0f;
        }
        return returnValue;
    }
}

GLfloat* OnionLetter::getSegment(unsigned int segmentIndex, int stopIndex, glm::mat4 trans)
{
    #pragma warning( suppress : 4018 )
    int len = sizes.at(segmentIndex);

    if (returnValue != NULL)
        delete returnValue;
    if (stopIndex < 0 || stopIndex >= len)
    {
        returnValue = new GLfloat[len*3];
        for (int i = 0; i < len; ++i)
        {
            returnValue[3*i] = segments[segmentIndex][2*i];
            returnValue[3*i+1] = segments[segmentIndex][2*i+1];
            returnValue[3*i+2] = 0.0f;
        }
        Onion::transform(returnValue, trans, len);
        return returnValue;
    }
    else
    {
        returnValue = new GLfloat[stopIndex*3];
        for (int i = 0; i < stopIndex; ++i)
        {
            returnValue[3*i] = segments[segmentIndex][2*i];
            returnValue[3*i+1] = segments[segmentIndex][2*i+1];
            returnValue[3*i+2] = 0.0f;
        }
        Onion::transform(returnValue, trans, stopIndex);
        return returnValue;
    }
}

unsigned int OnionLetter::segmentSize(unsigned int segmentIndex)
{
    return sizes.at(segmentIndex);
}

unsigned int OnionLetter::numVertices()
{
    unsigned int num = 0;
    #pragma warning( suppress : 4018 )
    for (int i = 0; i < numSegments(); ++i)
    {
        num += segmentSize(i);
    }
    return num;
}

void OnionLetter::printVertices()
{
    for (int cur = 0; cur < segments.size(); ++cur)
    {
        for (int i = 0; i <= 8.0; ++i)
        {
            printf("%f: (%f,%f) (%f,%f)\n", i / 8,
                                        segments[cur][4*i],
                                        segments[cur][4*i+1],
                                        segments[cur][4*i+2],
                                        segments[cur][4*i+3]);
        }
    }
}

GLfloat OnionLetter::angleDifference(GLfloat low, GLfloat high)
{
    if (high >= low)
        return high - low;
    else
        return 2 * PI - (high - low);
}

GLfloat OnionLetter::angleMidpoint(GLfloat low, GLfloat high)
{
    if (high >= low)
        return (high + low) / 2;
    else
    {
        while (high < low)
            high += 2*PI;
        GLfloat point = (high + low) / 2;
        return clampAngle(point);
    }
}

GLfloat OnionLetter::clampAngle(GLfloat angle)
{
    while (angle < 0)
        angle += 2*PI;
    while (angle >= 2*PI)
        angle -= 2*PI;
    return angle;
}
