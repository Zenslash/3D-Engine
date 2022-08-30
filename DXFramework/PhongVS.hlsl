
cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : POSITION, float3 n : NORMAL )
{
    VSOut vso;
    vso.worldPos = (float3)mul(float4(pos, 1.0f), model);
    vso.normal = mul(n, (float3x3) model);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
	return vso;
}