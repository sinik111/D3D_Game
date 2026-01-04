#include "../Include/Shared.hlsli"

// 입력: x는 모니터 Max Nits를 기준으로 정규화된 선형 RGB 값 (float3)
// 출력: 0.0 ~ 1.0 범위의 압축된 선형 RGB 값 (float3)
float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate(x * (a * x + b) / (x * (c * x + d) + e));
}

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
     // 1. 선형 HDR 값 로드 (Nits 값으로 간주)
    float3 linear709 = g_texHDR.Sample(g_samLinear, input.texCoord).rgb;
    float3 bloom = g_texBlit.Sample(g_samLinear, input.texCoord).rgb;
    
    float exposureFactor = pow(2.0f, g_exposure);
    linear709 *= exposureFactor;

    linear709 += (bloom * g_bloomStrength);
    
    float3 tonemapped = ACESFilm(linear709);
   
    float3 final = LinearToSRGB(tonemapped);
    
    return float4(final, 1.0);
}
