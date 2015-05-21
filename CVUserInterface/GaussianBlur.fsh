
/**
 *
 * Blur done in 2 passes
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#version 120


uniform sampler2D my_texture;

uniform float resolutionx;
uniform float resolutiony;
uniform float radius;
uniform float dirx;
uniform float diry;

void main(void)
{
	vec2 dir = vec2(dirx, diry);
    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 tc = gl_TexCoord[0].xy;
    //float blur = radius / resolution; 

	float hstep = dir.x * radius / resolutionx;
    float vstep = dir.y * radius / resolutiony;
	
	vec4 texColor = texture2D(my_texture, tc);

	sum += texture2D(my_texture, vec2(tc.x - 4.0 * hstep, tc.y - 4.0 * vstep )) * 0.0162162162;
    sum += texture2D(my_texture, vec2(tc.x - 3.0 * hstep, tc.y - 3.0 * vstep )) * 0.0540540541;
    sum += texture2D(my_texture, vec2(tc.x - 2.0 * hstep, tc.y - 2.0 * vstep )) * 0.1216216216;
    sum += texture2D(my_texture, vec2(tc.x - 1.0 * hstep, tc.y - 1.0 * vstep )) * 0.1945945946;

    sum += texture2D(my_texture, vec2(tc.x, tc.y)) * 0.2270270270;

    sum += texture2D(my_texture, vec2(tc.x + 1.0 * hstep, tc.y + 1.0 * vstep )) * 0.1945945946;
    sum += texture2D(my_texture, vec2(tc.x + 2.0 * hstep, tc.y + 2.0 * vstep )) * 0.1216216216;
    sum += texture2D(my_texture, vec2(tc.x + 3.0 * hstep, tc.y + 3.0 * vstep )) * 0.0540540541;
    sum += texture2D(my_texture, vec2(tc.x + 4.0 * hstep, tc.y + 4.0 * vstep )) * 0.0162162162;

	gl_FragColor = sum;

}