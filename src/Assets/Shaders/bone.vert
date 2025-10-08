#version 330 core

layout(location=0)in vec2 a_Corner;// [-1,+1] across thickness, [0,1] along bone
layout(location=1)in vec2 a_Start;// bone start
layout(location=2)in vec2 a_End;// bone end
layout(location=3)in float a_Thickness;// shaft thickness & joint radius
layout(location=4)in vec4 a_Color;// shaft color
layout(location=5)in vec4 a_SColor;// start joint color
layout(location=6)in vec4 a_EColor;// end joint color

out vec2 v_Local;// local space coords (x along bone, y across)
out float v_Length;// bone length
out float v_Radius;// radius = thickness / 2
out vec4 v_SColor;
out vec4 v_EColor;
out vec4 v_Color;

uniform mat4 u_ViewProjection;

void main(){
  // Flip Y for your coordinate system
  vec2 start=a_Start;
  vec2 end=a_End;
  start.y*=-1.;
  end.y*=-1.;
  
  // Increase the size of the bone so that the center of the joint snaps to the position coord.
  vec2 iDir=normalize(end-start);
  start-=(iDir*a_Thickness*.5);
  end+=(iDir*a_Thickness*.5);
  
  // Bone direction and perpendicular
  vec2 dir=end-start;
  float len=length(dir);
  vec2 ndir=(len>.0001)?dir/len:vec2(1.,0.);
  vec2 perp=vec2(-ndir.y,ndir.x);
  
  float halfThick=a_Thickness*.5;
  
  // position in world space
  vec2 pos=mix(start,end,a_Corner.x)+perp*(a_Corner.y*halfThick);
  gl_Position=u_ViewProjection*vec4(pos,0.,1.);
  
  // pass parameters to fragment shader
  v_Local=vec2(a_Corner.x*len,a_Corner.y*halfThick);
  v_Length=len;
  v_Radius=halfThick;
  v_SColor=a_SColor;
  v_EColor=a_EColor;
  v_Color=a_Color;
}
