uniform sampler2DRect backbuffer;

uniform sampler2DRect maskTex;
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

	float radar = texture2DRect(radarTex,st).r;
	float depth = texture2DRect(depthMap,st).r;
    float mask = texture2DRect(maskTex,st).r;
    vec4 bg = texture2DRect(backbuffer, st);
    
    gl_FragColor = bg;

    if( mask>0.0 ){
        //  Geometry
        //
        vec2 n = texture2DRect(normalMap,st).rg;
        
        vec3 color;
        color = mix(dstColor1,dstColor2,n.x);
        color = mix(color,dstColor3,n.y);


        gl_FragColor = vec4(mix(bg.rgb,color,radar),1.0);
    } else if(radar>(1.0-radarThreshold) ){
        //  TINT
        //
        vec3 colors[5];
        colors[0] = dstColor1;
        colors[1] = dstColor2;
        colors[2] = dstColor3;
        colors[3] = dstColor4;
        colors[4] = dstColor5;

        float index = depth*3.0;

        float fIndex = floor(index);
        float mIndex = mod(index,fIndex);

    	vec3 tint = mix(colors[int(index)],colors[int(index)+1],mIndex);
    	gl_FragColor = vec4(mix(bg.rgb,tint,radar),1.0);
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
        		sources += 1.0;
            	color += texture2DRect(backbuffer, st + offset[i]);
        	}
    	}
    
    	color = color / sources;                  
    
    	gl_FragColor = mix(color,bg,inc);
	}


    gl_FragColor.a = 1.0;
}