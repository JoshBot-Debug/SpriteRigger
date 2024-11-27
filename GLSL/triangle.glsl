#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float cross(vec2 a, vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

void main() {
    /**
    * We take the current pixel gl_FragCoord.xy and divide it by the resolution
    * By doing this we get the current pixel in the range of 0.0 - 1.0
    * Example:
    * Bottom left pixel
    * gl_FragCoord.xy = .0,.0;
    * u_resolution = 640.0,480.0;
    * currentPixel = .0,.0;
    *
    * Top right pixel
    * gl_FragCoord.xy = 640.0,480.0;
    * u_resolution = 640.0,480.0;
    * currentPixel = 1.0,1.0;
    */
    vec2 currentPixel = gl_FragCoord.xy/u_resolution;
    
    vec2 p0 = vec2(0.5, 0.7);  // Top vertex
    vec2 p1 = vec2(0.2, 0.3);  // Bottom left
    vec2 p2 = vec2(0.8, 0.3);  // Bottom right
    
    float area1 = cross(p1 - p0, currentPixel - p0);
    float area2 = cross(p2 - p1, currentPixel - p1);
    float area3 = cross(p0 - p2, currentPixel - p2);
    
    vec3 color;
    
    if (area1 >= 0.0 && area2 >= 0.0 && area3 >= 0.0) 
        color.x = 1.0;
    

    gl_FragColor = vec4(color,1.0);
}
