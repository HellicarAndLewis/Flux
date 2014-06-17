varying vec4 ambientGlobal, eyeSpaceVertexPos;
varying vec4 vertexPos;
varying vec3 vertexNormal;

void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vertexPos = gl_Vertex;
	vertexNormal = normalize(gl_NormalMatrix * gl_Normal);

	eyeSpaceVertexPos = gl_ModelViewMatrix * gl_Vertex;

	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient + gl_FrontMaterial.emission;

	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}