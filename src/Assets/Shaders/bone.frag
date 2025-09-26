#version 430 core

in vec2 v_localUV;
in vec4 v_color;
out vec4 FragColor;

void main(){
  // Optionally add a smooth edge based on v_localUV.y for nicer look:
  float edge=smoothstep(.02,0.,abs(v_localUV.y-.5)-.5);// tweak if needed
  // Basic color:
  FragColor=v_color*edge;
  // If you don't want edge fade, use:
  // FragColor = v_color;
}
