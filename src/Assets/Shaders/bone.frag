#version 430 core

in vec2 g_uv;
in vec4 g_color;
out vec4 fragColor;

void main(){
  fragColor=g_color;
}