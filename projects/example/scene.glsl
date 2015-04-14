vec2 scene(in vec3 p) {
    vec2 scene = vec2(MAX,0);

    p.xy = rotate(p.xy, p.z/50+uTime/10);
    vec3 rp = repeat(p, vec3(20, 2, 2));
    vec3 rq = repeat(p, vec3(20, 0.4, 0.4));
    float sph = sphere(rp, abs(sin(uTime + p.z / 5)*2));
    float sp2 = sphere(rq, 0.1);

    scene = add(scene, vec2(sph, 1));
    scene = add(scene, vec2(sp2, 2));

    return scene;
}