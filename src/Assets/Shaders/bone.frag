#version 330 core

in vec2 v_Local;
in vec4 v_Color;
in vec4 v_SColor;
in vec4 v_EColor;
out vec4 FragColor;

void main()
{
    // Local bone coordinates
    float ax = abs(v_Local.x);
    float ay = abs(v_Local.y);

    // Capsule parameters (in normalized local space)
    float radius = 1.0;
    float shaftHalf = 0.5;

    // Signed distance from capsule boundary
    float dist = (ax > shaftHalf)
        ? length(vec2(ax - shaftHalf, ay)) - radius
        : ay - radius;

    // Discard anything outside the capsule
    if (dist > 0.0)
        discard;

    // Compute position along the bone in normalized [0,1] range
    float t = clamp((v_Local.x / (shaftHalf * 2.0)) + 0.5, 0.0, 1.0);

    // Emphasize joint color near the spherical caps
    // The closer to start or end (t near 0 or 1), the more it blends to s/e color
    float edgeBlend = smoothstep(0.0, 0.15, t) * (1.0 - smoothstep(0.85, 1.0, t));

    // Combine start/end colors across the bone length
    vec4 color = mix(v_SColor, v_EColor, t);

    // Make caps more strongly colored
    float capMaskStart = smoothstep(0.0, 0.25, 0.25 - t); // strong near start
    float capMaskEnd   = smoothstep(0.0, 0.25, t - 0.75); // strong near end

    // Combine masks
    float capMask = max(capMaskStart, capMaskEnd);

    // Mix base shaft color with cap colors
    color = mix(v_Color, color, capMask);

    FragColor = color;
}
