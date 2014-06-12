uniform vec3 pct;
uniform float scale;

varying vec4 vertex;

void main(void){
	vec2 st = gl_TexCoord[0].st;

	vec3 normalPos = vertex.xyz*pow(10.0,scale*-4.0);
	vec4 color = vec4(normalPos,1.0);

	vec3 transPos = pct;
	transPos -= vec3(0.5);
	transPos *= 2.0;

	if (normalPos.x < transPos.x){
		color.x = 0.0;	
	} 

	if (normalPos.y < transPos.y){
		color.y = 0.0;	
	} 

	if (normalPos.z < transPos.z){
		color.z = 0.0;	
	} 

	
	gl_FragColor = color;
}