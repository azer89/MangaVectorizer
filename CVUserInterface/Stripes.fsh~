

/**
 *
 * This shader is used to draw stripes screentone on a triangle
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

float aastep(float threshold, float value);

void main(void)
{
	// Distance to nearest point in a grid of
	// (frequency x frequency) points over the unit square
	float frequency = imgHeight /  gap;
	
	vec2 st2 = mat2(cosO, -sinO, sinO, cosO) * gl_TexCoord[0].xy;
	float sty = st2.y;

	float nearest = 2.0*fract(frequency * sty) - 1.0;
	float dist = length(nearest);
    
	float radius = width / gap;

	vec3 fragcolor = mix(blackColor.xyz, whiteColor.xyz, aastep(radius, dist));
	//gl_FragColor = vec4(fragcolor, 1.0);

	float afwidth = 2.0 * frequency * max(length(dFdx(sty)), length(dFdy(sty)));
	float blend = smoothstep(0.5, 1.0, afwidth); 
	vec3 texcolor = vec3(0.75, 0.75, 0.75);
	vec3 final_color = mix(fragcolor, texcolor, blend);
	gl_FragColor = vec4(final_color, 1.0);
}

float aastep(float threshold, float value) 
{
	float afwidth = aaValue * length(vec2(dFdx(value), dFdy(value)));
	return smoothstep(threshold-afwidth, threshold+afwidth, value);
}

