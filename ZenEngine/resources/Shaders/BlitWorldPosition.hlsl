#include "ZenShaderLib.hlsl"

struct Vertex
{
    float2 Position: POSITION;
};

struct Interpolators
{
    float4 Position: SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

Texture2D InputTexture: register(t0);
SamplerState InputTexture_Sampler: register(s0);

Interpolators VSMain(Vertex v)
{
    Interpolators i;
    i.Position = float4(v.Position, 0.0f, 1.0f);
    i.TexCoord = 0.5f * (v.Position + float2(1.0f, 1.0f));
    return i;
}

float4 PSMain(Interpolators i)
{
    float3 position = WorldPositionFromDepth(InputTexture.Sample(InputTexture_Sampler, i.TexCoord).r, i.TexCoord);
    return float4(position, 1.0f);
}