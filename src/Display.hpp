/*
 * DISPLAY 
 * Create an SDL display 
 *
 * Stefan Wong 2020
 */

#ifndef __FRAYMA_DISPLAY_HPP
#define __FRAYMA_DISPLAY_HPP

#include <string>
#include <SDL2/SDL.h>


// TODO: maybe some kind of factory in case we can't init
// GLEW in the ctor

class Display
{
    private:
        bool closed;
        bool init_failed;       // TODO : don't keep this!
        SDL_Window*   window;
        SDL_GLContext context;

    public:
        Display(int w, int h, const std::string& title);
        virtual ~Display();

        void update(void);
        void clear(float r, float g, float b, float a);
        bool isClosed(void) const;

    public:
        // Disallow copying, copy ctor
        Display(const Display& that) = delete;
        Display& operator=(const Display& that) = delete;

};

#endif /*__FRAYMA_DISPLAY_HPP*/
