#version 430 core

// Per-instance data
layout(location=0)in vec2 a_start;// pixel coordinates
layout(location=1)in vec2 a_end;// pixel coordinates
layout(location=2)in float a_thickness;// pixels
layout(location=3)in vec4 a_color;// rgba

out vec2 v_start;
out vec2 v_end;
out float v_thickness;
out vec4 v_color;

void main(){
  // We render as a single point per-instance; vertex position is unused
  v_start=a_start;
  v_end=a_end;
  v_thickness=a_thickness;
  v_color=a_color;
  
  // A single point: place it at origin (will not be used by GS)
  gl_Position=vec4(0.);
}