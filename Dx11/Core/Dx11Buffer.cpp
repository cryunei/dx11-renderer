﻿#include "Dx11Buffer.h"
#include "Dx11Device.h"


//=====================================================================================================================
// @brief	Construct
//=====================================================================================================================
Dx11Buffer::Dx11Buffer()
{
    ZeroMemory( &BufferDesc, sizeof( D3D11_BUFFER_DESC ) );
}

//=====================================================================================================================
// @brief	Create
//=====================================================================================================================
void Dx11Buffer::Create( const D3D11_SUBRESOURCE_DATA* SubresourceData )
{
    Release();
    
    HRESULT hr = GetDx11Device()->CreateBuffer( &BufferDesc, SubresourceData, BufferComPtr.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        int i = 0;
        return;
    }
}

//=====================================================================================================================
// @brief	Release
//=====================================================================================================================
void Dx11Buffer::Release()
{
    BufferComPtr.Reset();
}