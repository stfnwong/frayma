/*
 * DISPLAY 
 * Create an SDL display 
 *
 * Stefan Wong 2020
 */

#include <iostream>
#include <GL/glew.h>
#include "Display.hpp"


/*
 * Display() 
 */
Display::Display(int w, int h, const std::string& title)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    this->window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            w,
            h,
            SDL_WINDOW_OPENGL
    );

    this->context = SDL_GL_CreateContext(this->window);

    GLenum status = glewInit();
    if(status != GLEW_OK)
    {
        this->init_failed = true;
        std::cerr << "[" << __func__ << "] failed to init GLEW, and also this ctor pattern is bad" << std::endl;
    }
    else
        this->init_failed = false;

    this->closed = false;
}

/*
 * ~Display()
 */
Display::~Display()
{
    SDL_GL_DeleteContext(this->context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}


/*
 * clear()
 */
void Display::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

/*
 * update()
 */
void Display::update(void)
{
    SDL_Event e;
    SDL_GL_SwapWindow(this->window);

    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
            this->closed = true;
    }
}

/*
 * isClosed()
 */
bool Display::isClosed(void) const
{
    return this->closed;
}

