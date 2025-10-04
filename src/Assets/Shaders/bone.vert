#version 430 core

layout(location=0)in vec2 a_corner;// per-vertex: x in [0,1] along bone, y in {-1,1} across thickness
layout(location=1)in vec2 a_start;// per-instance
layout(location=2)in vec2 a_end;// per-instance
layout(location=3)in float a_thickness;// per-instance
layout(location=4)in vec4 a_color;// per-instance

out vec2 v_localUV;
out vec4 v_color;

uniform mat4 u_ViewProjection;

void main(){
  // compute bone direction and perpendicular
  vec2 dir=a_end-a_start;
  float len=length(dir);
  vec2 ndir=(len>.0001)?(dir/len):vec2(1.,0.);
  vec2 perp=vec2(-ndir.y,ndir.x);
  
  // position along bone (0=start, 1=end)
  float t=a_corner.x;
  
  // offset across thickness; a_corner.y is -1 or +1
  float halfThick=a_thickness*.5;
  vec2 pos=mix(a_start,a_end,t)+perp*(a_corner.y*halfThick);
  
  gl_Position=u_ViewProjection*vec4(pos,0.,1.);
  
  // Provide a simple uv: x=t along bone, y across (-1..1 -> 0..1)
  v_localUV=vec2(t,a_corner.y*.5+.5);
  v_color=a_color;
}
