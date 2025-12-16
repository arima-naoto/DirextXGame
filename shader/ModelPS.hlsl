struct PSInput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 lightDir = normalize(float3(0.0f, 1.0f, -1.0f));
    float n = saturate(dot(normalize(input.normal), lightDir));
    return float4(n, n, n, 1.0f);
}