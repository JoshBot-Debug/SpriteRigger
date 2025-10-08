#version 330 core

layout(location=0)in vec2 a_Corner;   // [-1,+1] across, [0,1] along
layout(location=1)in vec2 a_Start;
layout(location=2)in vec2 a_End;
layout(location=3)in float a_Thickness;
layout(location=4)in vec4 a_Color;
layout(location=5)in vec4 a_SColor;
layout(location=6)in vec4 a_EColor;

out vec2 v_Position;    // (x = along-bone position, y = across-bone offset)
out float v_Radius;
out float v_Length;
out vec4 v_SColor;
out vec4 v_EColor;
out vec4 v_Color;

uniform mat4 u_ViewProjection;

void main() {
  // Flip coords for your coordinate system
  vec2 inStart = a_Start;
  vec2 inEnd = a_End;
  inStart.y *= -1.0;
  inEnd.y *= -1.0;

  // Bone direction and perpendicular
  vec2 dir = inEnd - inStart;
  float len = length(dir);
  vec2 ndir = (len > 0.0001) ? (dir / len) : vec2(1.0, 0.0);
  vec2 perp = vec2(-ndir.y, ndir.x);

  // Offset across bone
  float halfThick = a_Thickness * 0.5;
  vec2 pos = mix(inStart, inEnd, a_Corner.x) + perp * (a_Corner.y * halfThick);

  gl_Position = u_ViewProjection * vec4(pos, 0.0, 1.0);

  // Pass local coordinates (in units of half thickness)
  v_Position = vec2(a_Corner.x * len, a_Corner.y * halfThick); 
  v_Length = len;
  v_Radius = a_Thickness;
  v_Color = a_Color;
  v_SColor = a_SColor;
  v_EColor = a_EColor;
}
