#version 140
out vec4 LFragment;
uniform vec2 uResolution;

void main() {
    vec2 p = gl_FragCoord.xy/uResolution;
    LFragment = vec4(p, 1, 1);
}