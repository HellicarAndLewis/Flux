uniform sampler2DRect colorMaskTexture;
uniform sampler2DRect srcTexture;
uniform sampler2DRect dstTexture;

uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;

uniform vec2 laserPosition;
uniform vec3 laserColor;

uniform float lineWidth;
uniform float time;

varying vec4 ambientGlobal, eyeSpaceVertexPos;
varying vec4 vertexPos;
varying vec3 vertexNormal;

int lightsNumber = 2;
float PI = 3.14159265359;

vec4 directional_light(in int lightIndex, in vec3 normal) {
  vec3 lightDir;
  vec4 dirLightColor, diffuse, specular, ambient = vec4(0.0);;
  float intensity;

  lightDir = normalize(gl_LightSource[lightIndex].position.xyz); // that's already in eye space
  ambient = gl_FrontMaterial.ambient * gl_LightSource[lightIndex].ambient;
  /* The ambient term of a directional light will always be present */
  dirLightColor = ambient;
  /* compute light intensity
   * (the dot product between normal and light dir)
   */
  intensity = max(dot(normal, lightDir), 0.0);
  if (intensity > 0.0) {
       vec3 reflection;
       vec3 eyeSpaceVertexPos_n = normalize(vec3(eyeSpaceVertexPos));
       vec3 eyeVector = normalize(-eyeSpaceVertexPos_n); // in eye space, eye is at (0,0,0)

       diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightIndex].diffuse;
       dirLightColor += diffuse * intensity;
       // compute Phong specular component
       reflection = normalize((2.0 * dot(lightDir, normal) * normal) - lightDir);
       specular = pow(max(dot(reflection, eyeVector), 0.0), gl_FrontMaterial.shininess) *
	 gl_FrontMaterial.specular * gl_LightSource[lightIndex].specular;
       dirLightColor += specular;
  }
  return dirLightColor;
}

vec4 point_light(in int lightIndex, in vec3 normal) {
  vec3 lightDir;
  vec4 pointLightColor, diffuse, specular, ambient = vec4(0.0);
  float intensity, dist;

  pointLightColor = vec4(0.0);
  // Compute the light direction
  lightDir = vec3(gl_LightSource[lightIndex].position - eyeSpaceVertexPos);
  /* compute the distance to the light source */
  dist = length(lightDir);
  intensity = max(dot(normal, normalize(lightDir)), 0.0);
  if (intensity > 0.0) {
    vec3 reflection;
    vec3 eyeSpaceVertexPos_n = normalize(vec3(eyeSpaceVertexPos));
    vec3 eyeVector = normalize(-eyeSpaceVertexPos_n);
    float att, dist;

    dist = length(lightDir);
    att = 1.0 / (gl_LightSource[lightIndex].constantAttenuation +
		 gl_LightSource[lightIndex].linearAttenuation * dist +
		 gl_LightSource[lightIndex].quadraticAttenuation * dist * dist);
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightIndex].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[lightIndex].ambient;
    pointLightColor += att * (diffuse * intensity + ambient);
    // compute Phong specular component
    reflection = normalize((2.0 * dot(lightDir, normal) * normal) - lightDir);
    specular = pow(max(dot(reflection, eyeVector), 0.0), gl_FrontMaterial.shininess) *
      gl_FrontMaterial.specular *
      gl_LightSource[lightIndex].specular;
    pointLightColor += att * specular;
  }
  return pointLightColor;
}

vec4 spot_light(in int lightIndex, in vec3 normal) {
  vec3 lightDir;
  vec4 spotLightColor, diffuse, specular, ambient = vec4(0.0);
  float intensity, dist;

  spotLightColor = vec4(0.0);
  // Compute the light direction
  lightDir = vec3(gl_LightSource[lightIndex].position - eyeSpaceVertexPos);
  /* compute the distance to the light source */
  dist = length(lightDir);
  intensity = max(dot(normal, normalize(lightDir)), 0.0);
  if (intensity > 0.0) {
    float spotEffect;

    spotEffect = dot(normalize(gl_LightSource[lightIndex].spotDirection), normalize(-lightDir));
    if (spotEffect > gl_LightSource[lightIndex].spotCosCutoff) {
      vec3 reflection;
      vec3 eyeSpaceVertexPos_n = normalize(vec3(eyeSpaceVertexPos));
      vec3 eyeVector = normalize(-eyeSpaceVertexPos_n);
      float att, dist;

      dist = length(lightDir);
      att = spotEffect / (gl_LightSource[lightIndex].constantAttenuation +
			  gl_LightSource[lightIndex].linearAttenuation * dist +
			  gl_LightSource[lightIndex].quadraticAttenuation * dist * dist);
      diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightIndex].diffuse;
      ambient = gl_FrontMaterial.ambient * gl_LightSource[lightIndex].ambient;
      spotLightColor += att * (diffuse * intensity + ambient);
      // compute Phong specular component
      reflection = normalize((2.0 * dot(lightDir, normal) * normal) - lightDir);
      specular = pow(max(dot(reflection, eyeVector), 0.0), gl_FrontMaterial.shininess) *
	gl_FrontMaterial.specular * gl_LightSource[lightIndex].specular;
      spotLightColor += att * specular;
    }
  }
  return spotLightColor;
}

