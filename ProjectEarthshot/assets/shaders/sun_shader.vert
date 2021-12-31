#version 150

uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

in vec4		ciPosition;
in vec2		ciTexCoord0;
in vec3		ciNormal;
in vec4		ciColor;

out highp vec2	TexCoord;
out lowp vec4	Color;
out highp vec3	Normal;
out highp vec3 fragVert;

void main( void )
{
	gl_Position	= ciModelViewProjection * ciPosition;
	Color 		= ciColor;
	TexCoord	= ciTexCoord0;
	Normal		= ciNormal;

	fragVert = vec3(ciPosition);

}
