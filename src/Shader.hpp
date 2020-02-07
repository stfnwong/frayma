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

#define MAX_NUM_SHADERS 2

/*
 * Shader class
 * Encapsulates a Shader
 */
class Shader
{
    private:
        GLuint gl_program;
        GLuint gl_shader[MAX_NUM_SHADERS];  // make a vector?

    public:
        Shader(const std::string& vert_shader_filename, const std::string& frag_shader_filename);
        virtual ~Shader();
        //void loadShader(const std::string& filename);
        void use(void);

    public:
        // Don't allow assignment or copy construction
        Shader(const Shader& that) = delete;
        Shader& operator=(const Shader& that) = delete;
};

#endif /*__FRAYMA_SHADER_HPP*/
