cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VSOut
{
    float3 cameraPos : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float3 n : NORMAL, float2 texCrd : TEXCOORD)
{
    VSOut vso;
    vso.cameraPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.normal = mul(n, (float3x3) modelView);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    vso.tex = texCrd;
    return vso;
}