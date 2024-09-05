#version 450 core

layout(location = 0) in vec2 texcoords;
layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {    
    outColor = texture(texSampler, texcoords);
    //outColor = vec4(0.5, 0.5, 0, 1); 
}