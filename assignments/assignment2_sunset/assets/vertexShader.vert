#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;  // new UV coordinate

out vec2 UV;  // to be passed to fragment shader

void main() {
	UV = vUV;  // pass to fragment shader
	gl_Position = vec4(vPos, 1.0);
}
