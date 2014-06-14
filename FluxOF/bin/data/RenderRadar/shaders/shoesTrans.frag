uniform vec3 pct;
uniform float lineWidth;

uniform vec3 tranColor;
uniform float tranNoiseZoom;
uniform float tranWidth;

uniform float scale;

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

	vec3 pos = vPos.xyz*pow(10.0,scale*-4.0);

	vec3 A = vec3(1.0,pos.y*2.0,0.0);
	vec3 B = vec3(0.0,pos.y*2.0,1.0);

	vec3 transPos = pct;
	transPos -= vec3(0.5);
	transPos *= 2.0;
	
	float threshold = transPos.z;
	vec3 green = tranColor * vec3(perlin3(vPos.xyz*vec3(sin(time*0.01),abs(cos(time*0.015)),cos(time*0.017))*(tranNoiseZoom*100.0)) );

	//	RADAR TRAIL
	//
	vec4 color = vec4(A,1.0);
	if (pos.y + tranWidth*0.5 < threshold){
		color.xyz = B;
	} else if( pos.y < threshold ){
		float posPct = ((pos.y+tranWidth*0.5) - threshold)/tranWidth;
		color.xyz = mix(B,green,posPct);
	}

	//	RADAR LINE
	//
	if( pos.y - lineWidth*0.5 < threshold && !(pos.y + lineWidth*0.5 < threshold) ){
		float posPct = 1.0-((pos.y+lineWidth*0.5) - threshold)/lineWidth;
		// color.xyz += green*sin(posPct*PI);
		color.xyz = mix(color.xyz,green*2.0,pow(max(0.0, abs(sin(posPct*PI))*2.0-1.0),2.5));
	}

	gl_FragColor = color;
}