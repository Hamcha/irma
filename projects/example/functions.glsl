vec2 rotate(vec2 k, float t)
{
    return vec2(cos(t)*k.x-sin(t)*k.y,sin(t)*k.x+cos(t)*k.y);
}

vec2 add(in vec2 a, in vec2 b) {
    return a.x < b.x ? a : b;
}

float displace(in float a, in float b) {
    return a + b;
}

vec3 repeat(in vec3 p, in vec3 dist) {
    return mod(p, dist) - 0.5*dist;
}

float sphere(in vec3 p, in float t) {
    return length(p) - t;
}