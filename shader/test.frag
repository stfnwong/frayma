// Basic fragment shader for unit tests
#version 130 

varying vec2 tex_coord0;
uniform sampler2D diffuse;

void main()
{
    vec2 xy = gl_FragCoord.xy;      // current pixel coord
    //xy.x = xy.x / 
    gl_FragColor = texture2D(diffuse, tex_coord0);
}
