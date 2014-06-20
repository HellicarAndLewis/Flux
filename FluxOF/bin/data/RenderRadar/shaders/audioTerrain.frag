uniform sampler2DRect heightMap;
uniform sampler2DRect audioFft;

uniform float audioFftSize;
uniform float scale;
uniform float pct;
uniform float offset;

uniform float time;

void main(){
	vec2 st = gl_TexCoord[0].st;
	float height = 1.0-texture2DRect(heightMap, st ).x;
	float freq = texture2DRect(audioFft,vec2(audioFftSize*offset+height * audioFftSize*scale ,0.5)).x;
	gl_FragColor = vec4(freq);
	gl_FragColor.a = freq*pct;
}