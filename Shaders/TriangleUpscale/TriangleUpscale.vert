#version 450 core

layout(location=0) out vec2 texcoords;

layout(std140, set = 0, binding = 1) uniform UpscaleData {
    vec2 AspectRationCompensation;
} upscaleData;

void main() {
    vec2 vertices[3] = vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1, 3));
    gl_Position = vec4(vertices[gl_VertexIndex] * upscaleData.AspectRationCompensation,0,1);
    texcoords = 0.5 * vertices[gl_VertexIndex] + vec2(0.5);
}