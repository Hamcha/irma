#define MAX_STEPS 2500

vec2 intersect(in vec3 ro, in vec3 rd) {
    float t = 0.;
    for (int i = 0; i < MAX_STEPS; i++) {
        vec2 h = scene(ro+rd*t);
        if (h.x < .001) return vec2(t,h.y);
        t += h.x;
    }
    return vec2(0);
}

vec3 normal(in vec3 p) {
    vec3 e = vec3(.001,0,0);
    vec3 n = vec3(scene(p+e.xyy).x - scene(p-e.xyy).x,
                  scene(p+e.yxy).x - scene(p-e.yxy).x,
                  scene(p+e.yyx).x - scene(p-e.yyx).x);
    return normalize(n);
}