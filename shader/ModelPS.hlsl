struct PSInput
{
    float4 svpos : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal : NORMAL; // ワールド空間想定
};

float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = saturate(
        abs(frac(hsv.x + float3(0, 2.0 / 3.0, 1.0 / 3.0)) * 6.0 - 3.0) - 1.0
    );
    rgb = rgb * rgb * (3.0 - 2.0 * rgb);
    return hsv.z * lerp(float3(1, 1, 1), rgb, hsv.y);
}

float4 main(PSInput input) : SV_TARGET
{
    /* ===== グラデーション ===== */
    float minY = -5.0f;
    float maxY = 5.0f;
    float t = saturate((input.worldPos.y - minY) / (maxY - minY));

    float3 baseColor = HSVtoRGB(float3(t, 1.0f, 1.0f));

    /* ===== Phong Lighting ===== */
    float3 N = normalize(input.normal);

    float3 lightDir = normalize(float3(0.3f, 1.0f, -0.5f)); // 平行光
    float3 viewPos = float3(0.0f, 5.0f, -10.0f); // カメラ位置
    float3 V = normalize(viewPos - input.worldPos);
    float3 R = reflect(-lightDir, N);

    // 環境光
    float3 ambient = 0.2f * baseColor;

    // 拡散反射
    float diff = saturate(dot(N, lightDir));
    float3 diffuse = diff * baseColor;

    // 鏡面反射
    float spec = pow(saturate(dot(R, V)), 32.0f);
    float3 specular = spec * float3(1.0f, 1.0f, 1.0f);

    float3 color = ambient + diffuse + specular;

    return float4(color, 1.0f);
}