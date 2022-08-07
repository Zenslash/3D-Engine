
struct VSOut
{
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer CBuffer
{
	matrix transform;
};

VSOut main(float3 pos : POSITION, float4 color : COLOR)
{
	VSOut vs;
	vs.pos = mul(transform, float4(pos, 1.f));
	vs.color = color;
	return vs;
}