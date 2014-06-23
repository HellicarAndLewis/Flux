uniform sampler2DRect audioFft;

uniform float audioFftSize;
uniform float scale;
uniform float pct;
uniform float offset;

uniform float time;

void main(){
	vec2 st = gl_TexCoord[0].st;
	//float freq = texture2DRect(audioFft,vec2(audioFftSize*offset * audioFftSize*scale ,0.5)).x;
    
    float freq = texture2DRect(audioFft,vec2(st.x* scale + offset*audioFftSize ,0.5)).x;
    
    if(1.-st.y/100. > freq){
        gl_FragColor.r = 0.;
    }
    else {
        gl_FragColor.r = 1.;
    }
 //   gl_FragColor.g = st.y/100.;
//	gl_FragColor = vec4(freq);
//	gl_FragColor.a = freq*pct;
}