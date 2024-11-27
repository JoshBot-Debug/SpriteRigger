#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

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

    /**
    * Select a point on the screen as our center.
    * Because we normalized our currentPixel, the center is at 0.5,0.5
    */
    vec2 center = vec2(.5,.5);
    
    // Size of our circle
    float radius = 0.1;
    
    /**
    * Calculate using length
    * 
    * Length is an inbuilt function.
    * It takes in two vectors and returns
    */
    // float point = length(currentPixel - center);
    
    /**
    * Calculate using distance
    * 
    * Length is an inbuilt function.
    * It takes in two vectors and returns
    */
    float point = distance(currentPixel, center);
    
    // Set the color of our pixel
    vec3 color;
    
    if(point < radius)
        color.r = 1.0;
    
    gl_FragColor = vec4(color,1.0);
}
