#version 450

out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;       // For StoneWall.png
uniform sampler2D _NoiseTexture;  // For Noise.png
uniform float _Time; // Current time value

void main() {
    vec4 stoneColor = texture(_Texture, UV);

    // Calculate a dynamic scaling factor using sin. This will cause the texture to "pulse".
    float scale = 1.0 + 0.5 * sin(_Time); // This will scale between 0.5 and 1.5

    // Adjust UV coordinates to apply the scaling effect
    vec2 center = vec2(0.5, 0.5); // Assuming the center of scaling is the center of the texture
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
