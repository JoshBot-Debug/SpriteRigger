#version 330 core

layout(location=0)in vec2 mesh;
layout(location=1)in vec3 color;
layout(location=2)in vec3 translate;

out vec3 fragColor;

uniform mat4 mvp;

void main(){
  fragColor=color;
  
  vec2 translation=translate.xy;
  // float angle=radians(translate.z);
  float angle=radians(60);
  
  mat2 rotation=mat2(cos(angle),-sin(angle),sin(angle),cos(angle));
  
  gl_Position=mvp*vec4(rotation*mesh+translation,0.,1.);
}