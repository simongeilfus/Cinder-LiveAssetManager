uniform float time;
varying vec3 position;

void main() {
	vec4 pos = gl_Vertex;
	pos.x = pos.x + sin( pos.x + time ) * 25.0;
	pos.y = pos.y + cos( pos.y + time ) * 25.0;

	position 	= ( gl_ModelViewMatrix * pos ).xyz;
    gl_Position = gl_ModelViewProjectionMatrix * pos;
}