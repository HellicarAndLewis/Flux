uniform float radarHeight;

uniform float lineWidth;

uniform vec3 radarColor;

uniform float tranNoiseZoom;

uniform float time;

varying vec4 ambientGlobal, eyeSpaceVertexPos;
varying vec4 vertexPos;
varying vec3 vertexNormal;

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

void main(void){
  //  Variables
  //  
	vec2 uv = gl_TexCoord[0].st;
  vec3 pos = vertexPos.xyz;

  vec3 n = normalize(vertexNormal);

	vec4 color = vec4(vec3(0.0),1.0);

	//	RADAR LINE
	//
	if( pos.y - lineWidth*5.0 < radarHeight && !(pos.y + lineWidth*5.0 < radarHeight) ){
		float posPct = 1.0-((pos.y+lineWidth*5.0) - radarHeight)/(lineWidth*10.0);
		float noise = perlin3(vertexPos.xyz*vec3(time*0.1)*tranNoiseZoom);
		vec4 green = mix(color,vec4(radarColor,1.0),noise);
		color = mix(color,green*2.0,pow(max(0.0, abs(sin(posPct*PI))*2.0-1.0),0.5));
	}

	gl_FragColor = color;
}