uniform sampler2DRect backbuffer;
uniform sampler2DRect depthMap;
uniform sampler2DRect normalMap;
uniform sampler2DRect radarTex;

uniform vec3 dstColor1;
uniform vec3 dstColor2;
uniform vec3 dstColor3;
uniform vec3 dstColor4;
uniform vec3 dstColor5;

uniform float time;
uniform float resolution;
uniform float radarThreshold;

#define PI 3.14159265358979323846

void main(void){
	vec2 st = gl_FragCoord.xy;
	// vec2 st = gl_TexCoord[0].st;

	float radar = texture2DRect(radarTex,st).r;
	float depth = texture2DRect(depthMap,st).r;

    vec4 bg = texture2DRect(backbuffer, st);
    
    if(radar>(1.0-radarThreshold) ){
    	vec4 tint = vec4(sin(time*0.2)*0.5+0.5,0.0,abs(cos(time*0.1)),depth);
    	gl_FragColor = mix(bg,tint,depth);
	} else {

		//	DISPERSE PIGMENTS
		//
		vec2 norm = ( texture2DRect(normalMap, st).rg - 0.5 ) * 2.0;
    	float inc = ( abs(norm.x) + abs(norm.y) ) * 0.5;

		vec2 offset[36];
    	int iTotal = 36;
    	float fTotal = 36.0;
                                       
    	float step = (PI*2.0)/fTotal;
    	float angle = 0.0;
    	for (int i = 0; i < iTotal; i++) {
        	offset[i].x = cos(angle);
        	offset[i].y = sin(angle);
        	angle += step;
    	}
                                       
    	float sources = 1.0;
    	vec4 color = bg;
    	for (int i = 0; i < iTotal; i++){
        	vec4 goingTo = ( texture2DRect( normalMap, st + offset[i] ) - 0.5 ) * 2.0;
        	if ( dot(goingTo.rg,offset[i]) < -1.0/fTotal){
        		float pixHeight = texture2DRect(depthMap, st + offset[i]).r;

        		// if(	pixHeight > depth)
        		{
        			sources += 1.0;
            		color += texture2DRect(backbuffer, st + offset[i]);
        		}
        	}
    	}
    
    	color = color / sources;                  
    
    	gl_FragColor = mix(color,bg,inc);
	}


    gl_FragColor.a = 1.0;
}