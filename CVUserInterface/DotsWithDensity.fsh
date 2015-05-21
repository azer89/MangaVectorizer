
/**
 *
 * This shader is used to draw dots screentone on a triangle with density used for lighting effect
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#version 120

uniform vec4 whiteColor;
uniform vec4 blackColor;

uniform float cosO;
uniform float sinO;
uniform float gap;
uniform float width;

uniform float imgWidth;
uniform float imgHeight;

uniform float aaValue;

uniform float thicknessFactor;

uniform sampler2D my_texture;

float aastep(float threshold, float value);

void main(void)
{
	vec4 texColor = texture2D(my_texture, gl_TexCoord[0].xy);
	//gl_FragColor = texColor;

	float frequency = imgHeight / gap;
	vec2 xyCoord = gl_TexCoord[0].xy;

	xyCoord.x *= (imgWidth / imgHeight);

	vec2 st2 = mat2(cosO, -sinO, sinO, cosO) * xyCoord;		

	vec2 nearest = 2.0 * fract(frequency * st2) - 1.0;
	float dist = length(nearest);
    
	
	float radius = width / gap * texColor.x * thicknessFactor;

	vec3 fragcolor = mix(blackColor.xyz, whiteColor.xyz, aastep(radius, dist));
	//gl_FragColor = vec4(fragcolor, 1.0);

	float afwidth = 2.0 * frequency * max(length(dFdx(st2)), length(dFdy(st2)));
	float blend = smoothstep(0.5, 1.0, afwidth); 
	vec3 texcol = vec3(1.5 - texColor.x, 1.5 - texColor.y, 1.5 - texColor.z);
	vec3 final_color = mix(fragcolor, texcol, blend);
	gl_FragColor = vec4(final_color, 1.0);
}

float aastep(float threshold, float value) 
{
	float afwidth = aaValue * length(vec2(dFdx(value), dFdy(value)));
	return smoothstep(threshold-afwidth, threshold+afwidth, value);
}
