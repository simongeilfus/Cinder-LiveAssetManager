uniform float time;
varying vec3  position;

void main() {
	vec3 color = ( 1.0 + cos( time + position * 0.025 ) ) * 0.5;
	color += vec3( 0.35 );
    gl_FragColor = vec4( color, 1.0 );
}