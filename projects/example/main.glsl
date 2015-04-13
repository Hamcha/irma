void main() {
    vec2 p = gl_FragCoord.xy/uResolution;
    vec2 ratio = uResolution.xy/uResolution.yy;

    vec3 ro = vec3(0,0,-3);
    vec3 rd = normalize(vec3((-1.+2.*p)*ratio,2.5));

    vec3 color = vec3(0);
    vec2 t = intersect(ro, rd);
    if (t.y > 0.) {
        vec3 amb = vec3(0.2)+vec3(0,.07,.15);
        vec3 col = vec3(0.6);
        vec3 pos = ro+rd*t.x;
        vec3 nor = normal(pos);
        vec3 lum = vec3(-1, .4, 0);
        float dif = dot(nor,lum);
        color = amb + col * dif;
    }

    gl_FragColor = vec4(color, 1);
}