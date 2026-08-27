


struct VSInput
{
    [[vk::location(0)]] float4 pos : SV_POSITION;
    [[vk::location(1)]] float3 color : COLOR0;
};

struct VSOutput
{
    [[vk::location(0)]] float4 pos : SV_POSITION;
    [[vk::location(1)]] float3 color : COLOR0
};

VSOutput main(VSInput vert_in )
{
    
    VSOutput vert_out;
    vert_out.pos = vert_in.pos;
    vert_out.color = vert_in.color;
    return vert_out;
}