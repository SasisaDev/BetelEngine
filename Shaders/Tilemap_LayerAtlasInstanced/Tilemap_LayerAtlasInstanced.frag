#version 450

layout(location = 0) in vec2 texcoords;

layout(set = 0, binding = 2) uniform sampler2D atlasTexture;

layout(location = 0) out vec4 outColor;

void main() {    
    outColor = texture(atlasTexture, texcoords);
    if (outColor.w < 0.05) { discard; }
}