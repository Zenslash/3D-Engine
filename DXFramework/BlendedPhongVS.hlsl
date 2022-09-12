cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VSOut
{
    float3 cameraPos : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float3 n : NORMAL, float3 color : COLOR)
{
    VSOut vso;
    vso.cameraPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.normal = mul(n, (float3x3) modelView);
    vso.color = color;
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}