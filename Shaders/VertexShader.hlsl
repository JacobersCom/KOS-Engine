
struct VSInput
{
    float2 inPosition;
    float3 inColor;
};

struct VSOutput
{
    float4 outPosition : SV_Position;
    float3 outColor;
};

VSOutput main(VSInput input)
{
    
    VSOutput output;
    output.outPosition = float4(input.inPosition, 0.0f, 1.0f);
    output.outColor = input.inColor;
    return output;
}