uniform vec2 resolution;

void main( void ) {
    vec2 position = gl_FragCoord.xy/resolution;
    gl_FragColor = vec4(position,1,1);
}