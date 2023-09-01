﻿#include "Dx11LevelLoader.h"
#include "../Dx11.h"
#include "../Asset/CrMesh.h"
#include "../Core/Dx11Texture2D.h"
#include "../Core/Dx11ResourceManager.h"
#include "../Level/CrLevel.h"
#include "../Render/Dx11Mesh.h"
#include "../Render/Dx11Renderer.h"


//=====================================================================================================================
// @brief	Load
//=====================================================================================================================
void Dx11LevelLoader::Load( CrLevel& InLevel )
{
    int i = 0;
    for ( const CrMesh* crMesh : InLevel.GetMeshes() )
    {
        Dx11IndexBuffer*  ib = GetDx11ResourceManager()->CreateIndexBuffer ( crMesh->GetPrimitve    () );
        Dx11VertexShader* vs = GetDx11ResourceManager()->CreateVertexShader( crMesh->GetVertexShader() );
        Dx11PixelShader*  ps = GetDx11ResourceManager()->CreatePixelShader ( crMesh->GetPixelShader () );

        Dx11VertexBuffer* vb = nullptr;
        if ( crMesh->GetVertexShader()->GetName().find( "Normal" ) != std::string::npos )
        {
            vb = GetDx11ResourceManager()->CreateVertexBuffer< Vertex_NormalMap >( crMesh->GetPrimitve() );
        }
        else
        {
            vb = GetDx11ResourceManager()->CreateVertexBuffer< Vertex_Diffuse >( crMesh->GetPrimitve() );            
        }        

        Dx11Mesh* dxMesh =new Dx11Mesh();
        dxMesh->InitializePrimitive( vb, ib );
        dxMesh->InitializeMaterial( vs, ps );

        std::vector< Dx11ResourceRenderer* > dxRRs;
        for ( int t = 0; t < crMesh->GetCount_Texture2D(); ++t )
        {
            Dx11ResourceRenderer* dxRR = GetDx11ResourceManager()->CreateResourceRenderer_Texture2D( crMesh->GetTexture2D( t ), t );
            if ( !dxRR ) break;

            dxRRs.push_back( dxRR );
        }
        dxMesh->InitializeTexture2Ds( dxRRs );

        Meshes.push_back( dxMesh );

        dxMesh->GetTransform().SetLocationX( -8.f + 8.f * ( ( i ) % 3 ) );
        dxMesh->GetTransform().SetLocationY( -4.f + 8.f * ( ( i ) / 3 ) );
        ++i;
    }
    TestMesh = new Dx11Mesh();
    *TestMesh = *Meshes[ 0 ];
    TestMesh->GetTransform().SetLocation( 0.f, 0.f, 0.f );

    TestRT = new Dx11Mesh();
    Dx11VertexBuffer* vb = GetDx11ResourceManager()->CreateVertexBuffer< Vertex_Diffuse >( "Plane" );
    Dx11IndexBuffer*  ib = GetDx11ResourceManager()->CreateIndexBuffer ( "Plane" );

    Dx11VertexShader* vs = GetDx11ResourceManager()->CreateVertexShader( "DefaultDiffuse" );
    Dx11PixelShader*  ps = GetDx11ResourceManager()->CreatePixelShader ( "DefaultDiffuse" );

    TestRT->InitializePrimitive( vb, ib );
    TestRT->InitializeMaterial( vs, ps );

    std::vector< Dx11ResourceRenderer* > dxRRs;
    dxRRs.push_back( GetDx11ResourceManager()->CreateResourceRenderer_Texture2D( G_Dx11Renderer.GetRenderTarget( "TestRT" )->GetTexture2D(), 0 ) );

    TestRT->InitializeTexture2Ds( dxRRs );

    TestRT->GetTransform().SetLocationX( -8.f + 8.f * ( Meshes.size() % 3 ) );
    TestRT->GetTransform().SetLocationY( -4.f + 8.f * ( Meshes.size() / 3 ) );
    TestRT->GetTransform().SetScale( 4.f, 4.f, 1.f );

    
}

//=====================================================================================================================
// @brief	Add render meshes
//=====================================================================================================================
void Dx11LevelLoader::AddRenderMeshes( Dx11Renderer& InRenderer ) const
{
    auto itr = Meshes.begin();

    for ( ; itr != Meshes.end(); ++itr )
    {
        InRenderer.AddMeshRenderElement( *itr );
    }

    InRenderer.AddMeshRenderElement( TestMesh, "TestRT" );
    
    InRenderer.AddMeshRenderElement( TestRT );
    InRenderer.SortRenderQueue();
}
