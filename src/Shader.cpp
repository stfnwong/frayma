/*
 * SHADER
 * Represents a Shader 
 *
 * Stefan Wong 2020
 */

#include <iostream>
#include <fstream>
#include "Shader.hpp"

// TODO: re-do the shader compilation stages. I might follow the 
// design from the Joey DeVries book here.

// Util functions
/*
 * check_shader_error()
 */
void check_shader_error(GLuint shader, GLuint flag, bool is_program, const std::string& err_msg)
{
    GLint success = 0;
    GLchar error[1024];

    if(is_program)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(is_program)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << "[" << __func__ << "] " << err_msg << " '"
            << error << "'" << std::endl;
    }
}

/*
 * create_shader()
 */
GLuint create_shader(const std::string& shader_text, GLenum shader_type)
{
    GLuint shader;

    shader = glCreateShader(shader_type);
    if(shader == 0)
    {
        std::cerr << "[" << __func__ << "] ERROR: shader creation failed" << std::endl;
        return -1;
    }

    const GLchar* shader_source[1];
    GLint shader_source_len[1];

    shader_source[0] = shader_text.c_str();
    shader_source_len[0] = shader_text.length();

    // DEBUG: remove 
    std::cout << "[" << __func__ << "] shader_source[0] : " << std::endl;
    std::cout << shader_source[0] << std::endl;

    glShaderSource(shader, 1, shader_source, shader_source_len);
    check_shader_error(
            shader, 
            GL_COMPILE_STATUS, 
            false, 
            "ERROR: Shader compilation failed"
    ); 

    return shader;
}

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
Shader::Shader(const std::string& vert_shader_filename, const std::string& frag_shader_filename)
{
    GLint success;
    GLchar info_log[512];


    //this->gl_shader[0] = create_shader(load_shader(vert_shader_filename), GL_VERTEX_SHADER);
    //this->gl_shader[1] = create_shader(load_shader(frag_shader_filename), GL_FRAGMENT_SHADER);

    std::string vshader_source = load_shader(vert_shader_filename);
    std::string fshader_source = load_shader(frag_shader_filename);

    // For now, lets just put all the shader code here and worry about cleaning up later
    const GLchar* vs_code = vshader_source.c_str(); 
    const GLchar* fs_code = fshader_source.c_str(); 

    // vertex shader 
    this->gl_shader[0] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->gl_shader[0], 1, &vs_code, NULL);
    glCompileShader(this->gl_shader[0]);
    glGetShaderiv(this->gl_shader[0], GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(this->gl_shader[0], 512, NULL, info_log);
        std::cout << "[" << __func__ << "] shader compilation log '" 
            << info_log << "' " << std::endl;
    }

    // fragment shader
    this->gl_shader[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->gl_shader[1], 1, &fs_code, NULL);
    glCompileShader(this->gl_shader[1]);
    glGetShaderiv(this->gl_shader[1], GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(this->gl_shader[1], 512, NULL, info_log);
        std::cout << "[" << __func__ << "] shader compilation log '" 
            << info_log << "' " << std::endl;
    }

    // Create the program
    this->gl_program = glCreateProgram();

    for(int i = 0; i < MAX_NUM_SHADERS; ++i)
        glAttachShader(this->gl_program, this->gl_shader[i]);

    // For now we just fix the attributes here. Not sure it its worth trying to make this more dynamic?
    glBindAttribLocation(this->gl_program, 0, "position");
    glBindAttribLocation(this->gl_program, 1, "tex_coord");

    glLinkProgram(this->gl_program);
    check_shader_error(this->gl_program, GL_LINK_STATUS, true, "ERROR: Failed to link program");

    glValidateProgram(this->gl_program);
    check_shader_error(this->gl_program, GL_VALIDATE_STATUS, true, "ERROR: Failed to validate program");
}

/*
 * ~Shader()
 */
Shader::~Shader()
{
    for(int i = 0; i < MAX_NUM_SHADERS; ++i)
    {
        glDetachShader(this->gl_program, this->gl_shader[i]);
        glDeleteShader(this->gl_shader[i]);
    }
    glDeleteProgram(this->gl_program);
}

/*
 * use()
 */
void Shader::use(void)
{
    glUseProgram(this->gl_program);
}
