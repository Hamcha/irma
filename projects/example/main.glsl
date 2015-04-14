vec3 hsb(float h, float s, float b) {
    if (s <= 0) return vec3(b);
    if (h >= 1) h = 0;
    h *= 6;
    int i = int(floor(h));
    float f = h - i;
    float p = b * (1 - s);
    float q = b * (1 - s * f);
    float t = b * (1 - s * (1 - f));
    switch (i) {
    case 0:
        return vec3(b, t, p);
    case 1:
        return vec3(q, b, p);
    case 2:
        return vec3(p, b, t);
    case 3:
        return vec3(p, q, b);
    case 4:
        return vec3(t, p, b);
    case 5:
    default:
        return vec3(b, p, q);
    }
}

void main() {
    vec2 p = gl_FragCoord.xy/uResolution;
    vec2 ratio = uResolution.xy/uResolution.yy;
    vec2 mouse = uMouseCoord/uResolution;

    vec3 ro = vec3(0,0,uTime*10);
    vec3 rd = normalize(vec3((-1.+2.*p)*ratio,2.5));

    rd.yz = rotate(rd.yz, -0.5+mouse.y);
    rd.xz = rotate(rd.xz, 0.5-mouse.x);

    vec3 color = vec3(0);
    vec2 t = intersect(ro, rd);
    if (t.y > 0.) {
		vec3 pos = ro + rd*t.x;
		vec3 amb = hsb(mod(pos.z/10, 1.0), 0.5, 0.2) + vec3(0, .07, .15);
		vec3 col = hsb(mod(pos.z/10, 1.0), 1, 0.5);
        vec3 lum = vec3(0.5, 0.5, -2);
        color = amb + col * softshadow(pos, lum, 5) * 50;
    }

    gl_FragColor = vec4(color, 1);
}