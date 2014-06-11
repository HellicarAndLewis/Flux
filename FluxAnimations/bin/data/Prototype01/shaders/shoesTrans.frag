uniform vec3 pct;
uniform float lineWidth;
uniform float xRayWidth;

uniform float scale;
uniform float specularExpo;

varying vec3 norm;
varying vec4 vPos;
varying vec3 ePos;

float PI = 3.14159265359;

void main(void){
	

	vec2 uv = gl_TexCoord[0].st;

	float sExpo = specularExpo * 64.0;
	float fr = abs(dot( -normalize(ePos), normalize( norm ) ) );
	float xray = pow(1. - fr, max( .51, sExpo * .1));
	
	vec3 pos = vPos.xyz*pow(10.0,scale*-4.0);
	pos = pos.xzy;

	vec3 A = vec3(1.0,pos.z*2.0,0.0);
	vec3 B = vec3(0.0,pos.z*2.0,1.0);

	vec3 transPos = pct;
	transPos -= vec3(0.5);
	transPos *= 2.0;

	vec4 color = vec4(A,1.0);
	if (pos.x + lineWidth*0.5 < transPos.x){
		color.xyz = B;
	} else if( pos.x - lineWidth*0.5 < transPos.x ){
		float posPct = 1.0-((pos.x+lineWidth*0.5) - transPos.x)/lineWidth;
		color.xyz = mix(A,B,posPct);
	}

	if (pos.x + xRayWidth*0.5 < transPos.x){
		color.xyz = B;
	} else if( pos.x - xRayWidth*0.5 < transPos.x ){
		float posPct = 1.0-((pos.x+xRayWidth*0.5) - transPos.x)/xRayWidth;

		vec4 xRayColor = vec4(color.xyz * xray,fr);

		color = mix(color,xRayColor,sin(posPct*PI));
	}	
	
	

	gl_FragColor = color;
}