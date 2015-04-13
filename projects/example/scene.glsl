vec2 scene(in vec3 p) {
    vec2 scene = vec2(MAX,0);

    // Add objects here
    add(scene, vec2(sphere(p, 0.5), 1));

    return scene;
}