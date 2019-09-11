#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;

uniform vec2 center;

uniform float time;
uniform float active[3*128];


float wave(in float x, in float y) {
    float dx = aPos.x - x + center.x - mod(center.x, 0.05);
    float dy = aPos.y - y + center.y - mod(center.y, 4.0/45.0);
    float l = sqrt(dx*dx+dy*dy);
    float val = sin(10*(l+time/100))/(5*l+1.0);
    return val;
}

float bessel_drum(in float x, in float y) {
    float dx = aPos.x - x + center.x - mod(center.x, 0.05);
    float dy = aPos.y - y + center.y - mod(center.y, 4.0/45.0);
    float r = sqrt(dx*dx+dy*dy);
    return sin(10*(r-time/100))/(5*r*r+1) + cos(20*(r-time/100))/(5*r+1);
}

float point(in float x, in float y) {
    float dx = aPos.x - x + center.x - mod(center.x, 0.05);
    float dy = aPos.y - y + center.y - mod(center.y, 4.0/45.0);
    float r = sqrt(dx*dx+dy*dy);
    if (r > 0.1) {
        return 0.0;
    } else {
        float r2 = r*r;
        return 1.0 - 100.0*r2 + 50*r2*r2;
    }
}


void main() {
    
    float new_z = aPos.z;

    for (int i = 0; i < 3*128; i=i+3) {
        if (int(active[i]) == 0) {
            break;
        }
        
        if (int(active[i]) == 2) {
            new_z = new_z + wave(active[i+1], active[i+2]);
        } else if (int(active[i]) == 3) { 
            new_z = new_z + point(active[i+1], active[i+2]);
        }
    }
    
    if (new_z < 0.0) {
        new_z = 0;
    }

    gl_Position = vec4(aPos.x - mod(center.x, 0.05), 
                       aPos.y - mod(center.y, 4.0/45.0) + 0.2f*new_z, 
                       0.0f, 
                       1.0f);
    color = vec3(0.0, (new_z+1.0)/2.0, 0.0);
}


