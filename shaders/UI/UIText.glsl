#ifdef VERTEX_SHADER

layout(location=0)in vec4 mVertex;
layout(location=1)in vec2 mPosition;
layout(location=2)in vec2 mSize;
layout(location=3)in vec4 mUV;
layout(location=4)in vec4 mUVSize;
layout(location=5)in vec4 mColor;

uniform float uWidth;
uniform float uHeight;

out vec3 vUV;
out vec4 vColor;

void main(){
    vColor = mColor.abgr;
    gl_Position = vec4( (mVertex.xy*mSize+ceil(mPosition)) /vec2(uWidth/2, uHeight/2) - vec2(1), 0, 1);
    vUV.xy = mUV.xy + mVertex.xy*mUVSize;
    vUV.z = mUV.z;
}

#endif

#ifdef FRAGMENT_SHADER

uniform sampler2DArray uTexture;

in vec3 vUV;
in vec4 vColor;

out vec4 outColor;

void main(){
    outColor = vColor*texture(uTexture, vUV).r;
}

#endif

