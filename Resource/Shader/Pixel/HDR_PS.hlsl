#include "../Include/Shared.hlsli"

// PQ 인코딩에 사용되는 상수 (ST.2084)
static const float PQ_m1 = 2610.0 / 4096.0 / 4.0; // 0.1593017578125
static const float PQ_m2 = 2523.0 / 4096.0 * 128.0; // 78.84375
static const float PQ_c1 = 3424.0 / 4096.0; // 0.8359375
static const float PQ_c2 = 2413.0 / 4096.0 * 32.0; // 18.8515625
static const float PQ_c3 = 2392.0 / 4096.0 * 32.0; // 18.6875

// Rec. 709 -> Rec. 2020 변환 행렬 (간소화)
// 1. R709_to_R2020 행렬 정의 (행 주소/Row-major)
float3x3 R709_to_R2020 =
{
    0.6274f, 0.3293f, 0.0433f, // 첫 번째 행 (R_out)
    0.0691f, 0.9195f, 0.0114f, // 두 번째 행 (G_out)
    0.0164f, 0.0889f, 0.8947f // 세 번째 행 (B_out)
};

float3 LinearToPQ(float3 linearColor, float maxNits)
{
    // 먼저 Rec.709 -> Rec.2020 변환 (옵션, Unreal에서도 비슷한 매트릭스 사용)
    static const float3x3 Rec709to2020 =
    {
        0.6274040f, 0.3292820f, 0.0433136f,
        0.0690970f, 0.9195400f, 0.0113612f,
        0.0163916f, 0.0880132f, 0.8955950f
    };
    linearColor = mul(Rec709to2020, linearColor);

    // tonemapped linear 값을 maxNits로 스케일링 후 PQ 적용
    float3 norm = linearColor * (maxNits / 10000.0f);

    float3 powered = pow(abs(norm), PQ_m1);
    float3 num = PQ_c1 + PQ_c2 * powered;
    float3 den = 1.0f + PQ_c3 * powered;
    float3 pq = pow(num / den, PQ_m2);

    return pq;
}

float4 main(PS_INPUT_TEXCOORD input) : SV_Target
{
    float3 linear709 = g_texHDR.Sample(g_samLinear, input.texCoord).rgb;
    float3 bloom = g_texBlit.Sample(g_samLinear, input.texCoord).rgb;
    
    float exposureFactor = pow(2.0f, g_exposure);
    linear709 *= exposureFactor;
    linear709 += (bloom * g_bloomStrength);
    
    // PQ로 변환 (HDR10 출력용)
    float3 pq = LinearToPQ(linear709, g_maxHDRNits);
    // 최종 PQ 인코딩된 값 [0.0, 1.0]을 R10G10B10A2_UNORM 백버퍼에 출력
    return float4(pq, 1.0);
}