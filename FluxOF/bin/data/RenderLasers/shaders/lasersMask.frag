uniform vec2 pct;
uniform float resolution;
uniform float time;

void main(void){
	vec2 st = gl_FragCoord.xy;
	vec2 pos = pct*vec2(resolution);

	vec3 color = vec3(0.0);

	if( pos.x > st.x && pos.y > st.y){
		color.xy = vec2(1.0);
	} else if( pos.x > st.x){
		color.x = 1.0;
	} else if (pos.y > st.y){
		color.y = 1.0;
	}

	gl_FragColor = vec4(color,1.0);
}