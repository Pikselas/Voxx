struct VertexShaderOut // this is the ouput type
{
    float2 tex : TEXCOORD; // this is the user defined semantic for color which will be passed to pixel shader
    float4 pos : SV_POSITION; // this is the SYSTEM_VALUE SEMANTIC this is fixed (and defined by the API) 
};


cbuffer sprite_transform
{
    matrix transform;
};

cbuffer screen_size
{
    float2 half_screen_size;
    float2 padding;
};

VertexShaderOut main(float2 pos : POSITION, float2 tex : TEXCOORD)
{
    VertexShaderOut Out;
    pos = mul(transform, float4(pos, 0.0f, 1.0f)).xy;
    pos /= half_screen_size;
    pos.x -= 1.0f;
    pos.y = 1.0f - pos.y;
    Out.pos = float4(pos , 0.0f , 1.0f);
    Out.tex = tex;
    return Out;
}