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
        GLuint program;
        GLuint shaders[MAX_NUM_SHADERS];  // make a vector?
        bool   shader_ok;

    private:
        // Compile a single shader
        GLuint      compile_shader(const std::string& src, GLenum type);
        GLint       get_error(GLuint shader, GLuint flag, bool is_program);
        std::string get_error_log(GLuint shader, GLuint flag, bool is_program);

    public:
        Shader();
        virtual ~Shader();
        void   use(void);
        bool   ok(void) const;

        // Compile the shaders in this Shader object
        int    compile(const std::string& vert_src, const std::string& frag_src, const std::string& attrib);
        GLuint getProgram(void) const;

    public:
        // Don't allow assignment or copy construction
        Shader(const Shader& that) = delete;
        Shader& operator=(const Shader& that) = delete;
};

#endif /*__FRAYMA_SHADER_HPP*/
