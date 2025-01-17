#ifndef NORMAL_MAP_HLSL
#define NORMAL_MAP_HLSL


#include "vs_cbuffers.hlsl"
#include "ps_cbuffers.hlsl"


//------------------------------------------------------------------------------
// VertexIn
//------------------------------------------------------------------------------
struct VertexIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

//------------------------------------------------------------------------------
// PixelIn
//------------------------------------------------------------------------------
struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};


//------------------------------------------------------------------------------
// VS_NormalMap
//------------------------------------------------------------------------------
PixelIn VS_NormalMap( VertexIn input )
{
    PixelIn output;
    float4 worldLocation = mul( input.position, worldMat );
    
    output.position = worldLocation;
    output.position = mul( output.position, viewMat );
    output.position = mul( output.position, projMat );
    
    output.normal    = mul( input.normal   , (float3x3)worldMat );
    output.tangent   = mul( input.tangent  , (float3x3)worldMat );
    output.bitangent = mul( input.bitangent, (float3x3)worldMat );
    
    output.tex = input.tex;

    return output;
}

Texture2D diffuseTexture   : register( t0 );
Texture2D normalMapTexture : register( t1 );

SamplerState SampleType;

//------------------------------------------------------------------------------
// PS_NormalMap
//------------------------------------------------------------------------------
float4 PS_NormalMap( PixelIn input ) : SV_TARGET
{
    float4 textureColor = float4( 1.f, 1.f, 1.f, 1.f );
    textureColor = diffuseTexture.Sample( SampleType, input.tex );
    textureColor = pow( textureColor, 1.f/2.2f );

    float3x3 tbn = transpose( float3x3( input.tangent, input.bitangent, input.normal ) );
    float3 lightDirection_tangentSpace = mul( -lightDirection, tbn );

    float3 normal = normalize( normalMapTexture.Sample( SampleType, input.tex ).xyz * 2.f - 1.f );
    
    float3 lightDir = normalize( lightDirection_tangentSpace );
    float lightIntensity = saturate( dot( lightDir, normal ) );

    lightIntensity = lightIntensity * 0.5 + 0.5;
    lightIntensity = pow( lightIntensity, 2.f );

    float4 lightColor = saturate( diffuseColor * lightIntensity );
    float4 finalColor = textureColor * lightColor;
    finalColor.a = opacity;

    return finalColor;
}

#endif