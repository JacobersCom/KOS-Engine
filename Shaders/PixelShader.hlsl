
struct PSOutput
{
    float4 position : SV_POSITION;
	float3 color : COLOR0;
};

float4 main(PSout pix_out) : SV_Target0 
{
    return float4(pix_out.color, 1.0);
}