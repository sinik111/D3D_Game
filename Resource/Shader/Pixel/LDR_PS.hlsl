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

float3 LinearToSRGB(float3 linearColor)
{
    return pow(linearColor, 1.0f / 2.2f);
}

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
     // 1. 선형 HDR 값 로드 (Nits 값으로 간주)
    float3 C_linear709 = g_texHDR.Sample(g_samLinear, input.texCoord).rgb;
    
    float exposureFactor = pow(2.0f, g_exposure);
    C_linear709 *= exposureFactor;

    float3 C_tonemapped;
    C_tonemapped = ACESFilm(C_linear709);
   
    float3 C_final;
    C_final = LinearToSRGB(C_tonemapped);
    return float4(C_final, 1.0);
}
