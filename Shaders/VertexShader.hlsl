


struct VSInput
{
    float4 inPosition : POSITION0;
    float3 inColor : COLOR1;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 fragColor : COLOR1;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.pos = input.inPosition;
    output.fragColor = input.inColor;
    return output;
}