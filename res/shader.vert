#version 120

attribute vec3 position;
attribute vec2 texCoord;


varying vec2 texCoord0;
varying float visibility;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_proj;

const float fog_density = 0.007;
const float gradient = 1.5;

void main()
{
	vec4 posToCam = m_view * m_model * vec4(position, 1.0);
	gl_Position = m_proj * posToCam;

	//
	//calculate distance to camera
	float distance = length(posToCam.xyz);
	//exponential formula
	visibility = clamp(exp(-pow((distance*fog_density), gradient)), 0.0, 1.0);
	texCoord0 = texCoord;
}