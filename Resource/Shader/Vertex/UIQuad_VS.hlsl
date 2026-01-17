#include "../Include/Shared.hlsli"

PS_INPUT_TEXCOORD main(VS_INPUT_TEXCOORD input)
{
    PS_INPUT_TEXCOORD o = (PS_INPUT_TEXCOORD) 0;

    // g_uiClip은 CPU에서 NDC까지 만든 행렬
    o.position = mul(float4(input.position.xy, 0.0f, 1.0f), g_uiClip);

    // UV: offset.xy + input * scale.xy
    o.texCoord = input.texCoord * g_uiUV.zw + g_uiUV.xy;
    return o;
}
