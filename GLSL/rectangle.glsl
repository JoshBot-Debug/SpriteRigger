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
    
    /**
    * Specify the minimum and maximum size of the rectangle.
    */
    vec2 rect = vec2(.3,.2);

    /**
    * Get the current point
    * subract the currentPixel from the center
    * Example
	* 0.8 - 0.5 = 0.3 	// We are in the red space
    * 0.2 - 0.5 = -0.3  // We are in the red space on the other side.
    * 
    * Use abs() because we don't care about the sign
    */
    vec2 point = abs(currentPixel - center);
    
    // Set the color of our pixel
    vec3 color;
    
    /**
    * Check if the x & y point is less than the min
    */
    // if(point.x < rect.x && point.y < rect.y)
    //     color.r = 1.0;
    
    /**
    * Use inbuild funtions to shorten the compairson
    * lessThan() will return a boolean vec
    * Also see:
	* lessThanEqual(), greaterThan(), greaterThanEqual(), equal(), notEqual()
    *
    * all() checks if all the all components of the boolean vec is true
    * Also see:
	* any(), not()
    */
    if(all(lessThan(point, rect)))
        color.r = 1.0;
    
    gl_FragColor = vec4(color,1.0);
}
