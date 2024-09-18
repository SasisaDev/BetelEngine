#version 450 core

layout(location=0) out vec2 texcoords;

void main() {
    vec2 vertices[3] = vec2[3](vec2(-0.5,-0.5), vec2(0.5,-0.5), vec2(-0.35, 0.5));
    gl_Position = vec4(vertices[gl_VertexIndex],0,1); 
    texcoords = 0.5 * gl_Position.xy + vec2(0.5);
}