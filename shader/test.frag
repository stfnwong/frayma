// Basic fragment shader for unit tests
#version 130 

varying vec2 tex_coord0;
uniform sampler2D diffuse;

void main()
{
    gl_FragColor = texture2D(diffuse, tex_coord0);
}
