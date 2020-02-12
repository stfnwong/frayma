/*
 * SHADER
 * Represents a Shader 
 *
 * Stefan Wong 2020
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.hpp"


/*
 * load_shader()
 */
std::string load_shader(const std::string& filename)
{
    std::ifstream file;

    file.open(filename.c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
        std::cerr << "[" << __func__ << "] failed to loader shader from file " << filename << std::endl;

    return output;
}


// ======== SHADER OBJECT ======= //
Shader::Shader()
{
    this->shader_ok = false;
}

/*
 * ~Shader()
 */
Shader::~Shader()
{
    for(int i = 0; i < MAX_NUM_SHADERS; ++i)
    {
        glDetachShader(this->program, this->shaders[i]);
        glDeleteShader(this->shaders[i]);
    }
    glDeleteProgram(this->program);
}

/*
 * compile_shader()
 */
GLuint Shader::compile_shader(const std::string& src, GLenum type)
{
    GLuint shader;
    const GLchar* shader_src = src.c_str(); 

    shader = glCreateShader(type);
    if(shader == 0)
    {
        std::cout << "[" << __func__ << "] failed to create shader" << std::endl;
        return shader;
    }
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);
    
    return shader;
}

/*
 * get_error()
 */
GLint Shader::get_error(GLuint shader, GLuint flag, bool is_program)
{
    GLint status = 0;
    if(is_program)
        glGetProgramiv(shader, flag, &status);
    else
        glGetShaderiv(shader, flag, &status);

    return status;
}

/*
 * get_error_log()
 */
std::string Shader::get_error_log(GLuint shader, GLuint flag, bool is_program)
{
    GLint status = 0;
    GLchar info_log[512];
    std::ostringstream oss;

    status = this->get_error(shader, flag, is_program);
    if(status == GL_FALSE)
    {
        if(is_program)
        {
            glGetProgramInfoLog(shader, sizeof(info_log), NULL, info_log);
            oss << "ERROR : Program failed to link with error '"
                << info_log << "'";
        }
        else
        {
            glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
            oss << "ERROR : Shader failed to compile with error '"
                << info_log << "'";
        }
    }
    
    return oss.str();
}

/*
 * use()
 */
void Shader::use(void)
{
    glUseProgram(this->program);
}

/*
 * ok()
 */
bool Shader::ok(void) const
{
    return this->shader_ok;
}

/*
 * compile()
 */
int Shader::compile(const std::string& frag_src, const std::string& vert_src, const std::string& attrib)
{
    std::string fshader_source = load_shader(frag_src);
    std::string vshader_source = load_shader(vert_src);

    // Create the program
    this->program = glCreateProgram();

    // frag shader 
    this->shaders[0] = this->compile_shader(fshader_source, GL_FRAGMENT_SHADER);

    // vert shader
    this->shaders[1] = this->compile_shader(vshader_source, GL_VERTEX_SHADER);

    for(int i = 0; i < MAX_NUM_SHADERS; ++i)
    {
        if(!this->shaders[i])
        {
            std::cerr << "[" << __func__ << "] shader compilation failed" << std::endl;
            return -1;
        }
        glAttachShader(this->program, this->shaders[i]);
    }

    glBindAttribLocation(this->program, 0, attrib.c_str());
    glLinkProgram(this->program);
    if(this->get_error(this->program, GL_LINK_STATUS, true))
    {
        std::cout << "[" << __func__ << "] " << this->get_error_log(
                this->program,
                GL_LINK_STATUS,
                true) << std::endl;
    }

    glValidateProgram(this->program);
    if(this->get_error(this->program, GL_VALIDATE_STATUS, true))
    {
        std::cout << "[" << __func__ << "] " << this->get_error_log(
                this->program,
                GL_VALIDATE_STATUS,
                true) << std::endl;
    }

    this->shader_ok = true;

    return 0;
}

/*
 * getProgram()
 */
// TODO : do I want to return a reference here?
GLuint Shader::getProgram(void) const
{
    return this->program;
}
