/*
    FRACTAL FRAGMENT SHADER
    Takes parts from various places
*/

#version 120
// Constants (NOTE: how to make these settable outside shader source file?)
#define M_PI        3.14159265358979
#define MIN_DIST    0.00001      // NOTE: probably need to tune this...
#define MAX_MARCHES 1000
#define EXPOSURE    1.0

// Depth of Field 
#define DOF_STRENGTH 0.0
#define DOF_DIST     1.0
// Level of Detail
#define LOD_MULT     10.0

// Field of View
#define FIELD_OF_VIEW 60.0      // degrees

// Lighting / Colors
#define LIGHT_COL       vec3(1.0, 0.9, 0.8)
#define BG_COL          vec3(0.6, 0.6, 0.6)
#define SHADOW_DARKNESS 0.8

uniform mat4 mat_cur;
uniform mat4 mat_prev;
uniform vec3 i_resolution;

const float FOCAL_DIST = 1.0 / tan(M_PI * FIELD_OF_VIEW / 360.0);

// Utils 
float rand(float s, float v_min, float v_max)
{
    float r = sin(s * s * 27.12345 + 1000.9876 / (s * s + 1e-5));
    return (r + 1.0) * 0.5 * (v_max - v_min) + v_min;
}


// ======== RAY MARCHER CODE ======== //
/*
    ray_march()
*/
vec4 ray_march(inout vec4 p, vec4 ray, float sharpness, float td)
{
    float d = MIN_DIST;
    float min_d = 1.0;
    float s;
    
    for(s = 0.0; s < MAX_MARCHES; s += 1.0)
    {
        d = dist_est(p);
        if(d < MIN_DIST)
        {
            s += d / MIN_DIST;
            break;
        }
        else if(td > MAX_DIST)
            break;
        td += d;
        p += ray * d;
        min_d = min(min_d, sharpness * d / td);
    }

    return vec4(d, s, td, min_d);
}

/*
    raymarch()
*/
vec4 raymarch(inout vec4 p, vec3 ray, float sharpness, float td)
{
    return ray_march(p, vec4(ray, 0.0), sharpness, td);
}

// Compute surface normals
// Implementation taken from http://www.iquilezles.org/www/articles/normalsSDF/normalsSDF.htm
vec3 surf_norm(vec4 p, float dx)
{
    const vec3 k = vec3(1, -1, 0);
    return normalize(
            k.xyy * dist_est(p + k,xyyz * dx) + 
            k.yyx * dist_est(p + k.yyxz * dx) + 
            k.yxy * dist_est(p + k.yxyz * dx) + 
            k.xxx * dist_est(p + k.xxxz * dx)
    );
}

/*
    get_pixel()
*/
vec4 get_pixel(inout vec4 origin, inout vec4 ray, float td)
{
    // trace the ray
    vec4 p = origin;
    vec4 dist_ray_march = raymarch(p, ray, 0.5, td);
    
    float d = dist_ray_march.x;
    float s = dist_ray_march.y;
    td = dist_ray_march.z;
    float m = dist_ray_march.w;

    vec3 color = vec3(0.0);
    float dist_min = MIN_DIST * max(td * LOD_MULT, 1.0);

    // ray hit something
    if(d < dist_min)
    {
        float k = 1.0;
        vec3 n = surf_norm(p, MIN_DIST * 10.0);
        vec3 reflected = ray.xyz - 2.0 * dot(ray.xyz, n) * n;

        // TODO : Shadows go here

        // color this pixel
        vec3 orig_col = clamp(COL(p).xyz, 0.0, 1.0);    // what to do for color macro?

        k = max(k, 1.0 - SHADOW_DARKNESS);
        col += orig_col * LIGHT_COL * k;
    }
    // ray missed
    else
    {
        col += BG_COL;
    }

    return vec4(col, td);
}


/*
    main()
*/
void main()
{
    vec4 col = vec4(0,0);

    // Loop over the scene. NOTE: are we doing overampling here?
    for(int i = 0; i < AA_SAMPLES; ++i)
    {
        for(int j = 0; j < AA_SAMPLES; ++j)
        {
            mat4 mat = mat_cur;
            vec2 delta = vec2(i, j) / AA_SAMPLES;
            vec2 delta2 = vec2(rand(i, 0, 1), rand(j+0.1, 0, 1);
            vec4 dxy = vec4(delta2.x, delta2.y, 0.0, 0.0) * DOF_STRENGTH / i_resolution.x;
            // get normalized screen coords
            vec2 screen_pos = (gl_FragCoord.xy + delta) / i_resolution.xy;
            vec3 uv = 2 * screen_pos - 1;
            uv.x *= i_resolution.x / i_resolution.y;
            // convert to ray 
            vec4 ray = normalize(vec4(uv.x, uv.y, -FOCAL_DIST, 0.0));
            ray = mat * normalize(ray * DOF_DISTANCE + dxy);

            // cast a ray into the scene
            vec4 p = mat[3] - mat * dxy;

            col += get_pixel(p, ray, 0.0);
        }
    }

    col = col / (AA_SAMPLES * AA_SAMPLES);
    gl_FragColor.rgb = clamp(col.xyz * EXPOSURE, 0.0, 1.0);
    gl_FragDepth = min(col.w / MAX_DIST, 0.99999);
}
