uniform sampler2DRect textTex;
uniform sampler2DRect audioTex;

uniform float resolution;
uniform float audioTexSize;

float map(float _value, float _minIn, float _maxIn, float _minOut, float _maxOut){
	return ((_value - _minIn) / (_maxIn - _minIn) * (_maxOut - _minOut) + _minOut);
}

float getColorChannel(vec2 _pos, int _channel){
	if( _pos.x>resolution || _pos.x < 0.0 ){
		return 0.0;
	} else {
		vec4 color = texture2DRect(textTex, _pos);
		return color[_channel] * color.a;
	}
}

void main(){
	vec2 st = gl_TexCoord[0].st;

	

	float pct = st.y/resolution;
	float offset = texture2DRect(audioTex, vec2(pct*audioTexSize*0.5,0.5) ).x;

	vec4 color = texture2DRect(textTex,vec2(st.x+offset*resolution,st.y));

	gl_FragColor = color;
}