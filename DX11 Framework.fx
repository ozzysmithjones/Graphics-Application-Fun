//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define NUM_LIGHTS 2
#define SPECULAR_POWER 10.0f
#define SPOTLIGHT_ANGLE 0.8f


struct LIGHT
{
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Position;
};

struct SPOTLIGHT
{
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Position;
    float4 Direction;
};



//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;

    float4 DiffuseMtrl;
    float4 AmbientMtrl;
    float4 SpecularMtrl;
	
    float3 EyePosW; 	// Camera position in world space
    float time;
}

cbuffer LightBuffer : register(b1)
{
    LIGHT Lights[NUM_LIGHTS];
    SPOTLIGHT SpotLight;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : NORMAL;
    float3 PosW : POSITION;
	float2 Tex : TEXCOORD0;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD0;
};

//------------------------------------------------------------------------------------
// Vertex Shader 
//------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;

	output.Pos = mul(input.Pos, World);
	output.PosW = output.Pos;

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	// Convert from local space to world space 
	// W component of vector is 0 as vectors cannot be translated
	float3 normalW = mul(float4(input.NormalL, 0.0f), World).xyz;
	normalW = normalize(normalW);

	output.Norm = normalW;
	output.Tex = input.Tex;
	
	return output;
}

float3 SpotLightColor(VS_OUTPUT input)
{
    float4 color = float4(0, 0, 0, 0);
    
    float3 toEye = normalize(EyePosW - input.PosW);
    float3 toLight = normalize(SpotLight.Position.xyz - input.PosW);
    
    // Ambient lighting
    float3 ambient = (SpotLight.Ambient * AmbientMtrl).rgb;
    
     //spotlight effect:
    float dotAngle = dot(toLight, SpotLight.Direction.xyz);
    if (dotAngle > -SPOTLIGHT_ANGLE)
    {
        return ambient;
    }
    
	//specular
    float3 r = reflect(-toLight, input.Norm);
    float specularAmount = pow(max(dot(r, toEye), 0.0f), SPECULAR_POWER);
		
	// Diffuse lighting
    float diffuseAmount = max(dot(toLight, input.Norm), 0.0f);
    float3 diffuse = diffuseAmount * (DiffuseMtrl * SpotLight.Diffuse).rgb;

	
		
	//specular lighting:
    float3 specular = specularAmount * (SpecularMtrl * SpotLight.Specular).rgb;
    
    color.rgb = max((diffuse + ambient + specular).rgb, color.rgb);
    return color.rgb;
}

//--------------------------------------------------------------------------------------
// Pixel Shader- Implements Gouraud Shading 
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 toEye = normalize(EyePosW - input.PosW);
    float4 color = float4(0, 0, 0, 0);

    
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        float3 toLight = normalize(Lights[i].Position.xyz - input.PosW);
		
        
		
		// Diffuse lighting
        float diffuseAmount = max(dot(toLight, input.Norm), 0.0f);
        float3 diffuse = diffuseAmount * (DiffuseMtrl * Lights[i].Diffuse).rgb;

		// Ambient lighting
        float3 ambient = (Lights[i].Ambient * AmbientMtrl).rgb;
		
		//specular lighting:
        //float3 r = reflect(-toLight, input.Norm);
       // float specularAmount = pow(max(dot(r, toEye), 0.0f), SPECULAR_POWER);
       // float3 specular = specularAmount * (SpecularMtrl * Lights[i].Specular).rgb;

		//final colour:
        color.rgb = max((diffuse + ambient).rgb, color.rgb);
    }
    
    color.rgb = max(SpotLightColor(input),color.rgb);
    color.a = DiffuseMtrl.a;
	float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);
	return color * textureColour;
}
