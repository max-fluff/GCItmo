cbuffer mycBuffer : register(b0)
{
   row_major float4x4 worldViewProj;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);


struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    float4 res = input.col;
    
    //res.y = input.col;
    
    
    //output.pos = input.pos;
    output.pos = mul(input.pos, worldViewProj);
    output.col = input.col;
     
	
	return output;
}


float4 PSMain( PS_IN input ) : SV_Target
{
    input.col.x = input.col.x;
    input.col.y = input.col.y;
    float3 pixelColor = objTexture.Sample(objSamplerState, input.col.xy);
    
    
	
    return float4(pixelColor, 1.0f);
}