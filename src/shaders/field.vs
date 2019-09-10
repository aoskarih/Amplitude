#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform float time;


float wave() {
    float l = sqrt(aPos.x*aPos.x+aPos.y*aPos.y);
    float val = sin(10*(l+time/100))/(l+1.0);
    return val;
}

void main() {
    float new_z = aPos.z + wave();
    gl_Position = vec4(aPos.x, aPos.y+0.2f*new_z, 0.0f, 1.0f);
    color = vec3(0.0, (new_z+1.0)/2.0, 0.0);
}


