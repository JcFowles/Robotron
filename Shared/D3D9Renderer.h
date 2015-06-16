//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CD3D9Renderer.h 
// Description : Header for the CD3D9Renderer class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

#ifndef __CD3D9RENDERER_H__
#define __CD3D9RENDERER_H__

//Library Includes
#include <map>

//Local Includes
#include "IRenderer.h"
#include "StaticBuffer.h"

//Prototypes
class CD3D9Renderer : public IRenderer
{
	//Member Functions
public:
	/***********************
	* CD3D9Renderer: Default Constructor for D3D9Renderer
	* @author: Jc Fowles
	* @return:
	********************/
	CD3D9Renderer();

	/***********************
	* ~CD3D9Renderer: Default Destructor for D3D9Renderer
	* @author: Jc Fowles
	* @return:
	********************/
	virtual ~CD3D9Renderer();
	
	/***********************
	* Shutdown: Shut down the device
	* @author: Jc Fowles
	* @parameter: _iWidth: The width of the screen
	* @parameter: _iHeight: The height of the screen
	* @parameter: _hWindow: Handle to the Window
	* @parameter: _bFullscreen: Whether the screen is full screen or not
	* @return: bool: Successful Initialization or not
	********************/
	virtual bool Shutdown();

	/***********************
	* Initialise: Initialise the D3D9 Renderer
	* @author: Jc Fowles
	* @parameter: _iWidth: The width of the screen
	* @parameter: _iHeight: The height of the screen
	* @parameter: _hWindow: Handle to the Window
	* @parameter: _bFullscreen: Whether the screen is full screen or not
	* @return: bool: Successful Initialization or not
	********************/
	virtual bool Initialise(int _iWidth, int _iHeight, HWND _hWindow, bool _bFullscreen);


	//Getters
	virtual D3DXMATRIX& GetProjectionMatrix();
	virtual D3DXMATRIX& GetViewMatrix();
	virtual D3DXMATRIX& GetWorldMatrix();

	virtual void RetrieveVertices(std::vector<CVertexUV>* _pVertices, int _iSurfaceID, D3DXIMAGE_INFO& _pImageInfo, ScalarVertex _fScaleValues);

	virtual bool GetSpecularEnable();
	virtual D3DXVECTOR3 GetAmbient();
	
	//Font Sizes
	virtual int GetFontHeight(eTextType _textType);
	virtual int GetFontWidth(eTextType _textType);

	//Setters
	virtual void SetWorldMatrix(D3DXMATRIX& _rWorld);
	virtual void SetViewMatrix(D3DXMATRIX& _rView);
	virtual void SetProjectionMatrix(D3DXMATRIX& _rProjection);
	virtual void SetClearColour(float _fRed, float _fGreen, float _fBlue);

	virtual void SetLights(D3DLightParameter _pLightParameter);
	virtual bool SetMaterial();
	virtual bool SetMaterial(int _materialID);
	virtual bool SetTexture(int _textureID, int _iStage);
	
	virtual void SetSpecularEnable(bool _bEnable);
	virtual void SetAmbient(D3DCOLOR _Color);

	

	//Render
	virtual void Render(unsigned int _uiStaticId);
	virtual void Clear(bool _bTarget, bool _bDepth, bool _bStencil);
	virtual void StartRender(bool _bTarget, bool _bDepth, bool _bStencil);
	virtual void EndRender();

	virtual void RenderDebugOutput(std::string _strInfo,int _iXPos, int _iYPos, D3DCOLOR _color);

	virtual void RenderText(std::string _strText, RECT _rect, DWORD _color, eTextType _textType, DWORD _Format);
	
	virtual void CD3D9Renderer::SetBackgroundColor(DWORD _Color);

	//Matrices
	virtual void CreateViewMatrix(D3DXVECTOR3 _vPosition, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp);
	virtual void CalculateProjectionMatrix(float _fFov, float _fNear, float _fFar);
	
	//Creators
	virtual int CreateOffScreenSurface(std::string _strFileName, D3DXIMAGE_INFO& _rImageInfo);
	virtual UINT CreateMaterial(MaterialValues _materialVal);
	virtual UINT CreateTexture(std::string _strFileName);
	virtual void FillRectColor(DWORD _Color, RECT _rect);
	virtual void CreateTextFont(UINT uiHeight, UINT uiWidth, char* _strFontType, eTextType _textType);
	virtual int CreateStaticBuffer(	VertexType _VertexType,
									eIGPrimitiveType _ePrimitiveType,
									unsigned int _uiTotalVerts,
									unsigned int _uiTotalIndices,
									int _iStride,
									void* _pData,
									eIGIndexType _eIndexType,
									void* _pIndices
									);

	
	
private:
	
	virtual bool DeviceCreation();
	
	/***********************
	* SetRenderStates: Set the render states for the device
	* @author: Jc Fowles
	* @return: void
	********************/
	virtual void SetRenderStates();
		
	/***********************
	* FontSelect: Returns a D3D Font pointer of the selected text type
	* @author: Jc Fowles
	* @Parameter: eTextType _textType: Selected text type
	* @return: ID3DXFont*: The D3D Font pointer of the selected text type
	********************/
	ID3DXFont* FontSelect(eTextType _textType);
	
	
	
	
	//Member Variables
protected:

	HWND m_hWindow;
	IDirect3D9* m_pDirect3D;
	IDirect3DDevice9* m_pDevice;
	
	D3DCOLOR m_ClearColor;

	int m_iScreenWidth;
	int m_iScreenHeight;

	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProjection;

	bool m_bFullscreen;
	bool m_bRenderingScene;

	std::vector< CStaticBuffer* >* m_pvectBuffer;

	std::map<int, IDirect3DSurface9*>* m_pSurfaceMap;
	std::map<int, D3DMATERIAL9>* m_pMaterialMap;
	std::map<int, IDirect3DTexture9*>* m_pTextureMap;

	UINT m_iSurfaceKeyCount;
	UINT m_iMaterialKeyCount;
	UINT m_iTextureKeyCount;

	ID3DXFont* m_pTitleFont;
	ID3DXFont* m_pMenuFont;
	ID3DXFont* m_pListFont;
	

	D3DLIGHT9 m_pDirectionalLight;

	bool m_bSpecular;

	D3DCOLOR m_color;

	
};

#endif //__CD3D9RENDERER_H__