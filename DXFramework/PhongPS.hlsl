
cbuffer LightCBuf
{
    float3 lightPos;
};

static const float3 materialColor = { 0.2f, 0.3f, 0.9f };
static const float3 ambient = { 0.0f, 0.0f, 0.0f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : POSITION, float3 n : NORMAL) : SV_TARGET
{
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    //Calculate diffuse attenuation
    const float attenuation = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    //Calculate diffuse
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(dirToL, n));
    
	return float4(saturate(diffuse + ambient) * materialColor, 1.0f);
}