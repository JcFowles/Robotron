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
	
	//Getters

	/***********************
	* GetProjectionMatrix: Retrieves the location of the Projection Matrix stored in memory
	* @author: Jc Fowles
	* @return: D3DXMATRIX&: Memory address of the Devices Projection Matrix
	********************/
	virtual D3DXMATRIX& GetProjectionMatrix();

	/***********************
	* GetViewMatrix: Retrieves the location of the View Matrix stored in memory
	* @author: Jc Fowles
	* @return: D3DXMATRIX&: Memory address of the Devices View Matrix
	********************/
	virtual D3DXMATRIX& GetViewMatrix();

	/***********************
	* GetWorldMatrix: Retrieves the location of the World Matrix stored in memory
	* @author: Jc Fowles
	* @return: D3DXMATRIX&: Memory address of the Devices World Matrix
	********************/
	virtual D3DXMATRIX& GetWorldMatrix();
		
	/***********************
	* GetAmbient: Get Ambient color
	* @author: Jc Fowles
	* @parameter: _Enable: boolean value to set specular to
	* @return: void
	********************/
	virtual D3DXVECTOR3 GetAmbient();

	/***********************
	* GetSpecularEnable: Get Specular Enable
	* @author: Jc Fowles
	* @return: void
	********************/
	virtual bool GetSpecularEnable();

	/***********************
	* GetFontHeight: Return the Height of the font
	* @author: Jc Fowles
	* @parameter: eTextType _textType: Height of this text type is returned
	* @return: int: The height of the font
	********************/
	virtual int GetFontHeight(eTextType _textType);

	/***********************
	* GetFontWidth: Return the Width of the font
	* @author: Jc Fowles
	* @parameter: eTextType _textType: Width of this text type is returned
	* @return: int: The Width of the font
	********************/
	virtual int GetFontWidth(eTextType _textType);

	//Setters

	/***********************
	* SetWorldMatrix: Sets the world matrix on the Render
	* @author: Jc Fowles
	* @parameter: _rWorld: Memory address of the world Matrix to be set too
	* @return: void:
	********************/
	virtual void SetWorldMatrix(D3DXMATRIX& _rWorld);

	/***********************
	* SetViewMatrix: Sets the View Matrix of the Render
	* @author: Jc Fowles
	* @parameter: _rView:  Memory address of the View Matrix to be set too
	* @return: void:
	********************/
	virtual void SetViewMatrix(D3DXMATRIX& _rView);
	
	/***********************
	* SetProjectionMatrix: Sets the Projection Matrix of the Render
	* @author: Jc Fowles
	* @parameter: _rProjection:  Memory address of the Projection Matrix to be set too
	* @return: void:
	********************/
	virtual void SetProjectionMatrix(D3DXMATRIX& _rProjection);
	
	/***********************
	* SetClearColour: Initialise the D3D9 Renderer
	* @author: Jc Fowles
	* @parameter: _fRed: The Red value for the clear color
	* @parameter: _fGreen: The Green value for the clear color
	* @parameter: _fBlue: The Blue value for the clear color
	* @return: void
	********************/
	virtual void SetClearColour(float _fRed, float _fGreen, float _fBlue);

	/***********************
	* SetMaterial: Sets the default material to reflect all types and color lights
	* @author: Jc Fowles
	* @return: bool: return success
	********************/
	virtual bool SetMaterial();
	
	/***********************
	* SetMaterial: Sets the material to be used by the renderer
	* @author: Jc Fowles
	* @parameter: int _materialID: The ID of the material to set on the device
	* @return: bool: return success
	********************/
	virtual bool SetMaterial(int _materialID);
	
	/***********************
	* SetTexture: Sets the texture to be applied and at what stage it should be applied
	* @author: Jc Fowles
	* @parameter: int _textureID: The ID of the texture to set on the device
	* @parameter: int _iStage: The texture stage to be applied
	* @return: bool: return success
	********************/
	virtual bool SetTexture(int _textureID, int _iStage);

	/***********************
	* SetAmbient: Set Ambient color
	* @author: Jc Fowles
	* @parameter: _Enable: boolean value to set specular to
	* @return: void
	********************/
	virtual void SetAmbient(D3DCOLOR _Color);

	/***********************
	* SetSpecularEnable: Set Specular Enable
	* @author: Jc Fowles
	* @parameter: _Enable: boolean value to set specular to
	* @return: void
	********************/
	virtual void SetSpecularEnable(bool _bEnable);
		
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
	
	/***********************
	* RetrieveVertices: Retrieve the Vertices's with normals from the Surface of the ID given
	* @author: Jc Fowles
	* @parameter: _pVertices: Vector of all the vertices's with normals and UV Coords
	* @parameter: _iSurfaceID: ID of the surface to render
	* @parameter: _pImageInfo: Information about the image
	* @parameter: _fScaleValues: Width, Height and Depth Scale values
	* @return: void
	********************/
	virtual void RetrieveVertices(std::vector<CVertexUV>* _pVertices, int _iSurfaceID, D3DXIMAGE_INFO& _pImageInfo, ScalarVertex _fScaleValues);

	/***********************
	* Render: Render a specific Static buffer, based on the passed in ID
	* @author: Jc Fowles
	* @parameter: _uiStaticId: ID To which buffer to render
	* @return: void:
	********************/
	virtual void Render(unsigned int _uiStaticId);

	/***********************
	* Clear: Clears the device
	* @author: Jc Fowles
	* @parameter: _bTarget: Whether to clear the target area
	* @parameter: _bDepth: Whether to clear the Z buffer
	* @parameter: _bStencil: Whether to clear the stencil
	* @return: void
	********************/
	virtual void Clear(bool _bTarget, bool _bDepth, bool _bStencil);

	/***********************
	* StartRender: Calls the device BeginScene to start the current frame
	* @author: Jc Fowles
	* @parameter: _bTarget: Whether to clear the target area
	* @parameter: _bDepth: Whether to clear the Z buffer
	* @parameter: _bStencil: Whether to clear the stencil
	* @return: void
	********************/
	virtual void StartRender(bool _bTarget, bool _bDepth, bool _bStencil);
	
	/***********************
	* EndRender: Calls the device EndScence to end the current frame
	* @author: Jc Fowles
	* @return: void
	********************/
	virtual void EndRender();

	/***********************
	* RenderDebugOuput: Render Text to the screen for debug purposes
	* @author: Jc Fowles
	* @parameter: _strInfo: String to be rendered on screen
	* @parameter: _iYPos: Y position of the text in screen coordinates
	* @parameter: _color: Color to make the Text
	* @return: void
	********************/
	virtual void RenderDebugOutput(std::string _strInfo,int _iXPos, int _iYPos, D3DCOLOR _color);

	/***********************
	* RenderText: Render Text to the screen
	* @author: Jc Fowles
	* @parameter: _strText: Text to be rendered on screen
	* @parameter: _rect: Rectangle to draw text in
	* @parameter: DWORD _color: Color to make the Text
	* @parameter: eTextType _textType: Text type used to select the correct font style
	* @parameter: DWORD _Format: Text format
	* @return: void
	********************/
	virtual void RenderText(std::string _strText, RECT _rect, DWORD _color, eTextType _textType, DWORD _Format);
	
	/***********************
	* SetBackgroundColor: Create a Static Buffer
	* @author: Jc Fowles
	* @parameter: _Color: D3DCOLOR you which to set the back buffer to
	* @return: int: ID of the newly created Static Buffer
	********************/
	virtual void SetBackgroundColor(DWORD _Color);
	
	/***********************
	* CalculateProjectionMatrix: Calculates the the projection Matrix
	* @author: Jc Fowles
	* @parameter: _fFov: The Field of view in radians
	* @parameter: _fNear: The closest the projection can be
	* @parameter: _fFar: The Furtherest the projection can be
	* @return: void:
	********************/
	virtual void CalculateProjectionMatrix(float _fFov, float _fNear, float _fFar);
	
	/***********************
	* CreateViewMatrix: Creates the view Matrix
	* @author: Jc Fowles
	* @parameter: _vPosition: View Position
	* @parameter: _vLookAt: View Looking at the Origin (Origin is center of the screen)
	* @parameter: _vUp: View Direction up vector
	* @return: void:
	********************/
	virtual void CreateViewMatrix(D3DXVECTOR3 _vPosition, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp);
		
	/***********************
	* CreateOffScreenSurface: Create an off screen surface
	* @author: Jc Fowles
	* @parameter: _strFileName: The file name of the image we wish to create the surface from
	* @return: D3DXMATRIX&: Memory address of the Devices World Matrix
	********************/
	virtual int CreateOffScreenSurface(std::string _strFileName, D3DXIMAGE_INFO& _rImageInfo);
	
	/***********************
	* CreateMaterial: Create a material based on passed in values
	* @author: Jc Fowles
	* @parameter: MaterialValues _materialVal: Material values to be used for creating the material
	* @return: UINT: a unique ID that is used to reference the created material
	********************/
	virtual UINT CreateMaterial(MaterialValues _materialVal);

	/***********************
	* CreateTexture: Create a Texture from the passed in file path 
	* @author: Jc Fowles
	* @parameter: std::string _strFileName: File path where texture is found
	* @return: UINT: a unique ID that is used to reference the created texture
	********************/
	virtual UINT CreateTexture(std::string _strFileName);
		
	/***********************
	* SetLight: Set Light properties of the device
	* @author: Jc Fowles
	* @parameter: _pLightParameter: the light parameter to create the lights
	* @return: UINT: a unique ID that is used to reference the created lights
	********************/
	virtual UINT CreateLights(D3DLightParameter _pLightParameter);
		
	/***********************
	* UpdatePointLight: Update the pulsating point light around the players
	* @author: Jc Fowles
	* @Parameter: int _LightID: Id to which light to update
	* @Parameter: bool bIsTurnedOn: value to turn on or off the light
	* @Parameter: float3 _f3Pos: position of the light
	* @Parameter: float _fRange: The range that the point light spreads out to
	* @return: bool: Successful Update
	********************/
	virtual bool UpdatePointLight(int _LightID, bool bIsTurnedOn, float3 _f3Pos, float _fRange);
		
	/***********************
	* UpdateDirectionLight: Update the directional light used for the lighting effect
	* @author: Jc Fowles
	* @Parameter: int _LightID: Id to which light to update
	* @Parameter: bool bIsTurnedOn: value to turn on or off the light
	* @return: bool: 
	********************/
	virtual bool UpdateDirectionLight(int _LightID, bool bIsTurnedOn);
	
	/***********************
	* FillRectColor: Fill a rectangle with a passed in color
	* @author: Jc Fowles
	* @parameter: DWORD _Color: Color value to fill the rect with
	* @parameter: RECT _rect: Rectangle to be filled with color
	* @return: void:
	********************/
	virtual void FillRectColor(DWORD _Color, RECT _rect);
	
	/***********************
	* CreateTextFont: Create the text font to be rendered to the screen
	* @author: Jc Fowles
	* @param: _pFont: pointer to the created font
	* @param: uiHeight: Height of the font to be created
	* @param: uiWidth: Width of the font to be created
	* @param: _strFontType: Type of the font to be created
	* @param: _textType: Type of font to create, and where to save it
	* @return: void
	********************/
	virtual void CreateTextFont(UINT uiHeight, UINT uiWidth, char* _strFontType, eTextType _textType);
	
	/***********************
	* CreateStaticBuffer: Create a Static Buffer
	* @author: Jc Fowles
	* parameter: _VertexType: Long number to define the Vertex Type
	* @parameter: _ePrimitiveType: The Primitive Type used
	* @parameter: _uiTotalVerts: The Total number of Verts in the Buffer
	* @parameter: _uiTotalIndices: The Total number of Indices in the Buffer
	* @parameter: _iStride: The size of the Vertex
	* parameter: _pData: Pointer to data to be assigned to the Vertex Buffer
	* @parameter: _eIndexType: Index type used
	* parameter: _pIndices: Pointer to data to be assigned to the Index Buffer
	* @return: int: ID of the newly created Static Buffer
	********************/
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
	
	/***********************
	* DeviceCreation: Initialize the device present parameters, and creates the device
	* @author: Jc Fowles
	* @return: bool: Successful Creation or not
	********************/
	virtual bool DeviceCreation();
	
	/***********************
	* SetRenderStates: Set the render states for the device
	* @author: Jc Fowles
	* @return: void
	********************/
	virtual void RenderStates();
		
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
	UINT m_iLightKeyCount;

	ID3DXFont* m_pTitleFont;
	ID3DXFont* m_pMenuFont;
	ID3DXFont* m_pListFont;

	std::map<UINT, D3DLIGHT9*>* m_pLightMap;

	bool m_bSpecular;

	D3DCOLOR m_color;

};

#endif //__CD3D9RENDERER_H__