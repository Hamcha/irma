uniform vec2 uResolution;
uniform ivec2 uMouseCoord;
uniform float uTime;

void main() {
    vec2 p = gl_FragCoord.xy/uResolution;
    vec2 m = vec2(uMouseCoord.x, uResolution.y - uMouseCoord.y);

    vec3 color = vec3(p, 1);
    if (distance(m,gl_FragCoord.xy) < 10f+sin(uTime*10)*5f) {
        color = vec3(0);
    }

    gl_FragColor = vec4(color, 1);
}