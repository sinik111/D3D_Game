#include "../Include/Shared.hlsli"

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
    return g_texBlit.Sample(g_samLinear, input.texCoord);
}