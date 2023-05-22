﻿#include "Dx11Mesh.h"

//=================================================================================================
// @brief	Constructor
//=================================================================================================
Dx11Mesh::Dx11Mesh()
{
}

//=================================================================================================
// @brief	Initialize
//=================================================================================================
void Dx11Mesh::Initialize()
{
    Primitive.Initialize();
    Material .Initialize();
}

//=================================================================================================
// @brief	Render
//=================================================================================================
void Dx11Mesh::Render()
{
    Material .Render();
    Primitive.Render();
}
