uniform float pct;
uniform float tailWidth;
uniform float resolution;

uniform float time;

#define PI 3.14159

void main(void){
	vec2 p = (gl_FragCoord.xy / vec2(resolution))*2.0-1.0;
	
	float a = (atan(p.y,p.x) + (1.0-pct)*PI*2.0);
	float r = sqrt(dot(p,p));	
	vec2 uv = vec2(0.1/r,a/(PI));
	float len = dot(p,p);
	float brightness = pow(fract(uv.y / 2.0), tailWidth*180.0);

	gl_FragColor = vec4(vec3(1.0),brightness);
}