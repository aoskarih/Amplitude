
#version 330 core

out vec4 FragColor;
in vec4 color;
in float depth_value;

void main() {
    FragColor = vec4(color.x, color.y, color.z, 0.2);
}

