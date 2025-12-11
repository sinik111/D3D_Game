Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return shaderTexture.Sample(sampleType, input.Tex);
}
