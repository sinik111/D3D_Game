#include "../Include/Shared.hlsli"

PS_INPUT_GBUFFER main(VS_INPUT_TEXCOORD input)
{
    PS_INPUT_GBUFFER output = (PS_INPUT_GBUFFER) 0;
    
    output.position = mul(float4(input.position, 1.0f), g_world);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, g_viewProjection);
    
    output.normal = mul(float3(0.0f, 0.0f, -1.0f), (float3x3) g_worldInverseTranspose);
    output.tangent = mul(float3(1.0f, 0.0f, 0.0f), (float3x3) g_worldInverseTranspose);
    output.binormal = mul(float3(0.0f, -1.0f, 0.0f), (float3x3) g_worldInverseTranspose);
    
    output.texCoord = input.texCoord;
    
    return output;
}