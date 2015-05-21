
/**
 *
 * Don't use this shader.
 * I tried to render cubic bezier with anti-aliasing as described in the paper
 * but still no luck, currently I use FXAA
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#version 120


uniform vec4 insideColor;
uniform vec4 outsideColor;

void main(void)
{
	vec3 uv = gl_TexCoord[0].xyz;
	float val = pow(uv.x, 3) - uv.y * uv.z;
	
	vec3 px = dFdx(uv);  
	vec3 py = dFdy(uv); 

	float fx = (3 * uv.x * uv.x) * px.x - (uv.y * px.y) - (uv.z * px.z);  
	float fy = (3 * uv.x * uv.x) * py.x - (uv.y * py.y) - (uv.z * py.z);

	float sd = val / sqrt(fx * fx + fy * fy); 

	float alpha = 0.5 - sd;  

	//vec4 mycol =  vec4(insideColor.xyz, 1.0);
	vec4 mycol =  vec4(1.0, 0.0, 0.0, 1.0);

	if (alpha > 1)  
	{
		 //mycol = vec4(outsideColor.xyz, 1.0);	
		 mycol =vec4(0.0, 1.0, 0.0, 1.0);	 
	}
	else if (alpha < 0) 
	{
		//mycol =  vec4(insideColor.xyz, 1.0);
		mycol =  vec4(1.0, 0.0, 0.0, 1.0);
	}
	else // Near boundary  
	{
		//mycol =  vec4(1.0 - alpha, 1.0 - alpha, 1.0 - alpha, 1.0);
		mycol =  vec4(0.0, 0.0, 1.0, 1.0);; 
	}

	gl_FragColor = mycol;
}