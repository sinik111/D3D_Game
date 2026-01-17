#include "../Include/Shared.hlsli"

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
    // Å¬¸®ÇÎ: SV_PositionÀº ÇÈ¼¿ ÁÂÇ¥
    if ((g_uiFlags & 1u) != 0u)
    {
        float2 p = input.position.xy;
        if (p.x < g_uiClipRect.x || p.y < g_uiClipRect.y ||
            p.x > g_uiClipRect.z || p.y > g_uiClipRect.w)
        {
            discard;
        }
    }

    float4 tex = g_texBlit.Sample(g_samLinear, input.texCoord);
    return tex * g_uiColor;
}
