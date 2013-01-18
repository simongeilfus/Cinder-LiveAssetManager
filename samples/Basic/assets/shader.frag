void main() {
    gl_FragColor = vec4( 1.0, 0.2 + gl_FragCoord.y * 0.005, 0.5 + gl_FragCoord.y * 0.001, 1.0 );
}