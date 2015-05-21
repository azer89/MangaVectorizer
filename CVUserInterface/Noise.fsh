
/**
 * Simple noise shader
 * 
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#version 120

float rand(vec2 co);


void main(void)
{
	vec2 xyCoord = gl_TexCoord[0].xy;	
	float randval = rand(xyCoord);
	gl_FragColor = vec4(randval, randval, randval, 1.0);
}


float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
