#ifdef VERTEX_SHADER

layout(location=0)in vec4 mVertex;
layout(location=1)in vec2 mPxPosition;
layout(location=2)in vec2 mPxSize;
layout(location=3)in vec3 mUV;
layout(location=4)in vec2 mUVSize;
layout(location=5)in vec4 mColor;

uniform vec2 uFrameSize;

out vec3 vUV;
out vec4 vColor;

void main(){
    vColor = mColor.abgr;
    vUV.xy = mUV.xy + (mVertex.xy)*mUVSize;
    vUV.z = mUV.z;
    gl_Position = vec4(
                      (mVertex.xy*mPxSize + mPxPosition) / (uFrameSize/2) - vec2(1),
                      0,
                      1);
}

#endif

#ifdef FRAGMENT_SHADER

uniform sampler2DArray uTexture;

in vec3 vUV;
in vec4 vColor;

out vec4 outColor;

void main(){
    outColor = vColor * texture(uTexture, vUV).r;
}

#endif
