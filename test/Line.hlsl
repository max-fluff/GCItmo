
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
    input.pos.x += xOffset;
    input.pos.y += yOffset;
	output.pos = input.pos;
	output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
    float4 col=input.col;
    //if (floor((input.pos.y)) % 40 > 20)
    //{
    //    col.x = 0.0f;
    //    col.y = 0.0f;
    //    col.z = 0.0f;
    //}
    //else
    //{
    //    col.x = 1.0f;
    //    col.y = 1.0f;
    //    col.z = 1.0f;
    //}
	
    return col;
}