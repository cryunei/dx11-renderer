﻿#pragma once


#include "../DxTypes.h"


//=====================================================================================================================
// @brief	WorldMatrix
//=====================================================================================================================
struct WorldMatrix
{
    XMMATRIX worldMat;
};

//=====================================================================================================================
// @brief	ViewProjMatrix
//=====================================================================================================================
struct ViewProjMatrix
{
    XMMATRIX viewMat;
    XMMATRIX projMat;
};

//=====================================================================================================================
// @brief	LightProperty
//=====================================================================================================================
struct LightProperty
{
    XMFLOAT4 diffuseColor;
    XMFLOAT3 lightDirection;
    float    padding;
};