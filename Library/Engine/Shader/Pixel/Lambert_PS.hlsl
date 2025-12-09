cbuffer SimpleConstant : register(b0)
{
    float4x4 g_world;
    float4x4 g_view;
    float4x4 g_projection;
    float3 g_lightDir;
    float __pad1;
    float3 g_lightColor;
    float __pad2;
    float3 g_ambientColor;
    float __pad3;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float lightIntensity = saturate(dot(normalize(input.normal), -g_lightDir));
    
    return float4(saturate(g_lightColor * lightIntensity + g_ambientColor), 1.0f);
}