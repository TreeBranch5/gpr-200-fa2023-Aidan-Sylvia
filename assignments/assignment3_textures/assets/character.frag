#version 450

out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;
uniform float _Time;

void main(){
    vec4 texColor = texture(_Texture, UV);

    // Oscillate the alpha using a sine wave
    float oscillatingAlpha = sin(_Time) * 0.5 + 0.5; // this will oscillate between 0 and 1

    FragColor = vec4(texColor.rgb, texColor.a * oscillatingAlpha);
}
