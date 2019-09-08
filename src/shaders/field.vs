#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = view * vec4(aPos, 1.0f);
    color = vec3(0.0, aPos.z, 0.0);
}
