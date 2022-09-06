
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
    float specularIntensity;
    float specularPower;
};



float4 main(float3 cameraPos : POSITION, float3 n : NORMAL) : SV_TARGET
{
    const float3 vToL = lightPos - cameraPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    //Calculate diffuse attenuation
    const float attenuation = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    //Calculate diffuse
    const float3 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(dirToL, n));
    
    //specular
    const float r = reflect(vToL, n);
    const float3 specular = (diffuseColor * diffuseIntensity) * specularIntensity
    * pow(max(0.0f, dot(normalize(-r), normalize(cameraPos))), specularPower);
    
	return float4(saturate(diffuse + ambient + specular) * materialColor, 1.0f);
}