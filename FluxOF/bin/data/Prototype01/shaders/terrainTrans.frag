uniform sampler2DRect backbuffer;
uniform sampler2DRect depthMap;

uniform vec3 dstColor1;
uniform vec3 dstColor2;
uniform vec3 dstColor3;
uniform vec3 dstColor4;
uniform vec3 dstColor5;

uniform float pct;
uniform float lineWidth;

uniform float time;

#define PI 3.14159
#define D2R(_d) ((_d)*PI/180.0)

float draw_pointer(vec2 rel_pos,float angle,float pt_len){
	float r=length(rel_pos);
	vec2 pt_axis=vec2(cos(angle),sin(angle));
	vec2 pt_perp_axis=vec2(-sin(angle),cos(angle));
	
	if(dot(rel_pos,pt_axis)>0.){
		float d=abs(dot(rel_pos,pt_perp_axis));
		return smoothstep(pt_len+2.,pt_len,r)*
			smoothstep(lineWidth+1.,lineWidth*0.1,d);
	}
	return 0.;
}

void main(void){

	vec4 bg = texture2DRect(backbuffer,gl_FragCoord.xy);
	vec4 color = vec4(vec3(0.0),1.0);

	vec2 resolution = vec2(500.0);
	vec2 center=gl_FragCoord.xy-vec2(resolution)*0.5;

	float scale = 0.0;
	if((scale=draw_pointer(center,pct*PI*2.0,resolution.x))>0.){
		color += vec4(dstColor1,1.0)*scale;
	} 

	gl_FragColor= color;
}