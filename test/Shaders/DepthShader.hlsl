
struct CascadeData
{
    row_major float4x4 viewProj[4];
};

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
    CascadeData cascData;
    
};

struct GS_IN
{
    float4 pos : POSITION;
};

struct GS_OUT
{
    float4 pos : SV_Position;
    uint arrInd : SV_RenderTargetArrayIndex;


};

struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
};

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};




[instance(1)]
[maxvertexcount(3)]
void GSMain(triangle GS_IN p[3], in uint id : SV_GSInstanceID, inout TriangleStream<GS_OUT> stream)
{
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        GS_OUT gs = (GS_OUT) 0;
        gs.pos = mul(float4(p[i].pos.xyz, 1.0f), cascData.viewProj[id]);
        gs.arrInd = id;
        stream.Append(gs);

    }
    stream.RestartStrip();
    
}


PixelInputType
    DepthVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.pos = mul(input.position, world);
    output.pos = mul(float4(output.pos.xyz, 1.0f), cascData.viewProj[0]);
    output.pos.w = 1.0f;
	
    return output;
}




//float4 DepthPixelShader(GS_OUT input) : SV_TARGET
//{
//    //float depthValue;
//    //float4 color;
	
	

//    //depthValue = input.depthPosition.z / input.depthPosition.w;

//    //color = float4(depthValue, depthValue, depthValue, 1.0f);

//    return float4(1, 1, 1, 1);
//}