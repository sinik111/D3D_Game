#include "../Include/Shared.hlsli"

PS_INPUT_TEXCOORD main(VS_INPUT_TEXCOORD input)
{
    PS_INPUT_TEXCOORD output = (PS_INPUT_TEXCOORD) 0;
	
    output.position = float4(input.position.xy, 0.0f, 1.0f);
    output.texCoord = input.texCoord;
	
    return output;
}