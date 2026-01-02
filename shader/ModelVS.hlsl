#pragma pack_matrix(row_major)

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
    float3 worldPos : TEXCOORD0;
    float3 normal : NORMAL;
};

VSOutput main(VSInput input)
{
    VSOutput o;

    // ワールド座標変換
    float4 worldPos = mul(float4(input.pos, 1.0f), World);
    o.svpos = mul(worldPos, WVP);
    o.worldPos = worldPos.xyz;

    // 法線：World 行列の回転成分のみ使用
    o.normal = normalize(mul(input.normal, (float3x3) World));

    return o;
}