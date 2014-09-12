//--------------------------------------------------------------------------------------
// File: AEroD3D.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

Texture2D<float4> Texture : register(t0);
sampler Sampler : register(s0);


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
	float4 Clr : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
	float4 Clr : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	output.Pos.xyz = input.Pos.xyz;
	output.Pos.w = 1.0f;
    output.Tex = input.Tex;
	output.Clr = input.Clr;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	float4 finalColor;
	finalColor = Texture.Sample(Sampler, input.Tex);
	finalColor = finalColor * input.Clr;
	clip(finalColor.a - 0.01f);
	return finalColor;
}
