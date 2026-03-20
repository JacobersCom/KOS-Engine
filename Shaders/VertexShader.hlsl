
float2 pos[3] =
{
    float2(0.0f, -0.5),
	float2(0.5f, 0.0),
	float2(-0.5, 0.0)
};

float3 color[3] =
{
    float3(1.0f, 0.0f, 0.0f),
    float3(0.0f, 1.0f, 0.0f),
    float3(0.0f, 0.0f, 1.0f),
    
};


struct VSout
{
    float4 pos : SV_Position;
    float3 color : COLOR0;
};

VSout main(uint vertexId : SV_VertexID )
{
    VSout o;
    o.pos = float4(pos[vertexId], 0.0f, 1.0f);
    o.color = color[vertexId];
    return o;
}