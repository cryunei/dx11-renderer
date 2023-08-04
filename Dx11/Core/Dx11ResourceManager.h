﻿#pragma once


#include "../DxTypes.h"
#include <map>


class CrTexture2D;
class CrPixelShader;
class CrVertexShader;
class CrIndexBuffer;
class CrVertexBuffer;
class Dx11Texture2D;
class Dx11PixelShader;
class Dx11IndexBuffer;
class Dx11VertexBuffer;
class Dx11VertexShader;


//=====================================================================================================================
// @brief	Dx11ResourceManager
//=====================================================================================================================
class Dx11ResourceManager
{
private:
    std::map< std::string, Dx11VertexBuffer* > VertexBuffers;
    std::map< std::string, Dx11IndexBuffer* >  IndexBuffers;
    std::map< std::string, Dx11VertexShader* > VertexShaders;
    std::map< std::string, Dx11PixelShader* >  PixelShaders;
    std::map< std::string, Dx11Texture2D* >    Texture2Ds;

public:
    // Construct
    Dx11ResourceManager() = default;
    Dx11ResourceManager( const Dx11ResourceManager& Other ) = delete;
    Dx11ResourceManager( Dx11ResourceManager&& Other ) noexcept = delete;

    // Destruct
    ~Dx11ResourceManager() = default;

    // Operators
    Dx11ResourceManager& operator=( const Dx11ResourceManager& Other ) = delete;
    Dx11ResourceManager& operator=( Dx11ResourceManager&& Other ) noexcept = delete;

    // Create vertex buffer
    Dx11VertexBuffer* CreateVertexBuffer( const std::string& AssetName );
    Dx11VertexBuffer* CreateVertexBuffer( const CrVertexBuffer* AssetPtr );

    // Create index buffer
    Dx11IndexBuffer* CreateIndexBuffer( const std::string& AssetName );
    Dx11IndexBuffer* CreateIndexBuffer( const CrIndexBuffer* AssetPtr );

    // Create vertex shader
    Dx11VertexShader* CreateVertexShader( const std::string& AssetName );
    Dx11VertexShader* CreateVertexShader( const CrVertexShader* AssetPtr );

    // Create pixel shader
    Dx11PixelShader* CreatePixelShader( const std::string& AssetName );
    Dx11PixelShader* CreatePixelShader( const CrPixelShader* AssetPtr );

    // Create texture
    Dx11Texture2D* CreateTexture2D( const std::string& AssetName );
    Dx11Texture2D* CreateTexture2D( const CrTexture2D* AssetPtr );

    // Get vertex buffer
    Dx11VertexBuffer* GetVertexBuffer( const std::string& AssetName ) { return _get< Dx11VertexBuffer >( AssetName, VertexBuffers ); }

    // Get index buffer
    Dx11IndexBuffer* GetIndexBuffer( const std::string& AssetName ) { return _get< Dx11IndexBuffer >( AssetName, IndexBuffers );}

    // Get vertex shader
    Dx11VertexShader* GetVertexShader( const std::string& AssetName ) { return _get< Dx11VertexShader >( AssetName, VertexShaders ); }

    // Get pixel shader
    Dx11PixelShader* GetPixelShader( const std::string& AssetName ) { return _get< Dx11PixelShader >( AssetName, PixelShaders ); }

    // Get texture
    Dx11Texture2D* GetTexture2D( const std::string& AssetName ) { return _get< Dx11Texture2D >( AssetName, Texture2Ds ); }

    // Release
    void Release();

private:
    // Create
    template < typename T >
    T* _create( const std::string& Name, std::map< std::string, T* >& Map );

    // Get
    template < typename T >
    T* _get( const std::string& Name, std::map< std::string, T* >& Map );
};


//=====================================================================================================================
// @brief	Create
//=====================================================================================================================
template < typename T >
T* Dx11ResourceManager::_create( const std::string& Name, std::map< std::string, T* >& Map )
{
    if ( T* inst = _get( Name, Map ) )
    {
        return inst;
    }

    return new T();
}

//=====================================================================================================================
// @brief	Get
//=====================================================================================================================
template < typename T >
T* Dx11ResourceManager::_get( const std::string& Name, std::map<std::string, T*>& Map )
{
    auto itrFind = Map.find( Name );

    if ( itrFind != Map.end() )
    {
        return itrFind->second;
    }

    return nullptr;
}


//=====================================================================================================================
// @brief	GetDx11ResourceManager
//=====================================================================================================================
Dx11ResourceManager* GetDx11ResourceManager();
