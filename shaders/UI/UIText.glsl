#ifdef VERTEX_SHADER

layout(location=0)in vec4 mVertex;
layout(location=1)in vec2 mPosition;
layout(location=2)in vec2 mSize;
layout(location=3)in vec4 mUV;
layout(location=4)in vec4 mColor;

uniform float uWidth;
uniform float uHeight;

out vec2 vUV;
out vec4 vColor;

void main(){
    vColor = mColor.abgr;
    gl_Position = vec4( (mVertex.xy*mSize+mPosition) /vec2(uWidth/2, uHeight/2) - vec2(1), 0, 1);
    vUV = mUV.xy + mVertex.xy*mUV.zw;
}

#endif

#ifdef FRAGMENT_SHADER

uniform sampler2D uTexture;

in vec2 vUV;
in vec4 vColor;

out vec4 outColor;

void main(void){
    outColor = vColor*texture(uTexture, vUV).r;
}

#endif

