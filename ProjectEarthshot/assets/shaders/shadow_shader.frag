#version 150 core

//
// NOTE:  Portions of this code taken from https://www.tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
// as well as the shadow mapping sample within Cinder samples
//

uniform sampler2D uTex0;

uniform vec3 uLightPos;
uniform sampler2DShadow uShadowMap;

uniform vec4 uEmissiveColor;
uniform vec3 uSunlightPosition;

uniform mat4 ciModelMatrix;
uniform mat3 ciNormalMatrix;

in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;
in vec4 vShadowCoord;
in vec2	vTexCoord;

out vec4 Color;

void main( void )
{
	//vec3 Normal			= normalize( vNormal );
	//vec3 LightVec		= normalize( vec3(0.0, 0.0, 0.0) - vPosition.xyz );
	//float NdotL			= max( dot( vNormal, LightVec ), 0.0 );


	//calculate normal in world coordinates
    mat3 normalMatrix = transpose(inverse(mat3(ciModelMatrix)));
    vec3 normal = normalize(normalMatrix * vNormal);
    
    //calculate the location of this fragment (pixel) in world coordinates
    vec3 fragPosition = vec3(ciModelMatrix * vPosition);
    
    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = uSunlightPosition - fragPosition;

	//calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);
	
	vec4 Diffuse		=  vec4(brightness);
	vec4 Ambient		= vec4( 0.3, 0.3, 0.3, 1.0 );
	
	vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
	float Shadow		= 1.0;
	
	if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) {
		Shadow = textureProj( uShadowMap, ShadowCoord, -0.00005 );
	}

	//Color.rgb = ( Diffuse + Ambient ) * vColor.rgb;
	//Color.a	= 1.0;

	Color = texture( uTex0, vTexCoord.st) * Diffuse * Shadow + uEmissiveColor;

}