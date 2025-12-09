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

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    
    output.position = mul(float4(input.position, 1.0f), g_world);
    output.position = mul(output.position, g_view);
    output.position = mul(output.position, g_projection);
    
    output.normal = mul(input.normal, (float3x3) g_world);
    
    return output;
}