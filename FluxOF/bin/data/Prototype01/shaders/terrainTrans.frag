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

	gl_FragColor = vec4(dstColor1,radar);
}