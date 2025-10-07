#version 330 core

layout(location=0)in vec2 a_Corner;// per-vertex: x in [0,1] along bone, y in {-1,1} across thickness
layout(location=1)in vec2 a_Start;
layout(location=2)in vec2 a_End;
layout(location=3)in float a_Thickness;
layout(location=4)in vec4 a_Color;

out vec2 v_UV;
out vec4 v_Color;

uniform mat4 u_ViewProjection;

void main(){
  // compute bone direction and perpendicular
  vec2 dir=a_End-a_Start;
  float len=length(dir);
  vec2 ndir=(len>.0001)?(dir/len):vec2(1.,0.);
  vec2 perp=vec2(-ndir.y,ndir.x);
  
  // position along bone (0=start, 1=end)
  float t=a_Corner.x;
  
  // offset across thickness; a_Corner.y is -1 or +1
  float halfThick=a_Thickness*.5;
  vec2 pos=mix(a_Start,a_End,t)+perp*(a_Corner.y*halfThick);
  
  gl_Position=u_ViewProjection*vec4(pos,0.,1.);
  v_Color=a_Color;
  v_UV=vec2(t,a_Corner.y*.5+.5)*2.-1.;
}
