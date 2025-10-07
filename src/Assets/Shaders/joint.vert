#version 330 core

layout(location=0)in vec2 a_Corner;
layout(location=1)in vec2 a_Position;
layout(location=2)in float a_BoneThickness;
layout(location=3)in vec4 a_Color;

out vec2 v_UV;
out vec4 v_Color;

uniform mat4 u_ViewProjection;

void main(){
  // Flip coords
  vec2 inPos=a_Position;
  inPos.y*=-1.;
  
  float radius=a_BoneThickness;
  vec2 pos=inPos+a_Corner*radius;
  gl_Position=u_ViewProjection*vec4(pos,0.,1.);
  v_UV=a_Corner;
  v_Color=a_Color;
}