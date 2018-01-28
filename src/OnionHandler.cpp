#include "OnionHandler.h"

int OnionHandler::numInstances = 0;

OnionHandler::OnionHandler(OnionWindow* w)
{
    window = w->getWindow();
    ++numInstances;
}

OnionHandler::~OnionHandler()
{
    printf("HANDLER DESTRUCTOR RUN\n");
    --numInstances;
}

void OnionHandler::update()
{
    glfwPollEvents();
    for (auto iter : curPressed)
    {
        lastPressed[iter.first] = iter.second;
        if (iter.first >= GLFW_MOUSE_BUTTON_1 && iter.first <= GLFW_MOUSE_BUTTON_7)
            curPressed[iter.first] = glfwGetMouseButton(window, iter.first) == GLFW_PRESS;
        else
            curPressed[iter.first] = glfwGetKey(window, iter.first) == GLFW_PRESS;
    }
}

void OnionHandler::watchForKey(int key)
{
    lastPressed[key] = false;
    curPressed[key] = (glfwGetKey(window, key) == GLFW_PRESS);
}
void OnionHandler::stopWatchingKey(int key)
{
    lastPressed.erase(key);
    curPressed.erase(key);
}
bool OnionHandler::watchingKey(int key)
{
    try
    {
        return curPressed[key] || !curPressed[key];
    }
    catch (std::out_of_range)
    {
        return false;
    }
}

bool OnionHandler::keyPressed(int key)
{
    try
    {
        return curPressed[key] && !lastPressed[key];
    }
    catch (std::out_of_range)
    {
        watchForKey(key);
        return curPressed[key] && !lastPressed[key];
    }
}
bool OnionHandler::keyHeld(int key)
{
    try
    {
        return curPressed[key];
    }
    catch (std::out_of_range)
    {
        watchForKey(key);
        return curPressed[key];
    }
}
bool OnionHandler::keyReleased(int key)
{
    try
    {
        return !curPressed[key] && lastPressed[key];
    }
    catch (std::out_of_range)
    {
        watchForKey(key);
        return !curPressed[key] && lastPressed[key];
    }
}

bool OnionHandler::closeButtonPressed()
{
    return window == NULL || glfwWindowShouldClose(window) != 0;
}
