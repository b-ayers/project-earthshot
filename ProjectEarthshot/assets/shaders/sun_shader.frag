#version 150

uniform sampler2D uTex0;

uniform mat4 ciModelMatrix;
uniform mat3 ciNormalMatrix;

in vec4		Color;
in vec3		Normal;
in vec2		TexCoord;
in vec3   fragVert;

uniform vec4 uEmissiveColor;
uniform vec3 uSunlightPosition;

out vec4 	oColor;

void main( void )
{
	//calculate normal in world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(ciModelMatrix)));
	vec3 normal = normalize( Normal * normalMatrix );

	//calculate the location of this fragment (pixel) in world coordinates
  vec3 fragPosition = vec3(ciModelMatrix * vec4(fragVert, 1));



	//calculate the vector from this pixels surface to the light source
	vec3 surfaceToLight = uSunlightPosition - fragPosition;

	//calculate the cosine of the angle of incidence
	float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
	brightness = clamp(brightness, 0, 1);

	//calculate final color of the pixel, based on:
	// 1. The angle of incidence: brightness
	// 2. The color/intensities of the light: light.intensities
	// 3. The texture and texture coord: texture(tex, fragTexCoord)
	vec4 surfaceColor = texture(uTex0, TexCoord.st) * Color + uEmissiveColor;
	oColor = surfaceColor;
	//oColor = vec4(brightness * light.intensities * surfaceColor.rgb, surfaceColor.a);


	//float diffuse = max( dot( normal, vec3( 0, 0, 1 ) ), 0 );
	//oColor = texture( uTex0, TexCoord.st ) * Color * diffuse + uEmissiveColor;
}
