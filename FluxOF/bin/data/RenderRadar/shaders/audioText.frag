uniform sampler2DRect textTex;
uniform sampler2DRect audioTex;

uniform float resolution;

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

	vec2 offset = texture2DRect(audioTex, vec2(st.x,0.5)).rg;

	float red = getColorChannel(st+offset,0);
	float green = getColorChannel(st+offset,1);
	float blue = getColorChannel(st+offset,2);

	vec4 color = vec4(red,green,blue, min(red,min(green,blue))) ;

	gl_FragColor = vecolor;
}