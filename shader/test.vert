// Basic vertex shader for unit tests 

#version 110 

attribute vec3 position;
attribute vec2 tex_coord;

varying vec2 tex_coord0;

void main()
{
    gl_Position = vec4(position, 1.0);
    tex_coord0 = tex_coord;
}
