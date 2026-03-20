
struct PSout
{
	float4 color : COLOR0;
};

float4 main(PSout o) : SV_Target0 
{
    return o.color;
}