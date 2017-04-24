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

in vec2 vUV;

uniform sampler2D uBlurred;
uniform sampler2D uUIs;

void main(void){
    vec4 ui = texture(uUIs, vUV);
    vec4 blurred = texture(uBlurred, vUV);
    outColor.rgb = ui.rgb*ui.a + blurred.rgb*(1-ui.a);
    // outColor.a = ui.a > 0 ? 1 : 0;
    if(ui.a == 0) discard;
}

#endif
