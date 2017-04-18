#ifdef VERTEX_SHADER

layout(location=0)in vec4 mVertex;


out vec2 vUV;

void main(){
    vUV = mVertex.zw;
    gl_Position = vec4(mVertex.xy,0,1);
}

#endif

#ifdef FRAGMENT_SHADER
out vec4 outColor;

uniform sampler2D uBlured;
uniform sampler2D uSource;

in vec2 vUV;

void main(void){
    outColor = texture(uBlured, vUV);
    vec4 source = texture(uSource, vUV);
    // outColor.rgb *= outColor.a;
    outColor.rgb = sqrt(outColor.rgb*outColor.a);
    // outColor = texture(uTexture12, vUV) + texture(uTexture14, vUV) + texture(uTexture18, vUV);
}

#endif
