uniform sampler2DRect backbuffer;
uniform sampler2DRect depthMap;

uniform vec3 dstColor1;
uniform vec3 dstColor2;
uniform vec3 dstColor3;
uniform vec3 dstColor4;
uniform vec3 dstColor5;

uniform float pct;
uniform float tailWidth;

uniform float time;

#define PI 3.14159
#define D2R(_d) ((_d)*PI/180.0)

void main(void){
	vec4 bg = texture2DRect(backbuffer,gl_FragCoord.xy);
	// vec4 color = vec4(vec3(0.0),1.0);

	vec2 resolution = vec2(500.0);

	vec2 p = (gl_FragCoord.xy / resolution.xy)*2.0-1.0;
	float a = (atan(p.y,p.x) + (pct)*PI*2.0);
	float r = sqrt(dot(p,p));	
	vec2 uv = vec2(0.1/r,a/(PI));
	float len = dot(p,p);
	float brightness = pow(fract(uv.y / -2.0), tailWidth*180.0);

	gl_FragColor = vec4(dstColor1,brightness);
}