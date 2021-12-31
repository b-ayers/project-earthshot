#version 150

uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

uniform float uTime;

in vec4		ciPosition;
in vec2		ciTexCoord0;
in vec3		ciNormal;
in vec4		ciColor;

out highp vec2	TexCoord;
out lowp vec4	Color;

void main( void )
{
	gl_Position	= ciModelViewProjection * ciPosition * sin(uTime * 100);
	Color 		= ciColor;
	TexCoord	= ciTexCoord0;

}
