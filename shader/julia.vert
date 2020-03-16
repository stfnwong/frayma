/*
    VERTEX SHADER FOR JULIA SET

Let's see if I can render a Julia set

*/

#version 120

/*
    calc_normal()
Find surface normal
*/
vec3 calc_normal(in vec3 pos, in float t, in float px )
{
    vec4 tmp;
    vec2 e = vec2(1.0,-1.0)*0.5773*0.25*px;
    return normalize( e.xyy*map( pos + e.xyy,tmp ) + 
					  e.yyx*map( pos + e.yyx,tmp ) + 
					  e.yxy*map( pos + e.yxy,tmp ) + 
					  e.xxx*map( pos + e.xxx,tmp ) );
}

// ======== ENTRY POINT ========= //
void main()
{

}
