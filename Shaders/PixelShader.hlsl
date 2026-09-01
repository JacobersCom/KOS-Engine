struct PSInput
{
    float4 pos : SV_POSITION;
    float3 fragColor : COLOR1;
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(input.fragColor, 1.0);
}
