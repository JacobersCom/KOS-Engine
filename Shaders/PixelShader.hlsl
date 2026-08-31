
struct PSOutput
{
    [[vk::location(0)]]float4 position : POSITION0;
    [[vk::location(1)]]float3 color : COLOR1;
};

float4 main(PSOutput pix_out) : SV_Target
{
	return float4(pix_out.color, 1.0);
}
