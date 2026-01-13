#include "../Include/Shared.hlsli"

PS_INPUT main(VS_INPUT_POSITION input)
{
    PS_INPUT output;
    
    float4 worldPosition = mul(float4(input.position, 1.0f), g_world);
    
    output.position = mul(worldPosition, g_viewProjection);
    
    return output;
}