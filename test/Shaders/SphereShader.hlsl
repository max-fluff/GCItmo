cbuffer mycBuffer : register(b0)
{
    row_major float4x4 worldViewProj;
    row_major float4x4 worldViewInverseT;
    row_major float4x4 world;
    row_major float4x4 lightViewProj;
    row_major float4x4 worldView;
    float4 light;
    float4 ambient;
    float4 specular;
    float4 dir;
    float4 eyePos;
    row_major float4x4 viewProj[4];
    float4 distances[4];
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

Texture2DArray depthMapTexture : register(t1);
SamplerState depthMapSampler : register(s1);


struct VS_IN
{
    float4 pos : POSITION0;
    float4 col : COLOR0;
};

struct PS_IN
{
    float4 posH : SV_POSITION;
    float4 posW : POSITION;
    float4 col : COLOR;
    float4 normal : NORMAL;
};

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;
	
    float4 res = input.col;
    
    //res.y = input.col;
    
    
    //output.pos = input.pos;
    output.normal.xyz = normalize(mul(normalize(input.pos.xyz), (float3x3) worldViewInverseT));
    output.posH = mul(input.pos, worldViewProj);
    output.posW = mul(input.pos, world);
    output.col = input.col;
     
	
	return output;
}


float4 PSMain( PS_IN input ) : SV_Target
{
    input.col.xy = input.col.xy;
    

    float3 pixelColor = objTexture.Sample(objSamplerState, input.col.xy);
    
    
    float3 normal = normalize(input.normal.xyz);
    float3 lightVec = -normalize(dir.xyz).xyz;
    float3 toEye = normalize(eyePos.xyz - input.posW.xyz);
    float lightIntesity = light.w;
    float specularAlpha = specular.w;
  
    

    
    float3 diffuseFactor = lightIntesity * light * dot(lightVec, normal.xyz);
    
    diffuseFactor.x = max(diffuseFactor.x, 0);
    diffuseFactor.y = max(diffuseFactor.y, 0);
    diffuseFactor.z = max(diffuseFactor.z, 0);
    
    float3 specFactor = float3(0, 0, 0);
    
    if (length(diffuseFactor.xyz) > 0.0f)
    {
        float3 v = reflect(-lightVec,normal.xyz);
        specFactor = specular.xyz * lightIntesity * light.xyz * pow(max(dot(v, toEye), 0.0f), specularAlpha);
        
    }
    

    float3 resColor = diffuseFactor * pixelColor + ambient.xyz * pixelColor + specFactor;


   
	
    return float4(resColor, 1.0f);
}