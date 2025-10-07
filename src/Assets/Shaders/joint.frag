#version 330 core

in vec2 v_UV;
in vec4 v_Color;
out vec4 FragColor;

void main(){
  float dist=length(v_UV);
  float alpha=smoothstep(1.,.95,dist);
  FragColor=vec4(v_Color.rgb,v_Color.a*alpha);
  if(FragColor.a<1.)discard;
}