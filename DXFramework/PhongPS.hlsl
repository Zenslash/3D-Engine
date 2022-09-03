
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float3 materialColor;
};



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