#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform vec2 center;


void main() {
    gl_Position = vec4(aPos.x - mod(center.x, 0.05), 
                       aPos.y - mod(center.y, 4.0/45.0), 
                       0.0f,
                       1.0f);
    color = vec3(0.0, 0.0, 0.0);
}


