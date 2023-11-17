#version 450

out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;      
uniform sampler2D _NoiseTexture; 
uniform float _Time; 

void main() {
    vec4 stoneColor = texture(_Texture, UV);
    
    float scale = 1.0 + 0.5 * sin(_Time); 

    vec2 center = vec2(0.5, 0.5); 
    vec2 scaledUV = center + (UV - center) * scale;

    float uOffset = 0.1 * sin(_Time);
    float vOffset = 0.1 * cos(_Time);

    vec2 oscillatingUV = UV + vec2(uOffset, vOffset);
    vec4 noiseColor = texture(_NoiseTexture, oscillatingUV);

     vec2 distortedUV = UV + (noiseColor.r - 0.5) * 0.005;
    stoneColor = texture(_Texture, distortedUV);
    noiseColor = noiseColor + texture(_NoiseTexture, scaledUV);
    noiseColor = noiseColor * texture(_NoiseTexture, distortedUV);

    FragColor = mix(stoneColor, noiseColor, 0.2);
}
