#version 330 core

in vec2 v_Local;// (x along bone, y across)
in float v_Length;
in float v_Radius;
in vec4 v_SColor;
in vec4 v_EColor;
in vec4 v_Color;

out vec4 FragColor;

void main(){
  float x=v_Local.x;
  float y=v_Local.y;
  
  float edgeWidth=v_Radius*.125;
  
  // Default base color and alpha
  vec4 color=v_Color;
  vec4 startColor=v_SColor;
  vec4 endColor=v_EColor;
  float alpha=1.;
  
  // --- Left joint ---
  if(x<v_Radius*2.){
    float d=length(vec2(x-v_Radius,y));
    alpha=1.-smoothstep(v_Radius-edgeWidth,v_Radius,d);
    color=startColor;
  }
  
  // --- Right joint ---
  else if(x>v_Length-(v_Radius*2.)){
    float d=length(vec2(x-(v_Length-v_Radius),y));
    alpha=1.-smoothstep(v_Radius-edgeWidth,v_Radius,d);
    color=endColor;
  }
  
  // --- Shaft ---
  else{
    // --- Left Shaft cap ---
    if((x-v_Radius*2.)<v_Radius){
      float d=length(vec2((x-v_Radius*2.)-v_Radius,y));
      alpha=1.-smoothstep(v_Radius-edgeWidth,v_Radius,d);
    }
    
    // --- Right Shaft cap ---
    else if((x+v_Radius*2.)>v_Length-v_Radius){
      float d=length(vec2((x+v_Radius*2.)-(v_Length-v_Radius),y));
      alpha=1.-smoothstep(v_Radius-edgeWidth,v_Radius,d);
    }
    
    // --- Shaft ---
    else{
      float d=abs(y);
      alpha=1.-smoothstep(v_Radius-edgeWidth,v_Radius,d);
    }
  }
  
  if(alpha<=0.)discard;
  
  FragColor=vec4(color.rgb,color.a*alpha);
}
