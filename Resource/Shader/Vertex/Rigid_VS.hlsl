#include "../Include/Shared.hlsli"

PS_INPUT_GBUFFER main(VS_INPUT_COMMON input)
{
    PS_INPUT_GBUFFER output = (PS_INPUT_GBUFFER) 0;
    
    float4x4 world = mul(g_boneTransform[g_boneIndex], g_world);
    
    output.position = mul(float4(input.position, 1.0f), world);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, g_viewProjection);
    
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = mul(input.binormal, (float3x3) world);
    
    output.texCoord = input.texCoord;
    
    return output;
}