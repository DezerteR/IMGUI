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

uniform sampler2D uTexture;

void main(void){
    vec4 color = texture(uTexture, vUV);
    outColor = color;
}

#endif
