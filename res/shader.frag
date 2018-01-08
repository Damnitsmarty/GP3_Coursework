#version 120

varying vec2 texCoord0;
varying float visibility;

uniform sampler2D diffuse;

const vec4 c_sky = vec4(0.3,0.3,0.5,1);

void main()
{
	gl_FragColor = mix(vec4(c_sky), texture2D(diffuse, texCoord0), visibility);
}
