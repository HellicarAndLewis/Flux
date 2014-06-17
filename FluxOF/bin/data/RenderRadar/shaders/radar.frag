uniform float pct;
uniform float tailWidth;

uniform vec2 center;

uniform float time;

#define PI 3.14159

void main(void){
	vec2 st = gl_FragCoord.xy;
	vec2 p = st-center;
	
	float a = (atan(p.y,p.x) + (1.0-pct)*PI*2.0);
	float r = sqrt(dot(p,p));	
	vec2 uv = vec2(0.1/r,a/(PI));
	float len = dot(p,p);
	float brightness = pow(fract(uv.y / 2.0), tailWidth*64.0);

	gl_FragColor = vec4(vec3(1.0),brightness);
}