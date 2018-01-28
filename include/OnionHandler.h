#ifndef ONIONHANDLER_H
#define ONIONHANDLER_H

#include <map>

#include "OnionWindow.h"

class OnionHandler
{
    public:
        static int numInstances;

        OnionHandler(OnionWindow* w);
        ~OnionHandler();

        void update();

        void watchForKey(int key);
        void stopWatchingKey(int key);
        bool watchingKey(int key);

        bool keyPressed(int key);
        bool keyHeld(int key);
        bool keyReleased(int key);

        bool closeButtonPressed();

    private:
        GLFWwindow* window;
        std::map<int,bool> lastPressed;
        std::map<int,bool> curPressed;
};

#endif // ONIONHANDLER_H
