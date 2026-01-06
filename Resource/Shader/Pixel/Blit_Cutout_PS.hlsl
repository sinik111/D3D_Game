#include "../Include/Shared.hlsli"

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
    float4 color = g_texBlit.Sample(g_samLinear, input.texCoord);
    
    clip(color.a < 0.5f);
    
    return color;
}