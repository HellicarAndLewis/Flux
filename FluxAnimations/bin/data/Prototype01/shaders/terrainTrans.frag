uniform float pct;
uniform float scale;

varying vec4 vertex;

void main(void){
	vec2 st = gl_TexCoord[0].st;

	vec3 normalPos = vertex.xyz*pow(10.0,scale*-4.0);

	if (normalPos.x < pct){
		gl_FragColor = vec4(normalPos,1.0);	
	} else if (normalPos.x < pct ) {
		gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	} else {
		gl_FragColor = vec4(1.0);	
	}

	
}