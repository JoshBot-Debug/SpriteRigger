#version 330 core

in vec2 v_Position;   // x = along (-1..+1), y = across (-1..+1)
in float v_Length;
in float v_Radius;
in vec4 v_Color;
in vec4 v_SColor;
in vec4 v_EColor;
out vec4 FragColor;

void main() {
  float ax = v_Position.x; // along the bone (0 → len)
  float ay = abs(v_Position.y); // across (thickness)

  // float dist = (ax < v_Radius)
  //     ? length(vec2(ax - v_Radius, ay)) - v_Radius
  //     : (ax > v_Length - v_Radius)
  //         ? length(vec2(ax - (v_Length - v_Radius), ay)) - v_Radius
  //         : ay - v_Radius;

  float dist = (ax < v_Radius)
      ? length(vec2(ax - v_Radius, ay - v_Radius)) - v_Radius
      : 1.;

  if (dist > 0.0)
      discard;


  FragColor = v_Color;
  // float t = clamp((v_Local.x * 0.5) + 0.5, 0.0, 1.0);
  // vec4 tipColor = mix(v_SColor, v_EColor, t);

  // float capStart = 1.0 - smoothstep(0.0, 0.2, t);
  // float capEnd   = smoothstep(0.8, 1.0, t);
  // float capMask  = max(capStart, capEnd);

  // FragColor = mix(v_Color, tipColor, capMask);
  FragColor = v_Color;
}
