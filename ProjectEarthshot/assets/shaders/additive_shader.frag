#version 150

uniform sampler2D uTex0;
uniform float uIntensity;

in vec4		Color;
in vec2		TexCoord;

out vec4 	oColor;

void main( void )
{
	oColor = texture(uTex0, TexCoord.st) * uIntensity;
}
