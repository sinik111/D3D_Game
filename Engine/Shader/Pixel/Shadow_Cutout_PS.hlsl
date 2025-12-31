#include "../Include/Shared.hlsli"

void main(PS_INPUT_TEXCOORD input)
{
    clip(g_texBaseColor.Sample(g_samLinear, input.texCoord).a - 0.5f);
}