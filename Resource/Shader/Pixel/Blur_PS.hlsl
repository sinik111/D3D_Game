#include "../Include/Shared.hlsli"

////static const float gaussianKernel9[9] =
////{
////    0.016216f, 0.054054f, 0.121622f, 0.194595f, 0.227027f,
////    0.194595f, 0.121622f, 0.054054f, 0.016216f
////};

//static const float gaussianKernel13[13] =
//{
//    0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f,
//    0.199471f, // 중앙
//    0.176033f, 0.120985f, 0.064759f, 0.026995f, 0.008764f, 0.002216f
//};

//float4 main(PS_INPUT_TEXCOORD input) : SV_Target
//{
//    // 1. 중앙 픽셀 (가중치 제일 높음)
//    float3 color = 0.0f;
    
//    // 2. 양옆 픽셀들 (루프 돌면서 좌우/상하 동시에 처리)
//    for (int i = -6; i <= 6; i++)
//    {
//        float2 offset = g_blurDir * i;
        
//        // 정방향 (+)
//        color += g_texBlit.Sample(g_samLinear, input.texCoord + offset).rgb * gaussianKernel13[i + 6];
//    }
    
//    return float4(color, 1.0f);
//}

static const float offsets[5] =
{
    0.0, 1.41176471, 3.29411765, 5.17647059, 7.05882353
};

static const float weights[5] =
{
    0.19648255, 0.29690696, 0.09447040, 0.01038136, 0.00032658
};

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
    // 중앙
    float3 color = g_texBlit.Sample(g_samLinear, input.texCoord).rgb * weights[0];
    
    // 좌우 (4번 반복 -> 총 9번 샘플링)
    for (int i = 1; i < 5; ++i)
    {
        float2 offset = g_blurDir * offsets[i];
        color += g_texBlit.Sample(g_samLinear, input.texCoord + offset).rgb * weights[i];
        color += g_texBlit.Sample(g_samLinear, input.texCoord - offset).rgb * weights[i];
    }
    
    return float4(color, 1.0f);
}