cbuffer Transform : register(b0)
{
    matrix WVP;
    matrix World;
};

struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
};

VSOutput main(VSInput input)
{
    VSOutput o;
    o.svpos = mul(float4(input.pos, 1.0f), WVP);
    o.normal = mul(input.normal, (float3x3) World);
    return o;
}