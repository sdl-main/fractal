#version 330 core

uniform sampler1D tex;
uniform vec2 center;
uniform float scale;
in vec2 cc;
out vec4 color;

void main() {
    vec2 z = vec2(-0.0, center.y);
    vec2 c = cc;
    bool JULIA = false;
    if (JULIA) {
       vec2 x = z;
       z = c;
       c = x;
    }
    int i;
    int num = 80;
    for (i = 0; i <= num; ++i) {
        z = vec2(z.x*z.x - z.y*z.y + c.x, 2.0*z.x*z.y + c.y);
        if (z.x*z.x + z.y*z.y >= 4.0) {
            break;
        }
    }
    color = texture(tex, (i == num ? 0.0 : float(i)) / float(num));
}