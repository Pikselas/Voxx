Texture2D tex;
SamplerState sam;

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
    return tex.Sample(sam , tc);
}