vec4 calc_lighting_color(in vec3 normal) {
  vec4 lightingColor = vec4(0.0);

  for (int i = 0; i < lightsNumber; i++) {
    if (gl_LightSource[i].position.w == 0.0) {
      lightingColor += directional_light(i, normal);
    }
    else {
      if (gl_LightSource[i].spotCutoff <= 90.0) {
	lightingColor +=spot_light(i, normal);
      }
      else {
	lightingColor += point_light(i, normal);
      }
    }
  }
  return lightingColor;
}

float noise3(vec3 co){
  return fract(sin(dot(co ,vec3(12.9898,78.233,125.198))) * 43758.5453);
}

float smooth(float v) {
	return 3.*pow(v,2.)-2.*pow(v,3.);
}

float perlin3(vec3 p) {
	float val = 0.;
	for(float i=0.;i<3.;i += 1.){
		p *= pow(2.,i);
		vec3 c = floor(p);
		float u = smooth(fract(p.x));
		float v = smooth(fract(p.y));
		val = 1.-((1.-val)*(1.-pow(.5,i)*
			mix(mix(mix(noise3(c),noise3(c+vec3(1.,0.,0.)),u),
					mix(noise3(c+vec3(0.,1.,0.)),noise3(c+vec3(1.,1.,0)),u),v),
			    mix(mix(noise3(c+vec3(0.,0.,1.)),noise3(c+vec3(1.,0.,1.)),u),
					mix(noise3(c+vec3(0.,1.,1.)),noise3(c+vec3(1.,1.,1.)),u),v),fract(p.z))));
	}
	return val;
}

vec2 XRay(float _specularExpo){
	float sExpo = _specularExpo * 64.0;
	float fr = abs(dot( -normalize(eyeSpaceVertexPos.xyz), normalize( vertexNormal ) ) );
	float xray = pow(1. - fr, max( .51, sExpo * .1));
	return vec2(xray, fr);
}

void main(void){

	vec2 uv = gl_TexCoord[0].st;
	vec3 n = normalize(vertexNormal);
  
	vec2 pos = vertexPos.xz;
	pos.x *= -1.0;

	vec3 A = vec3(1.0,1.0,0.0);
	vec3 B = vec3(0.0,0.0,1.0);
	
	vec3 color = B;
	if( laserPosition.y > pos.x && laserPosition.x > pos.y){
		color = A;
	} else if( laserPosition.y > pos.x){
		color = vec3(0.0,1.0,0.0);
	} else if ( laserPosition.x > pos.y){
		color = vec3(1.0,0.0,0.0);
	}

	float threshold = laserPosition.x;
	if( pos.y - lineWidth*5.0 < threshold && !(pos.y + lineWidth*5.0 < threshold) ){
		float posPct = 1.0-((pos.y+lineWidth*5.0) - threshold)/(lineWidth*10.0);
		float noise = perlin3(vertexPos.xyz*vec3(time*0.1));
		vec3 green = mix(color,laserColor,noise);
		color = mix(color.xyz,green*2.0,pow(max(0.0, abs(sin(posPct*PI))*2.0-1.0),0.5));
	}

	threshold = laserPosition.y;
	if( pos.x - lineWidth*5.0 < threshold && !(pos.x + lineWidth*5.0 < threshold) ){
		float posPct = 1.0-((pos.x+lineWidth*5.0) - threshold)/(lineWidth*10.0);
		float noise = perlin3(vertexPos.xyz*vec3(time*0.1));
		vec3 green = mix(color,laserColor,noise);
		color.xyz = mix(color.xyz,green*2.0,pow(max(0.0, abs(sin(posPct*PI))*2.0-1.0),0.5));
	}

	// color *= ambientGlobal.rgb;
	color *= calc_lighting_color(n).rgb;

	gl_FragColor.rgb = color;
	gl_FragColor.a = 1.0;
}