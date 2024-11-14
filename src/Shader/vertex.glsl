#version 330 core

layout(location=0)in vec2 position;
layout(location=1)in vec3 color;
layout(location=2)in vec3 transform;

out vec3 fragColor;

void main(){
  fragColor=color;
  
  vec2 translation=transform.xy;
  // float angle=radians(transform.z);
  float angle=radians(0);
  
  mat2 rotation=mat2(cos(angle),-sin(angle),sin(angle),cos(angle));
  
  gl_Position=vec4(rotation*position+translation,0.,1.);
}