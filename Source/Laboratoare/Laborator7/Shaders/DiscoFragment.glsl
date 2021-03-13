precision highp float;

uniform float time;
uniform float scale;
uniform float speed;

varying vec3 vNormal;
varying vec2 vUv;

float rand(in vec2 p)
{
	return abs( fract( sin(p.x * 95325.328 + p.y * -48674.077) + cos(p.x * -46738.322 + p.y * 76485.077) + time * speed ) -.5)+.5;
}
	
void main( void ) {
	
	vec2 position = ( vUv.xy ) * scale;

	vec3 color = vec3(rand( vec2(floor(position.x), floor(position.y) ) ), rand( vec2(floor(position.x) , floor(position.x) ) ) , rand( vec2(floor(position.x*.5) , floor(position.y*.5) ) ));
	float scale = 1.-pow( pow( (mod( position.x, 1.)-.5), 2.) + pow( (mod( position.y, 1.)-.5), 2.), .7 );
	
	gl_FragColor = vec4( color*scale, 1.);
}