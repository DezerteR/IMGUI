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

// uniform sampler2D uBlured;
uniform sampler2D uSource2;
uniform sampler2D uSource4;

in vec2 vUV;

void main(void){
    vec4 source2 = texture(uSource2, vUV);
    vec4 source4 = texture(uSource4, vUV);
    outColor = (source2*0.5 + source4)*0.25;
    // outColor.rgb *= outColor.a;
}

#endif
