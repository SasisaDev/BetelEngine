#version 460 core

layout(location=0) out vec2 texcoords;

layout(std140, set = 0, binding = 0) uniform WorldData {
    mat4 Projection;
    mat4 View;
    vec2 Position;
} worldData;

void main() {
    vec2 vertices[3] = vec2[3](vec2(0,0), vec2(100,0), vec2(0, 100));
    vec2 texverts[3] = vec2[3](vec2(100,25), vec2(100+100,25), vec2(100, 25+100));
    gl_Position = worldData.View * worldData.Projection * vec4(vertices[gl_VertexIndex] + vec2(worldData.Position.x, worldData.Position.y),0,1); 
    texcoords = 0.5 * (worldData.View * worldData.Projection * vec4(texverts[gl_VertexIndex],0,1)).xy + vec2(0.5);
}