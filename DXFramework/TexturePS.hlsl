Texture2D tex;
SamplerState state;

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
	return tex.Sample(state, tc);
}