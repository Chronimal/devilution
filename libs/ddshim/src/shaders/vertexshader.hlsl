//--------------------------------------------------------------------------------------
// vertexshader.hlsl
//--------------------------------------------------------------------------------------

struct VS_Input
{
    float2 pos : POS;
    float2 uv : TEX;
};

struct VS_Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_Output main(VS_Input input)
{
    VS_Output output;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}
