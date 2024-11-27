#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float plot(float a, float b)
{
    return smoothstep(a + 0.01, a, b) - smoothstep(a, a - 0.01, b);
}

void main() {
    vec2 currentPixel = gl_FragCoord.xy/u_resolution;
  
    /**
    * Type 1
    */
    float x = 1.0 - pow(abs(currentPixel.x), 0.5);
    // float x = 1.0 - pow(abs(currentPixel.x), 1.0);
    // float x = 1.0 - pow(abs(currentPixel.x), 1.5);
    // float x = 1.0 - pow(abs(currentPixel.x), 2.0);
    // float x = 1.0 - pow(abs(currentPixel.x), 2.5);
    // float x = 1.0 - pow(abs(currentPixel.x), 3.0);
    // float x = 1.0 - pow(abs(currentPixel.x), 3.5);
    
    /**
    * Type 2
    */
    // float x = pow(cos(PI * currentPixel.x / 2.0), 0.5);
    // float x = pow(cos(PI * currentPixel.x / 2.0), 1.0);
    // float x = pow(cos(PI * currentPixel.x / 2.0), 1.5);
    // float x = pow(cos(PI * currentPixel.x / 2.0), 2.0);
    // float x = pow(cos(PI * currentPixel.x / 2.0), 2.5);
    // float x = pow(cos(PI * currentPixel.x / 2.0), 3.0);
    // float x = pow(cos(PI * currentPixel.x / 2.0), 3.5);
    
    /**
    * Type 3
    */
    // float x = 1.0 - pow(abs(sin(PI * currentPixel.x / 2.0)), 0.5);
    // float x = 1.0 - pow(abs(sin(PI * currentPixel.x / 2.0)), 1.0);
    // float x = 1.0 - pow(abs(sin(PI * currentPixel.x / 2.0)), 1.5);
    // float x = 1.0 - pow(abs(sin(PI * currentPixel.x / 2.0)), 2.0);
    // float x = 1.0 - pow(abs(sin(PI * currentPixel.x / 2.0)), 2.5);
    // float x = 1.0 - pow(abs(sin(PI * currentPixel.x / 2.0)), 3.0);
    // float x = 1.0 - pow(abs(sin(PI * currentPixel.x / 2.0)), 3.5);
    
    /**
    * Type 4
    */
    // float x = pow(min(cos(PI * currentPixel.x / 2.0), 1.0 - abs(currentPixel.x) ), 0.5);
    // float x = pow(min(cos(PI * currentPixel.x / 2.0), 1.0 - abs(currentPixel.x) ), 1.0);
    // float x = pow(min(cos(PI * currentPixel.x / 2.0), 1.0 - abs(currentPixel.x) ), 1.5);
    // float x = pow(min(cos(PI * currentPixel.x / 2.0), 1.0 - abs(currentPixel.x) ), 2.0);
    // float x = pow(min(cos(PI * currentPixel.x / 2.0), 1.0 - abs(currentPixel.x) ), 2.5);
    // float x = pow(min(cos(PI * currentPixel.x / 2.0), 1.0 - abs(currentPixel.x) ), 3.0);
    // float x = pow(min(cos(PI * currentPixel.x / 2.0), 1.0 - abs(currentPixel.x) ), 3.5);
    
    /**
    * Type 5
    */
    // float x = 1.0 - pow(max(0.0, abs(currentPixel.x) * 2.0 - 1.0), 0.5);
    // float x = 1.0 - pow(max(0.0, abs(currentPixel.x) * 2.0 - 1.0), 1.0);
    // float x = 1.0 - pow(max(0.0, abs(currentPixel.x) * 2.0 - 1.0), 1.5);
    // float x = 1.0 - pow(max(0.0, abs(currentPixel.x) * 2.0 - 1.0), 2.0);
    // float x = 1.0 - pow(max(0.0, abs(currentPixel.x) * 2.0 - 1.0), 2.5);
    // float x = 1.0 - pow(max(0.0, abs(currentPixel.x) * 2.0 - 1.0), 3.0);
    // float x = 1.0 - pow(max(0.0, abs(currentPixel.x) * 2.0 - 1.0), 3.5);

    
    float y = currentPixel.y;

    vec3 gradient = vec3(x);
    
    float point = plot(x, y);
    
    gl_FragColor = vec4((1.0 - point) * gradient + point * vec3(1.0,0.0,0.0), 1.0);
}
