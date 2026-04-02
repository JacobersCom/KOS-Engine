
struct PSout
{
    float4 position : SV_POSITION;
	float3 color : COLOR0;
};

float4 main(PSout o) : SV_Target0 
{
    return float4(o.color, 1.0);
}