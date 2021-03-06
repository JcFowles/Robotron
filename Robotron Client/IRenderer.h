/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c)2005 - 2015 Media Design School
*
* File Name : IRenderer.h
* Description : Header for the IRenderer interface class
* Author : Jc Fowles
* Mail : Jc.Fowles@mediadesign.school.nz
*/
#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

// Local Includes
#include "..//Shared//Graphics_Defines.h"

// Prototypes
class CVertexUV;
class IRenderer
{
	// Member Functions
public:
	IRenderer()
	{
	}

	virtual ~IRenderer()
	{
	}
	
	virtual bool Initialise(int _iWidth, int _iHeight, HWND _hWindow, bool _bFullscreen) = 0;
	virtual bool Shutdown() = 0;

	//Getters
	virtual D3DXMATRIX& GetProjectionMatrix() = 0;
	virtual D3DXMATRIX& GetViewMatrix() = 0;
	virtual D3DXMATRIX& GetWorldMatrix() = 0;

	virtual void RetrieveVertices(std::vector<CVertexUV>* _pVertices, int _iSurfaceID, D3DXIMAGE_INFO& _pImageInfo, ScalarVertex _fScaleValues, int _iUVCoordTiled) = 0;

	virtual bool GetSpecularEnable() = 0;
	virtual D3DXVECTOR3 GetAmbient() = 0;

	//Font Sizes
	virtual int GetFontHeight(eTextType _textType) = 0;
	virtual int GetFontWidth(eTextType _textType) = 0;


	virtual void FillRectColor(DWORD _Color, RECT _rect) = 0;
	//Setters
	virtual void SetWorldMatrix(D3DXMATRIX& _rWorld) = 0;
	virtual void SetViewMatrix(D3DXMATRIX& _rView) = 0;
	virtual void SetProjectionMatrix(D3DXMATRIX& _rProjection) = 0;
	virtual void SetClearColour(float _fRed, float _fGreen, float _fBlue) = 0;

	
	virtual void SetSpecularEnable(bool _bEnable) = 0;
	virtual void SetAmbient(D3DCOLOR _Color) = 0;

	virtual bool SetMaterial() = 0;
	virtual bool SetMaterial(int _materialID) = 0;
	virtual bool SetTexture(int _textureID, int _iStage) = 0;

	virtual bool UpdatePointLight(int _LightID, bool bIsTurnedOn, float3 _f3Pos, float _fRange) = 0;
	virtual bool UpdateDirectionLight(int _LightID, bool bIsTurnedOn) = 0;
	virtual bool UpdateSpotLight(int _iSpotLightID, bool _bLightOn, float3 f3Position, float3 f3Direction) = 0;

	//Render
	virtual void Render(unsigned int _uiStaticId) = 0;
	virtual void Clear(bool _bTarget, bool _bDepth, bool _bStencil) = 0;
	virtual void StartRender(bool _bTarget, bool _bDepth, bool _bStencil) = 0;
	virtual void EndRender() = 0;
	virtual void RenderDebugOutput(std::string _strInfo, int _iXPos, int _iYPos, D3DCOLOR _color) = 0;;
	
	virtual void RenderText(std::string _strText, RECT _rect, DWORD _color, eTextType _textType, DWORD _Format) = 0;
	virtual void EnableAlphaBlend(bool _bEnable) = 0;;
	virtual void SetBackgroundColor(DWORD _Color) = 0;

	//Matrices
	virtual void CreateViewMatrix(D3DXVECTOR3 _vPosition, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp) = 0;
	virtual void CalculateProjectionMatrix(float _fFov, float _fNear, float _fFar) = 0;
	
	//Creators
	virtual UINT CreateMaterial(MaterialValues _materialVal) = 0;
	virtual UINT CreateTexture(std::string _strFileName) = 0;
	virtual UINT CreateLights(D3DLightParameter _pLightParameter) = 0;

	virtual int CreateOffScreenSurface(std::string _strFileName, D3DXIMAGE_INFO& _rImageInfo) = 0;
	virtual void CreateTextFont(UINT uiHeight, UINT uiWidth, char* _strFontType, eTextType _textType) = 0;
	virtual int CreateStaticBuffer( VertexType _VertexType,
									eIGPrimitiveType _ePrimitiveType,
									unsigned int _uiTotalVerts,
									unsigned int _uiTotalIndices,
									int _iStride,
									void* _pData,
									eIGIndexType _eIndexType,
									void* _pIndices
									) = 0;
	
	
	virtual void ToggleWireFrame(bool _bWireFrame) = 0;
	
private:
	//Disallow copies
	IRenderer(const IRenderer& _kr);
	IRenderer& operator= (const IRenderer& _kr);

	virtual bool DeviceCreation() = 0;
	virtual void SetStates() = 0;

	
	


};


#endif //__RENDERER_H__
