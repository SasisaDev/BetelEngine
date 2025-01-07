#version 450 core

layout(location = 0) in vec2 texcoords;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {    
    outColor = texture(texSampler, texcoords);
    if (outColor.w < 0.05) { discard; }
}