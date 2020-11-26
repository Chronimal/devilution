//------------------------------------------------------------------------------------
// pixelshader.hlsl
//
// Shader to map 8 bit indcies to RGBA using a color palette
//
//--------------------------------------------------------------------------------------

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D<int> canvas : register(t0);
Texture1D palette : register(t1);

SamplerState samplerState : register(s0);

float4 main(PS_Input input) : SV_Target
{
    int2 size;
    canvas.GetDimensions(size.x, size.y);

    int index = canvas.Load(int3(size * input.uv, 0)).r;
    return palette.Load(int2(index, 0)).bgra;
}
