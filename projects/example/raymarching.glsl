#define MAX_STEPS 200

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

float softshadow(in vec3 ro, in vec3 rd, float k) {
    float res = 1.;
    float t = 0.1;
    for (int i = 0; i < 16; i++) {
        float h = scene(ro + rd*t).x;
        if (h < .001) return 0.;
        res = min(res, k*h / t);
        t += h;
    }
    return res;
}