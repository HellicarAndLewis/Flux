uniform vec3 pct;
uniform float scale;

varying vec4 vertex;

void main(void){
	vec2 st = gl_TexCoord[0].st;

	vec3 normalPos = vertex.xyz*pow(10.0,scale*-4.0);
	normalPos = normalPos.xzy;

	vec4 color = vec4(normalPos,1.0);

	vec3 sclPct = pct;

	sclPct -= vec3(0.5);
	sclPct *= 2.0;

	if (normalPos.x < sclPct.x){
		color.x = 0.0;	
	} 

	if (normalPos.y < sclPct.y){
		color.y = 0.0;	
	} 

	if (normalPos.z < sclPct.z){
		color.z = 0.0;	
	} 

	
	gl_FragColor = color;
}