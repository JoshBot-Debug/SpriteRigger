#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

void main() {

    vec2 currentPixel = gl_FragCoord.xy/u_resolution;
    
	vec2 center = vec2(.5,.5);
    float radius = 0.3;
    
    float dist = length(currentPixel - center);
    
    // Here we set the red color,
    // because we use smoothstep, this is applying
    // what is basically anti-aliasing
    float smooth = smoothstep(radius, radius - 0.01, dist);

    vec3 color;
    
    if(dist < radius)
        color.x = 1.0;
    
    gl_FragColor = vec4(color - (1.0 - smooth),1.0);
    
    // Because we are just coloring the circle red
    // We could just do this instead
    // float red = smoothstep(radius, radius - 0.01, dist);
    // gl_FragColor = vec4(red,.0,.0,1.0);
}
