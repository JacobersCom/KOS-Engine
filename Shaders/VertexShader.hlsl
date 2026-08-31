


struct VSInput
{
    [[vk::location(0)]]float4 pos : POSITION0;
    [[vk::location(1)]]float3 color : COLOR1;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR1;
};

VSOutput main(VSInput vert_in )
{
    
    VSOutput vert_out;
    vert_out.pos = vert_in.pos;
    vert_out.color = vert_in.color;
    return vert_out;
}