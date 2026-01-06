#include "../Include/Shared.hlsli"

static const float g_maxBloom = 20.0f;

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
    float2 uv = input.texCoord;
    
    float3 hdrColor = g_texBlit.Sample(g_samLinear, uv).rgb;
   
    float luminance = GetLuminance(hdrColor);
   
    float kneeLow = g_bloomThreshold;
    float kneeHigh = g_bloomThreshold + g_bloomSoftKnee;
   
    float mask = saturate((luminance - kneeLow) / max(g_bloomSoftKnee, 1e-5));
   
    mask = min(mask, g_maxBloom);
    
    return float4(mask, mask, mask, 1.0f);
}