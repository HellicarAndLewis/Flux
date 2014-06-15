uniform vec2 pct;

void main(void){
	vec2 st = gl_FragCoord.xy;

	vec3 color = vec3(0.0);

	if( pct.x > st.x && pct.y > st.y){
		color.xy = vec2(1.0);
	} else if( pct.x > st.x){
		color.x = 1.0;
	} else if (pct.y > st.y){
		color.y = 1.0;
	}

	gl_FragColor = vec4(color,1.0);
}