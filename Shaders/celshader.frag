varying vec3 normal;
uniform sampler2D Texture1;
	
void main(void)
{
	vec4 pix;
	vec4 out_color;
	float intensity;
	vec4 color;
	vec3 n = normalize(normal);

	intensity = dot(vec3(gl_LightSource[0].position),n);
	if (intensity > 0.95)
		color = vec4(1.0,1.0,1.0,1.0);
	else if (intensity > 0.7)
		color = vec4(0.7,0.7,0.7,1.0);
	else if (intensity > 0.5)
		color = vec4(0.5,0.5,0.5,1.0);
	else
		color = vec4(0.35,0.35,0.35,1.0);
	pix = texture2D(Texture1,gl_TexCoord[0].xy);
	out_color = color * pix;
	gl_FragColor = out_color;
}

