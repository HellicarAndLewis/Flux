uniform vec2 laserPosition;
uniform vec3 laserColor;

uniform float lineWidth;

uniform float time;

varying vec3 norm;
varying vec4 vPos;
varying vec3 ePos;

float PI = 3.14159265359;

float noise3(vec3 co){
  return fract(sin(dot(co ,vec3(12.9898,78.233,125.198))) * 43758.5453);
}

float smooth(float v) {
	return 3.*pow(v,2.)-2.*pow(v,3.);
}

float perlin3(vec3 p) {
	float val = 0.;
	for(float i=0.;i<3.;i += 1.){
		p *= pow(2.,i);
		vec3 c = floor(p);
		float u = smooth(fract(p.x));
		float v = smooth(fract(p.y));
		val = 1.-((1.-val)*(1.-pow(.5,i)*
			mix(mix(mix(noise3(c),noise3(c+vec3(1.,0.,0.)),u),
					mix(noise3(c+vec3(0.,1.,0.)),noise3(c+vec3(1.,1.,0)),u),v),
			    mix(mix(noise3(c+vec3(0.,0.,1.)),noise3(c+vec3(1.,0.,1.)),u),
					mix(noise3(c+vec3(0.,1.,1.)),noise3(c+vec3(1.,1.,1.)),u),v),fract(p.z))));
	}
	return val;
}

vec2 XRay(float _specularExpo){
	float sExpo = _specularExpo * 64.0;
	float fr = abs(dot( -normalize(ePos), normalize( norm ) ) );
	float xray = pow(1. - fr, max( .51, sExpo * .1));
	return vec2(xray, fr);
}

void main(void){

	vec2 uv = gl_TexCoord[0].st;

	vec2 pos = vPos.xz;
	pos.x *= -1.0;

	vec3 A = vec3(1.0,1.0,0.0);
	vec3 B = vec3(0.0,0.0,1.0);
	
	vec3 color = B;
	if( laserPosition.y > pos.x && laserPosition.x > pos.y){
		color = A;
	} else if( laserPosition.y > pos.x){
		color = vec3(0.0,1.0,0.0);
	} else if ( laserPosition.x > pos.y){
		color = vec3(1.0,0.0,0.0);
	}

	float threshold = laserPosition.x;
	if( pos.y - lineWidth*5.0 < threshold && !(pos.y + lineWidth*5.0 < threshold) ){
		float posPct = 1.0-((pos.y+lineWidth*5.0) - threshold)/(lineWidth*10.0);
		float noise = perlin3(vPos.xyz*vec3(time*0.1));
		vec3 green = mix(color,laserColor,noise);
		color = mix(color.xyz,green*2.0,pow(max(0.0, abs(sin(posPct*PI))*2.0-1.0),0.5));
	}

	threshold = laserPosition.y;
	if( pos.x - lineWidth*5.0 < threshold && !(pos.x + lineWidth*5.0 < threshold) ){
		float posPct = 1.0-((pos.x+lineWidth*5.0) - threshold)/(lineWidth*10.0);
		float noise = perlin3(vPos.xyz*vec3(time*0.1));
		vec3 green = mix(color,laserColor,noise);
		color.xyz = mix(color.xyz,green*2.0,pow(max(0.0, abs(sin(posPct*PI))*2.0-1.0),0.5));
	}

	gl_FragColor.rgb = color;
	gl_FragColor.a = 1.0;
}