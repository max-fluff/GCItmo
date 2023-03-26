cbuffer mycBuffer : register(b0)
{
   row_major float4x4 matProj;
   row_major float4x4 matView;
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
    
    
   
    res = mul(res, matView);
    output.pos = mul(res, matProj);
	
     
	
	return output;
}


float4 PSMain( PS_IN input ) : SV_Target
{
    //float3 pixelColor = objTexture.Sample(objSamplerState, input.tex);
    
	
    return float4(1.0f,0.5f,1.0f, 1.0f);
}