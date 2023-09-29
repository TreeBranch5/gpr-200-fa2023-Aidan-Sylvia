#version 450

out vec4 FragColor;
in vec2 UV;

// Uniforms that you'll have to set from your C++ code
uniform vec2 iResolution;
uniform float iTime;
uniform float radius;
uniform vec4 skyColor;
uniform vec4 groundColor;
uniform vec4 sunColor;
uniform float sunSpeed;

void main() {



    vec2 aspectRatio = vec2(iResolution.x / iResolution.y, 1.0);
  
    vec2 uv = UV * 2.0 - 1.0;
  
    uv *= aspectRatio;

    float x = 0.0;
    float y = sin(iTime*sunSpeed) * 1.2 - 1.0;
    vec2 center = vec2(x, y);
  
    float dist = length(uv - center);


    vec4 topColor = vec4(0.0, 0.0, 0.0, 1.0); // Black

    if (dist < radius) {
        FragColor = sunColor; // White Circle
    } else {
        float t = uv.y * 0.5 - sin(iTime*sunSpeed) + 0.3;
        FragColor = mix(skyColor, topColor, t);
    }
    
    uv.x *= iResolution.x / iResolution.y;

    float sineWave1 = 0.2 * sin(2.0 * uv.x - iTime) - 0.8;
    float sineWave2 = 0.1 * sin(4.0 * uv.x + iTime) - 0.5;

    vec4 waveColor1 = vec4(0.0, 0.0, 1.0, 1.0);  // Blue
    vec4 waveColor2 = vec4(1.0, 1.0, 1.0, 1.0);  // Red
    

    if (abs(uv.y - sineWave1) < 0.05) {
        FragColor = waveColor1;
    }
    else if (abs(uv.y - sineWave2) < 0.05) {
        FragColor = waveColor2;
    }
    
}
