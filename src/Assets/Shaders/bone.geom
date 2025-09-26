#version 430 core

layout(points)in;
layout(triangle_strip,max_vertices=4)out;

in vec2 v_start[];
in vec2 v_end[];
in float v_thickness[];
in vec4 v_color[];

out vec2 g_uv;// optional: for texturing / rounded ends
out vec4 g_color;

uniform vec2 u_screenSize;// (width, height) in pixels
uniform mat4 u_view;// optional if your world coords need transform
uniform mat4 u_proj;// optional

// helper: convert pixel space (0..width, 0..height) to NDC (-1..1).
vec2 pixelToNDC(vec2 p){
  float y=u_screenSize.y-p.y;// flip Y
  return vec2((p.x/u_screenSize.x)*2.-1.,(y/u_screenSize.y)*2.-1.);
}

void emitVertexNDC(vec2 pos_ndc,vec2 uv,vec4 color){
  g_uv=uv;
  g_color=color;
  gl_Position=vec4(pos_ndc,0.,1.);
  EmitVertex();
}

void main(){
  vec2 s=v_start[0];
  vec2 e=v_end[0];
  float thickness=v_thickness[0];
  vec4 color=v_color[0];
  
  // convert to NDC
  vec2 s_ndc=pixelToNDC(s);
  vec2 e_ndc=pixelToNDC(e);
  
  // direction in NDC space
  vec2 dir=normalize(e_ndc-s_ndc);
  // perpendicular in NDC (not normalized to keep aspect)
  vec2 perp=vec2(-dir.y,dir.x);
  
  // thickness in NDC: (thickness_pixels / screen) * 1.0
  // half-thickness offset in NDC:
  vec2 halfOffset=perp*thickness*vec2(1./u_screenSize.x,1./u_screenSize.y);
  
  // Four corners of the quad (triangle strip)
  // We choose uv so fragment shader can do rounded cap if desired
  // Order: v0 = start - half, v1 = start + half, v2 = end - half, v3 = end + half
  emitVertexNDC(s_ndc-halfOffset,vec2(0.,0.),color);
  emitVertexNDC(s_ndc+halfOffset,vec2(0.,1.),color);
  emitVertexNDC(e_ndc-halfOffset,vec2(1.,0.),color);
  emitVertexNDC(e_ndc+halfOffset,vec2(1.,1.),color);
  
  EndPrimitive();
}
