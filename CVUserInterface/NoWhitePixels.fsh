
/**
 * Cleaning white artifact when rendering bezier curve
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#version 120

uniform sampler2D my_texture;

void main(void)
{
	vec4 texColor = texture2D(my_texture, gl_TexCoord[0].xy);
	//gl_FragColor = texColor;

	if(texColor.a < 0.5)
	{
		gl_FragColor = vec4(texColor.xyz, 0.0);
	}
	else if(texColor.x > 0.5 && texColor.y > 0.5 && texColor.z > 0.5)
	{
		gl_FragColor = vec4(texColor.xyz, 0.0);
	}
	else
	{
		gl_FragColor = vec4(texColor.xyz, 1.0);
	}
}