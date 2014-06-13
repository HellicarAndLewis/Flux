uniform sampler2DRect backbuffer;
uniform sampler2DRect radarTex;
uniform sampler2DRect depthMap;

uniform vec3 dstColor1;
uniform vec3 dstColor2;
uniform vec3 dstColor3;
uniform vec3 dstColor4;
uniform vec3 dstColor5;

uniform float time;

void main(void){
	vec4 bg = texture2DRect(backbuffer,gl_FragCoord.xy);
	float radar = texture2DRect(radarTex,gl_FragCoord.xy).r;

	vec3 color = vec3(sin(time*0.2)*0.5+0.5,0.0,abs(cos(time*0.1)));

	gl_FragColor = vec4(mix(bg.xyz,color,radar),1.0);
}