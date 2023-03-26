cbuffer MatrixBuffer
{
    row_major float4x4 worldMatrix;
    row_major float4x4 viewMatrix;
    row_major float4x4 projectionMatrix;
    row_major float4x4 lightViewMatrix;
    row_major float4x4 lightProjectionMatrix;
    float4 light;
    float4 ambient;
    float4 specular;
    float4 dir;
};


Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);


SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap : register(s1);


struct VS_IN
{
    float4 pos : POSITION;
    float4 tex : TEXCOORD0;
    float4 normals : NORMAL;
};

struct PS_IN
{
    float4 pos : POSITION;
    float4 tex : TEXCOORD0;
    float4 normals : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
};



PS_IN ShadowVertexShader(VS_IN input)
{
    PS_IN output = (PS_IN)0;


    // Change the position vector to be 4 units for proper matrix calculations.
    input.pos.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.pos = mul(input.pos, worldMatrix);
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);

    // Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(input.pos, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normals = mul(input.normals.xyz, (float3x3) worldMatrix);
	
    // Normalize the normal vector.
    output.normals = normalize(output.normals);

    return output;
}


float4 ShadowPixelShader(PS_IN input) : SV_TARGET
{
    float bias;
    float4 color;
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;
    float4 textureColor;
    float3 lightDir;
    // Invert the light direction.
    lightDir = -lightDirection;

    // Set the bias value for fixing the floating point precision issues.
    bias = 0.001f;

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Calculate the projected texture coordinates.
    projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

    // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        // Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
        depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

        // Calculate the depth of the light.
        lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

        // Subtract the bias from the lightDepthValue.
        lightDepthValue = lightDepthValue - bias;

        // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
        if (lightDepthValue < depthValue)
        {
             // Calculate the amount of light on this pixel.
            lightIntensity = saturate(dot(input.normal, lightDir));

            if (lightIntensity > 0.0f)
            {
                // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
                color += (diffuseColor * lightIntensity);

                // Saturate the final light color.
                color = saturate(color);
            }
        }
    }
    else
    {
        // If this is outside the area of shadow map range then draw things normally with regular lighting.
        lightIntensity = saturate(dot(input.normal, lightDir));
        if (lightIntensity > 0.0f)
        {
            color += (diffuseColor * lightIntensity);
            color = saturate(color);
        }
    }

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

    // Combine the light and texture color.
    color = color * textureColor;

    return color;
}
