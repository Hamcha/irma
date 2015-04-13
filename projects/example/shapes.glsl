void add(inout vec2 a, in vec2 b) {
    a = a.x < b.x ? a : b;
}

float sphere(in vec3 p, in float t) {
    return length(p) - t;
}