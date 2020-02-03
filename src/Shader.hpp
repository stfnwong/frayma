/*
 * SHADER
 * Represents a Shader 
 *
 * Stefan Wong 2020
 */

#ifndef __FRAYMA_SHADER_HPP
#define __FRAYMA_SHADER_HPP

#include <string>
#include <GL/glew.h>

#define MAX_NUM_SHADERS 4

class Shader
{
    private:
        GLuint gl_program;
        GLuint gl_shader[MAX_NUM_SHADERS];  // make a vector?

    public:
        Shader(const std::string& filename);
        virtual ~Shader();
        void bind(void);

    public:
        // Don't allow assignment or copy construction
        Shader(const Shader& that) = delete;
        Shader& operator=(const Shader& that) = delete;
};


#endif /*__FRAYMA_SHADER_HPP*/
