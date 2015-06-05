//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : IRenderer.h 
// Description : Header for the IRenderer interface class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#if !defined(__RENDERER_H__)
#define __RENDERER_H__

// Local Includes
#include "../Shared/Graphics_Defines.h"

// Prototypes
class CVertexColor;
class CVertexNormal;
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

	virtual void RetrieveVertices(std::vector<CVertexColor>* _pVertices, int _iSurfaceID, D3DXIMAGE_INFO& _pImageInfo, ScalarVertex _fScaleValues) = 0;
	virtual void RetrieveVertices(std::vector<CVertexNormal>* _pVertices, int _iSurfaceID, D3DXIMAGE_INFO& _pImageInfo, ScalarVertex _fScaleValues) = 0;

	virtual bool GetSpecularEnable() = 0;
	virtual D3DXVECTOR3 GetAmbient() = 0;

	//Font Sizes
	virtual int GetFontHeight(eTextType _textType) = 0;
	virtual int GetFontWidth(eTextType _textType) = 0;
		
	//Setters
	virtual void SetWorldMatrix(D3DXMATRIX& _rWorld) = 0;
	virtual void SetViewMatrix(D3DXMATRIX& _rView) = 0;
	virtual void SetProjectionMatrix(D3DXMATRIX& _rProjection) = 0;
	virtual void SetClearColour(float _fRed, float _fGreen, float _fBlue) = 0;

	virtual void SetLights(D3DLightParameter _pLightParameter) = 0;

	virtual void SetSpecularEnable(bool _bEnable) = 0;
	virtual void SetAmbient(D3DCOLOR _Color) = 0;

	virtual void SetMaterial() = 0;;

	//Render
	virtual void Render(unsigned int _uiStaticId) = 0;
	virtual void Clear(bool _bTarget, bool _bDepth, bool _bStencil) = 0;
	virtual void StartRender(bool _bTarget, bool _bDepth, bool _bStencil) = 0;
	virtual void EndRender() = 0;
	virtual void RenderDebugOutput(std::string _strInfo, int _iXPos, int _iYPos, D3DCOLOR _color) = 0;;
	
	virtual void RenderText(std::string _strText, RECT _rect, DWORD _color, eTextType _textType) = 0;

	virtual void SetBackgroundColor(DWORD _Color) = 0;

	//Matrices
	virtual void CreateViewMatrix(D3DXVECTOR3 _vPosition, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp) = 0;
	virtual void CalculateProjectionMatrix(float _fFov, float _fNear, float _fFar) = 0;
	virtual void CalculateOrthogonalMatrix(float _fNear, float _fFar) = 0;

	//Creators
	virtual int CreateOffScreenSurface(std::string _strFileName, D3DXIMAGE_INFO& _rImageInfo) = 0;
	//virtual void CreateTextFont() = 0;
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
	
	
private:
	//Disallow copies
	IRenderer(const IRenderer& _kr);
	IRenderer& operator= (const IRenderer& _kr);

	virtual bool DeviceCreation() = 0;
	virtual void SetRenderStates() = 0;
};


#endif //__RENDERER_H__
