#version 410 core

uniform float fGlobalTime; // in seconds
uniform vec2 v2Resolution; // viewport resolution (in pixels)

uniform sampler1D texFFT; // towards 0.0 is bass / lower freq, towards 1.0 is higher / treble freq
uniform sampler1D texFFTSmoothed; // this one has longer falloff and less harsh transients
uniform sampler1D texFFTIntegrated; // this is continually increasing

layout(location = 0) out vec4 out_color; // out_color must be written in order to see anything

vec4 plas( vec2 v, float time )
{
  float c = 0.5 + sin( v.x * 10.0 ) + cos( sin( time + v.y ) * 20.0 );
  return vec4( sin(c * 0.2 + cos(time)), c * 0.15, cos( c * 0.1 + time / .4 ) * .25, 1.0 );
}

// rotate around a 
mat2 rot(float a)
{
    float ca = cos(a);
    float sa = sin(a);
      
    return mat2(ca, sa, -sa, ca);
}


// primitives 
float sphere(vec3 p, float r)
{
    return length(p) - r;
}

float box(vec3 p, vec3 r)
{
    vec3 ap = abs(p) - r;
    return length(max(vec3(0.0), ap)) + min(0.0, max(ap.x, max(ap.y, ap.z)));
}

float slice(vec3 p)
{
  p.xz *= rot(fGlobalTime);
  
  for(int i = 0; i < 5; ++i)
  {
    float t_param = fGlobalTime * 0.1 + sin(i);
    p.xz *= rot(t_param);
    p -= 0.2;
    p.zy *= rot(t_param * 0.85);
    p = abs(p);
    p -= 0.1 + (0.12 * i);
    //p.zy *= rot(t_param * 0.75);
  }
  
  //vec3 box_rad = vec3(0.1, sin(fGlobalTime) / 2.0, 0.3);
  vec3 box_rad = vec3(0.4, 0.25, 0.2);
  return box(p, box_rad);
}

// primitive mapping
float map(vec3 p)
{
  return slice(p);
}



vec3 norm(vec3 p)
{
  vec2 off = vec2(0.01, 0.0);
  return normalize(map(p) - vec3(map(p - off.xyy), map(p - off.yxy), map(p - off.yyx)));
}


// camera methods
void cam(inout vec3 p)
{
  float t_param = fGlobalTime * 0.3;
  p.yz *= rot(t_param);
  p.zx *= rot(t_param * 1.2);
}


float mat = 0;
void main(void)
{
  vec2 uv = vec2(gl_FragCoord.x / v2Resolution.x, gl_FragCoord.y / v2Resolution.y);
  uv -= 0.5;
  uv /= vec2(v2Resolution.y / v2Resolution.x, 1);

  vec3 s = vec3(0.0, 0.0, -10.0);
  vec3 r = normalize(vec3(-uv, 0.7));
  
  // rotate camera 
  cam(s);
  cam(r);

  vec3 col = vec3(0);
  vec3 lpos = vec3(0);
  
  
  vec3 p = s;
  int i;
  float prod = 1.0;
  for(i = 0; i < 100; ++i)
  {
    float d = map(p);
    if(d < 0.001)
    {
      // colorize
      float curmat = mat;
      
      vec3 n = norm(p);
      vec3 l = normalize(lpos - p);
      float fog = 1.0 - i / 100.0;
      float fresnel = pow(1.0 - abs(dot(n, r)), 2.0);
      vec3 h = normalize(l - r);
      
      float back = (curmat == 2.0) ? 0.0 : 1.0;
      float gold  = (curmat == 1.0) ? 1.0 : 0.0;
      vec3 diff = mix(vec3(0.0, 0.1, 8.0), vec3(1.0, 0.2, 0.2), gold);
      
      // apply transforms to pixel color
      col += back * prod * max(0.0, dot(n, l)) * fog * (0.2 + pow(max(0, dot(n, h)), 5.0) * 0.8) * diff;
      col += back * prod * (-n.y * 0.5 + 0.5) * vec3(0.5, 0.2, 0.1) * 0.3 * diff;
      prod *= 0.8 + fresnel * 0.8;
    }
    if(d > 100.0)
    {
      i = 100;
      break;
    }
    p += r * d;
  }
  

  vec3 n = norm(p);
  vec3 l = normalize(vec3(-1));
  float fog = 1.0 - i / 100.0;
  
  //col += max(dot(n, vec3(1.0)), 1.0);
  col += max(0.0, dot(n, l)) * fog;
  
  
  out_color = vec4(col, 1);
}
