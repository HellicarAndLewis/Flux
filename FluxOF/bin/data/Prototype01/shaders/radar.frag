uniform float pct;
uniform float tailWidth;

uniform float time;

#define PI 3.14159

void main(void){
	vec2 resolution = vec2(800.0);

	vec2 p = (gl_FragCoord.xy / resolution.xy)*2.0-1.0;
	float a = (atan(p.y,p.x) + (pct)*PI*2.0);
	float r = sqrt(dot(p,p));	
	vec2 uv = vec2(0.1/r,a/(PI));
	float len = dot(p,p);
	float brightness = pow(fract(uv.y / -2.0), tailWidth*180.0);

	gl_FragColor = vec4(vec3(1.0),brightness);
}