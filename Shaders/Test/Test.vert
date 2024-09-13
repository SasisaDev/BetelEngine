#version 460 core

layout(location=0) out vec2 texcoords;

layout(std140, set = 0, binding = 0) uniform WorldData {
    mat4 Projection;
    mat4 View;
} worldData;

void main() {
    vec2 vertices[3] = vec2[3](vec2(20,35), vec2(174,100), vec2(75, 120));
    gl_Position = worldData.View * worldData.Projection * vec4(vertices[gl_VertexIndex],0,1); 
    texcoords = 0.5 * gl_Position.xy + vec2(0.5);
}