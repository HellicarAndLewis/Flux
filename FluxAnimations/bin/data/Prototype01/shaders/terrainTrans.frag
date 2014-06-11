uniform float pct;
uniform float scale;

varying vec4 vertex;

void main(void){
	vec2 st = gl_TexCoord[0].st;
	gl_FragColor = vec4(vertex.xyz*pow(10.0,scale*-4.0),1.0);
}