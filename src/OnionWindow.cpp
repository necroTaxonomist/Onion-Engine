#include "OnionWindow.h"

int OnionWindow::numInstances = 0;

OnionWindow::OnionWindow(int w, int h, std::string name):
    width(w),
    height(h),
    verbose(false)
{
    if (Onion::onionInitGlfw())
    {
        window = glfwCreateWindow(w, h, name.data(), NULL, NULL);
        if (window == NULL)
            fprintf(stdout,"Shit's goin down.");
    }

    glfwMakeContextCurrent(window);

    Onion::onionInitGlew();

    drawSetup();
    viewSetup();

    ++numInstances;
}

OnionWindow::~OnionWindow()
{
    printf("WINDOW DESTRUCTOR STARTED\n");

    for (int p = 0; p < programs.size(); ++p)
    {
        glDeleteProgram(programs[p]);
    }

    for (int i = 0; i < buffers.size(); ++i)
    {
        delete buffers.at(i);
    }

    glfwTerminate();

    printf("WINDOW DESTRUCTOR FINISHED\n");

    --numInstances;
}

void OnionWindow::update()
{
    render();
}
