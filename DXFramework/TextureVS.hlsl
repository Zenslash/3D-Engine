cbuffer CBuf
{
	matrix transform;
};

struct VSOut
{
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};

VSOut main( float3 pos : POSITION, float2 tex : TEXCOORD)
{
	VSOut o;
	o.pos = mul(float4(pos, 1.0f), transform);
	o.tex = tex;
	return o;
}