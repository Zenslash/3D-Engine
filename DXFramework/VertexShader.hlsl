
struct VSOut
{
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer CBuffer
{
	matrix transform;
};

VSOut main(float2 pos : POSITION, float4 color : COLOR)
{
	VSOut vs;
	vs.pos = mul(transform, float4(pos.x, pos.y, 0.f, 1.f));
	vs.color = color;
	return vs;
}