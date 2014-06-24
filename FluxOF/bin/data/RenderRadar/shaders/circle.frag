uniform vec3 color;

uniform vec2 center;
uniform vec2 resolution;

uniform float radius;
uniform float borderSize;
uniform float borderBlur;
uniform float alpha;

uniform float noiseZoom;

uniform float time;

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
	vec2 uv = (2.0*gl_FragCoord.xy-2.0*center)/min(resolution.y,resolution.x);

	float zoom = noiseZoom*1000.0;
	float r = length( uv );
	float gap = borderSize*0.1;
	vec3 col = vec3(0.0);

	// border of watch
	if(zoom>0.0){
		col = mix( col, color*perlin3(vec3(uv,time*0.1)*zoom), 1.0-smoothstep(gap,gap+borderBlur,abs(r-radius*2.0)) );
	}else {
		col = mix( col, color, 1.0-smoothstep(gap,gap+borderBlur,abs(r-radius*2.0)) );

	}
	
	gl_FragColor = vec4(col,min(max(col.r,max(col.g,col.b)),alpha) );
}