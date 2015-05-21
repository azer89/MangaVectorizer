
/**
 *
 * This shader is used to draw grid screentone on a triangle with density used for lighting effect
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
	vec2 texcoord;
	texcoord.x = 1.0 - gl_TexCoord[0].x;
	texcoord.y = 1.0 - gl_TexCoord[0].y;

	vec4 texColor = texture2D(my_texture, texcoord);

	//vec4 texColor = texture2D(my_texture, gl_TexCoord[0].xy);

	float frequency = imgHeight / gap;
	vec2 xyCoord = gl_TexCoord[0].xy;

	xyCoord.x *= (imgWidth / imgHeight);

	vec2 st2 = mat2(cosO, -sinO, sinO, cosO) * xyCoord;		

	float nearestx = 2.0 * fract(frequency * st2.x) - 1.0;
	float nearesty = 2.0 * fract(frequency * st2.y) - 1.0;
	float dist = max(length(nearestx), length(nearesty));
    
	float radius = width / gap * texColor.x * (thicknessFactor) * 1.5;

	//vec3 fragcolor = mix(blackColor.xyz, whiteColor.xyz, aastep(radius, dist));
	vec3 fragcolor = mix(whiteColor.xyz, blackColor.xyz, aastep(radius, dist));

	float afwidth = 2.0 * frequency * max(length(dFdx(st2)), length(dFdy(st2)));
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
