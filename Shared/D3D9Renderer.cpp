//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name : D3D9Renderer.cpp
// Description : Implementation file that holds the functionality of the D3D9Renderer class
// Author :	Jc.Fowles
// Mail :	Jc.Fowles@mediadesign.school.nz
//

//This Includes
#include "D3D9Renderer.h"

int CStaticBuffer::s_iActiveStaticBuffer = 0;

CD3D9Renderer::CD3D9Renderer()
{
	m_bSpecular = true;

	m_pDirect3D = 0;
	m_pDevice = 0;
	m_pvectBuffer = 0;

	//Fonts
	m_pTitleFont = 0;
	m_pMenuFont	= 0;
	m_pListFont = 0;
	
	//Map of Surfaces Setup
	m_pSurfaceMap = 0;
	m_iSurfaceKeyCount = 0;

	//Map of Materials Setup
	m_pMaterialMap = 0;
	m_iMaterialKeyCount = 0;

	//Map of Textures Setup
	m_pTextureMap = 0;
	m_iTextureKeyCount = 0;
}

CD3D9Renderer::~CD3D9Renderer()
{
	//Release the device
	m_pDevice->Release();
	m_pDevice = 0;

	//Release the Direct3D
	m_pDirect3D->Release();
	m_pDirect3D = 0;
}

D3DXMATRIX& CD3D9Renderer::GetProjectionMatrix()
{
	return m_matProjection;
}

D3DXMATRIX& CD3D9Renderer::GetViewMatrix()
{
	return m_matView;
}

D3DXMATRIX& CD3D9Renderer::GetWorldMatrix()
{
	return m_matWorld;
}

bool CD3D9Renderer::Shutdown()
{
	//Release COM objects
	m_pTitleFont->Release();
	m_pTitleFont = 0;
	m_pMenuFont->Release();
	m_pMenuFont = 0;
	m_pListFont->Release();
	m_pListFont = 0;
	
	//Deallocate the Texture map
	if (m_pTextureMap != 0)
	{
		std::map<int, IDirect3DTexture9*>::iterator iterCurrent = m_pTextureMap->begin();
		std::map<int, IDirect3DTexture9*>::iterator iterEnd = m_pTextureMap->end();
		while (iterCurrent != iterEnd)
		{
			iterCurrent->second->Release();
			iterCurrent->second = 0;
			iterCurrent++;
		}
		delete m_pTextureMap;
		m_pTextureMap = 0;
	}

	//Deallocate the Surface map
	if (m_pSurfaceMap != 0)
	{
		std::map<int, IDirect3DSurface9*>::iterator iterCurrent = m_pSurfaceMap->begin();
		std::map<int, IDirect3DSurface9*>::iterator iterEnd = m_pSurfaceMap->end();
		while (iterCurrent != iterEnd)
		{
			iterCurrent->second->Release();
			iterCurrent->second = 0;
			iterCurrent++;
		}
		delete m_pSurfaceMap;
		m_pSurfaceMap = 0;
	}

	//Deallocate the Material map
	m_pMaterialMap->clear();
	delete m_pMaterialMap;
	m_pMaterialMap = 0;

	//Deallocate the Static buffer container
	if (m_pvectBuffer != 0)
	{
		while (m_pvectBuffer->empty() == false)
		{
			delete m_pvectBuffer->back();
			m_pvectBuffer->back() = 0;
			m_pvectBuffer->pop_back();
		}
		delete m_pvectBuffer;
		m_pvectBuffer = 0;
	}



	
	return true;
}

bool CD3D9Renderer::Initialise(int _iWidth, int _iHeight, HWND _hWindow, bool _bFullscreen)
{
	//Initialise the member variables
	m_iScreenWidth = _iWidth;
	m_iScreenHeight = _iHeight;
	m_hWindow = _hWindow;
	m_bFullscreen = _bFullscreen;

	if (!DeviceCreation())
	{
		//Device creation failed
		return false;
	}

	RenderStates();

	SetMaterial();

	//Set Clear color to deep purple
	SetClearColour(1.0f, 20.0f / 255.0f, 147.0f / 255.0f);

	//Create a Container for Static Buffers
	m_pvectBuffer = new std::vector < CStaticBuffer* > ;

	//Create map for all the Surfaces
	m_pSurfaceMap = new std::map < int, IDirect3DSurface9* > ;

	//Create the various fonts to draw text to screen
	CreateTextFont(150, 150 / 2, "Times New Roman", TEXT_TITLE);
	CreateTextFont(70, 20, "Times New Roman", TEXT_MENU);
	CreateTextFont(26, 13, "Times New Roman", TEXT_LIST);

	//TO DO: Lights
	ZeroMemory(&m_pDirectionalLight, sizeof(m_pDirectionalLight));
	m_pDirectionalLight.Type = D3DLIGHT_DIRECTIONAL;
	m_pDirectionalLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pDirectionalLight.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	m_pDevice->SetLight(0, &m_pDirectionalLight);
	m_pDevice->LightEnable(0, true);

	//Create map for all the Materials
	m_pMaterialMap = new std::map < int, D3DMATERIAL9 > ;

	//Create map for all the Textures
	m_pTextureMap = new std::map < int, IDirect3DTexture9* >; 

	return true;
}

bool CD3D9Renderer::DeviceCreation()
{
	//Declare Variables structs
	D3DDISPLAYMODE displayMode;
	D3DCAPS9 caps;
	D3DPRESENT_PARAMETERS d3dpp;

	//Free the memory
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//Create the Direct3D
	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	//Set the Device type
	D3DDEVTYPE D3DevType = D3DDEVTYPE_HAL;

	//Get the default Adapter
	int iAdapter = D3DADAPTER_DEFAULT;

	//Get the default adapters display mode
	m_pDirect3D->GetAdapterDisplayMode(iAdapter, &displayMode);

	//Get the Caps for the device
	m_pDirect3D->GetDeviceCaps(iAdapter, D3DevType, &caps);

	//Determine the Vertex processing Hardware or software
	DWORD dwVertProcessing = 0;

	dwVertProcessing = (caps.VertexProcessingCaps != 0) ? (D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE) : (D3DCREATE_SOFTWARE_VERTEXPROCESSING);

	//Fill out the D3D Present Parameters structure
	if (m_bFullscreen == true)
	{
		d3dpp.FullScreen_RefreshRateInHz = displayMode.RefreshRate;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	//Sets the Surface format based on best available for the device
	D3DFORMAT adapterFormat = displayMode.Format;
	if ((m_pDirect3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DevType, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)) == D3D_OK)
	{
		//Z-Buffer	-> 32-bit
		//Depth		-> 24-bit
		//Stencil	-> 8-bit
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	else if ((m_pDirect3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DevType, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4)) == D3D_OK)
	{
		//Z-Buffer	-> 32-bit
		//Depth		-> 24-bit
		//Stencil	-> 4-bit
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24X4S4;
	}
	else if ((m_pDirect3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DevType, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)) == D3D_OK)
	{
		//Z-Buffer	-> 32-bit
		//Depth		-> 24-bit
		//Stencil	-> 0-bit
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	}
	else if ((m_pDirect3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DevType, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)) == D3D_OK)
	{
		//Z-Buffer	-> 32-bit
		//Depth		-> 0-bit
		//Stencil	-> 0-bit
		d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
	}
	else if ((m_pDirect3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DevType, adapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)) == D3D_OK)
	{
		//Z-Buffer	-> 16-bit
		//Depth		-> 0-bit
		//Stencil	-> 0-bit
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}
	else
	{
		//None of the about, something wrong return false
		return false;
	}

	RECT clientRect;
	GetClientRect(m_hWindow, &clientRect);

	//Set The D3D present parameters
	d3dpp.BackBufferFormat = displayMode.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.hDeviceWindow = m_hWindow;
	d3dpp.BackBufferHeight = clientRect.bottom;
	d3dpp.BackBufferWidth = clientRect.right;
	d3dpp.Windowed = !m_bFullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	//Create the device Using the set device information
	if ( (m_pDirect3D->CreateDevice(iAdapter, D3DevType, m_hWindow, dwVertProcessing, &d3dpp, &m_pDevice)) == D3D_OK)
	{
		//Device creation succeeded
		return true;
	}
	//Device creation failed
	return false;
}

void CD3D9Renderer::RenderStates()
{
	//Set initial Lighting
	//Redundant//m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Set initial FVF
	m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	//Set render state z buffer to true
	//Redundant//m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//Set ambient light to a light gray
	m_pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(40, 40, 40));
	m_color = D3DCOLOR_XRGB(40, 40, 40);
	//Set device to normalize all normals
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CD3D9Renderer::SetAmbient(D3DCOLOR _Color)
{
	m_pDevice->SetRenderState(D3DRS_AMBIENT, _Color);
	m_color = _Color;
}

D3DXVECTOR3 CD3D9Renderer::GetAmbient()
{
	D3DXVECTOR3 vecColor;
	vecColor.x = (float)((m_color >> 16) & 255);  //Red Channel
	vecColor.y = (float)((m_color >> 8) & 255);  //Green Channel
	vecColor.z = (float)((m_color)& 255);		 //Blue Channel
	return vecColor;
}

void CD3D9Renderer::SetSpecularEnable(bool _bEnable)
{
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, _bEnable);
	m_bSpecular = _bEnable;
}

bool CD3D9Renderer::GetSpecularEnable()
{
	return m_bSpecular;
}

void CD3D9Renderer::SetLights(D3DLightParameter _pLightParameter)
{
	//Create the light
	D3DLIGHT9 D3DLight;   
	//Clear memory of light use
	ZeroMemory(&D3DLight, sizeof(D3DLight));
		
	//Set Light parameters
	D3DLight.Type = _pLightParameter.eLightType;
	//All Light Types
	D3DLight.Ambient = _pLightParameter.colorAmbient;
	D3DLight.Diffuse = _pLightParameter.colorDiffuse;
	D3DLight.Specular = _pLightParameter.colorSpecular;

	//Only used in Spot and Directional light
	if (D3DLight.Type != D3DLIGHT_POINT)
	{
		D3DLight.Direction = _pLightParameter.vecDirection;
	}
	
	//Only used in Spot and Point light
	if (D3DLight.Type != D3DLIGHT_DIRECTIONAL)
	{
		D3DLight.Position = _pLightParameter.vecPosition;
		D3DLight.Range = _pLightParameter.fRange;
		D3DLight.Attenuation0 = _pLightParameter.fAttnConstant;
		D3DLight.Attenuation1 = _pLightParameter.fAttnLinear;
		D3DLight.Attenuation2 = _pLightParameter.fAttnExponential;
	}
	
	//Only used on Spot
	if (D3DLight.Type == D3DLIGHT_SPOT)
	{
		D3DLight.Theta = D3DXToRadian(_pLightParameter.fInnerAngle);
		D3DLight.Phi = D3DXToRadian(_pLightParameter.fOuterAngle);
		D3DLight.Falloff = _pLightParameter.fFallOff;
	}
	
	//Set light properties to light iID
	m_pDevice->SetLight((_pLightParameter.iID), &D3DLight);
	//Turn light on or off
	m_pDevice->LightEnable((_pLightParameter.iID), (_pLightParameter.bIsTurnedOn)); 
}

bool CD3D9Renderer::SetMaterial()
{
	D3DMATERIAL9 D3DMaterial;
	ZeroMemory(&D3DMaterial, sizeof(D3DMaterial));
	D3DMaterial.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //Reflect All diffuse light color  
	D3DMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //Reflect All Ambient light color 
	D3DMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //Reflect All Specular light color 
	D3DMaterial.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //Reflect All Emissive light color 
	HRESULT hr = m_pDevice->SetMaterial(&D3DMaterial);

	if (FAILED(hr))
	{
		return false;
	}
	
	return true;
}

bool CD3D9Renderer::SetMaterial(int _materialID)
{
	std::map<int, D3DMATERIAL9>::iterator materialIter = m_pMaterialMap->find(_materialID);

	if (materialIter == m_pMaterialMap->end())
	{
		//Material ID not found in the map
		return false;
	}
	else
	{
		//Set the material on the device
		m_pDevice->SetMaterial(&(materialIter->second));
		return true;
	}
}

bool CD3D9Renderer::SetTexture(int _textureID, int _iStage)
{
	std::map<int, IDirect3DTexture9*>::iterator TexIter = m_pTextureMap->find(_textureID);

	if (TexIter == m_pTextureMap->end())
	{
		//Material ID not found in the map
		return false;
	}
	else
	{
		//Set the material on the device
		m_pDevice->SetTexture(_iStage, (TexIter->second));
		return true;
	}
}

UINT CD3D9Renderer::CreateTexture(std::string _strFileName)
{
	//Create and ready the memory of the D3D Texture
	IDirect3DTexture9* pD3DTexture = 0;
	//ZeroMemory(pD3DTexture, sizeof(*pD3DTexture));

	HRESULT hr = D3DXCreateTextureFromFileA(m_pDevice, _strFileName.c_str(), &pD3DTexture);
	
	if (hr == D3DERR_NOTAVAILABLE)
	{
		int c = 9;
	}
	if (hr == D3DERR_INVALIDCALL)
	{
		int c = 9;
	}
	if (hr == D3DXERR_INVALIDDATA)
	{
		int c = 9;
	}
	if (hr == E_OUTOFMEMORY)
	{
		int c = 9;
	}

	
	if (FAILED(hr))
	{
		bool FailedToLoadTexture = false;
		assert(FailedToLoadTexture);
		return -1;
	}
	
	//Add the material to the map
	m_pTextureMap->insert(std::pair<int, IDirect3DTexture9*>(++m_iTextureKeyCount, pD3DTexture));
	//Return a material ID
	return m_iTextureKeyCount;

}

UINT CD3D9Renderer::CreateMaterial(MaterialValues _materialVal)
{	
	//Create and ready the memory of the D3D material
	D3DMATERIAL9 D3DMaterial;
	ZeroMemory(&D3DMaterial, sizeof(D3DMaterial));

	//Set the ambients
	D3DMaterial.Ambient.r = _materialVal.f4Ambient.r;
	D3DMaterial.Ambient.g = _materialVal.f4Ambient.g;
	D3DMaterial.Ambient.b = _materialVal.f4Ambient.b;
	D3DMaterial.Ambient.a = _materialVal.f4Ambient.a;

	//Set the diffuse
	D3DMaterial.Diffuse.r = _materialVal.f4Diffuse.r;
	D3DMaterial.Diffuse.g = _materialVal.f4Diffuse.g;
	D3DMaterial.Diffuse.b = _materialVal.f4Diffuse.b;
	D3DMaterial.Diffuse.a = _materialVal.f4Diffuse.a;
	
	//Set the Emissive
	D3DMaterial.Emissive.r = _materialVal.f4Emissive.r;
	D3DMaterial.Emissive.g = _materialVal.f4Emissive.g;
	D3DMaterial.Emissive.b = _materialVal.f4Emissive.b;
	D3DMaterial.Emissive.a = _materialVal.f4Emissive.a;

	//Set the Specular
	D3DMaterial.Specular.r = _materialVal.f4Specular.r;
	D3DMaterial.Specular.g = _materialVal.f4Specular.g;
	D3DMaterial.Specular.b = _materialVal.f4Specular.b;
	D3DMaterial.Specular.a = _materialVal.f4Specular.a;

	//Set the power
	D3DMaterial.Power = _materialVal.fPower;

	//Add the material to the map
	m_pMaterialMap->insert(std::pair<int, D3DMATERIAL9>(++m_iMaterialKeyCount, D3DMaterial));
	//Return a material ID
	return m_iMaterialKeyCount;
}

void CD3D9Renderer::RetrieveVertices(std::vector<CVertexUV>* _pVertices, int _iSurfaceID, D3DXIMAGE_INFO& _pImageInfo, ScalarVertex _fScaleValues)
{
	D3DLOCKED_RECT lockRect;
	ZeroMemory(&lockRect, sizeof(D3DLOCKED_RECT));

	//Add surface to map and pair it to a Surface ID
	std::map<int, IDirect3DSurface9*>::iterator iterSurface = m_pSurfaceMap->find(_iSurfaceID);
	IDirect3DSurface9* pSurface = iterSurface->second;

	//Lock the surface
	pSurface->LockRect(&lockRect, 0, D3DLOCK_READONLY);;

	//Calculate number of pixels in the image
	int iNumPixels = _pImageInfo.Width * _pImageInfo.Height;

	//Get bits pointer and cast to D3DCOLOR*
	D3DCOLOR* pPixel = reinterpret_cast<D3DCOLOR*>(lockRect.pBits);

	//Loop through entire surface pixel by pixel
	//Use the pixel information to create the vertex
	//Remembering Image height is the surface width
	//Add Image width is the surface depth
	for (int iCol = 0; iCol < (int)_pImageInfo.Height; iCol++)
	{
		for (int iRow = 0; iRow < (int)_pImageInfo.Width; iRow++)
		{
			//which pixel we currently using
			int iPixelNum = (iCol * _pImageInfo.Width) + iRow;
			//Get the pixel color
			D3DCOLOR PixelColor = pPixel[iPixelNum];
			//Extract only the red channel from the pixel color
			int iRedChannel = (PixelColor >> 16) & 255;
			
			//Scale the vertex to Calculate Vector positions
			float fXPos = iCol * _fScaleValues.fScalarDepth;			
			float fYPos = iRedChannel * _fScaleValues.fScalarHeight;	
			float fZPos = iRow * _fScaleValues.fScalarWidth;			
			
			//Calculate normal vector for Current vertex
			std::vector<D3DVECTOR> vecDirectionVectors;
			D3DVECTOR tempDirectionVector;
						
			if (iCol % 2 == 0)
			//Current in an even column
			{
				//Surrounding Vertices's Pattern
				/*
					  \ | /
					___\|/___
						|
						|
				*/
				//Start from vertex right and go anti clock wise

				if (iCol != (_pImageInfo.Width - 1))
				//vertex is not in the final column
				{
					//Get direction vector to the vertex (iCol + 1) -> Vertex right
					tempDirectionVector.x = 1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[(iRow + ((iCol + 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 0 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}
				
				if ((iCol != (_pImageInfo.Width - 1)) &&
					(iRow != (_pImageInfo.Width - 1)) )
				//vertex is in the final column and final row
				{
					//Get direction vector to the vertex (iCol + 1 , iRow + 1) - > Vertex right and up
					tempDirectionVector.x = 1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow + 1) + ((iCol + 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 1 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}
				
				if (iRow != (_pImageInfo.Width - 1))
				//vertex is not in the final row
				{
					//Get direction vector to the vertex (iRow + 1) - > Vertex up
					tempDirectionVector.x = 0 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow + 1) + (iCol * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 1 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}

				if ((iRow != (_pImageInfo.Width - 1)) &&
					(iCol != 0))
				//vertex is not in the first column
				{
					//Get direction vector to the vertex (iCol - 1, row + 1) - > Vertex left and up
					tempDirectionVector.x = -1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow + 1) + ((iCol - 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 1 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}

				if ((iCol != 0) &&
					(iRow != 0))
				//vertex is in the first column and first row
				{
					//Get direction vector to the vertex (iCol - 1) - > Vertex left
					tempDirectionVector.x = -1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[(iRow + ((iCol - 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 0 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}

				if (iRow != 0)
				//vertex is in the first row
				{
					//Get direction vector to the vertex (iRow - 1) - > Vertex down
					tempDirectionVector.x = 0 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow - 1) + (iCol * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = -1 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}

			}
			else
			//Current in an even column
			{
				//Surrounding Vertices's Pattern
				/*
				    	  |
			 	       ___|___
				    	 /|\
				    	/ | \
				*/
				//Start from vertex right and go anti clock wise 

				if (iCol != (_pImageInfo.Width - 1))
				//vertex is not in the final column
				{
					//Get direction vector to the vertex (iCol + 1) -> Vertex right
					tempDirectionVector.x = 1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[(iRow + ((iCol + 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 0 * _fScaleValues.fScalarDepth;
				
					vecDirectionVectors.push_back(tempDirectionVector);
				}

				if (iRow != (_pImageInfo.Width - 1))
				//vertex is not in the final row
				{
					//Get direction vector to the vertex (iRow + 1) -> Vertex up
					tempDirectionVector.x = 0 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow + 1) + (iCol * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 1 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}

				if ((iCol != 0) &&
					(iRow != 0))
				//vertex is not in the first column and first row
				{
					//Get direction vector to the vertex (iCol - 1) -> Vertex left
					tempDirectionVector.x = -1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[(iRow + ((iCol - 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = 0 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);

					//Get direction vector to the vertex (iRow - 1 , iCol - 1) -> Vertex left and down
					tempDirectionVector.x = -1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow - 1) + ((iCol - 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = -1 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}
				
				if (iRow != 0)
				//vertex is not in the first row
				{
					//Get direction vector to the vertex (iRow - 1) -> Vertex down
					tempDirectionVector.x = 0 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow - 1) + (iCol * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = -1 * _fScaleValues.fScalarDepth;
				
					vecDirectionVectors.push_back(tempDirectionVector);
				}

				if ((iCol != (_pImageInfo.Width - 1)) &&
					(iRow != 0))
				//vertex is not in the final column and first row	
				{
					//Get direction vector to the vertex (iRow - 1,iCol + 1 ) -> Vertex down right
					tempDirectionVector.x = 1 * _fScaleValues.fScalarWidth;
					tempDirectionVector.y = ((((pPixel[((iRow - 1) + ((iCol + 1) * _pImageInfo.Width))]) >> 16) & 255) * _fScaleValues.fScalarHeight) - fYPos;
					tempDirectionVector.z = -1 * _fScaleValues.fScalarDepth;
					
					vecDirectionVectors.push_back(tempDirectionVector);
				}
			}


			D3DXVECTOR3 tempNormalVector = { 0, 0, 0 };
			D3DXVECTOR3 VertexNormal = { 0, 0, 0 };
			
			//Cross product all directional vector to get average normal to all surrounding planes
			for (int i = (int)(vecDirectionVectors.size() - 1); i >= 0; i--)
			{
				//If on last directional vector 
				if (i == 0)
				{
					if (vecDirectionVectors.size() == 6)
					{
						tempNormalVector.x += ((vecDirectionVectors[i].y * vecDirectionVectors[(vecDirectionVectors.size() - 1)].z) - (vecDirectionVectors[i].z * vecDirectionVectors[(vecDirectionVectors.size() - 1)].y));
						tempNormalVector.y += ((vecDirectionVectors[i].z * vecDirectionVectors[(vecDirectionVectors.size() - 1)].x) - (vecDirectionVectors[i].x * vecDirectionVectors[(vecDirectionVectors.size() - 1)].z));
						tempNormalVector.z += ((vecDirectionVectors[i].x * vecDirectionVectors[(vecDirectionVectors.size() - 1)].y) - (vecDirectionVectors[i].y * vecDirectionVectors[(vecDirectionVectors.size() - 1)].x));
					}
				}
				else
				{
					tempNormalVector.x += ((vecDirectionVectors[i].y * vecDirectionVectors[i - 1].z) - (vecDirectionVectors[i].z * vecDirectionVectors[i - 1].y));
					tempNormalVector.y += ((vecDirectionVectors[i].z * vecDirectionVectors[i - 1].x) - (vecDirectionVectors[i].x * vecDirectionVectors[i - 1].z));
					tempNormalVector.z += ((vecDirectionVectors[i].x * vecDirectionVectors[i - 1].y) - (vecDirectionVectors[i].y * vecDirectionVectors[i - 1].x));
				}
			}
			
			//Normalize the calculated normal
			D3DXVec3Normalize(&VertexNormal, &tempNormalVector);
			
			//Create the CVertexUV and and it the vector of Vertices's
			CVertexUV TempVertexNormal(CVertexUV(float3(fXPos, fYPos, fZPos), 
												 float3(VertexNormal.x, VertexNormal.y, VertexNormal.z),
												 float2( ( (float)iRow / _pImageInfo.Width ), ( (float)iCol / _pImageInfo.Height ) ) ) 
									  );
			_pVertices->push_back(TempVertexNormal);
		}
	}

	//Unlock the surface
	pSurface->UnlockRect();
}

void CD3D9Renderer::SetWorldMatrix(D3DXMATRIX& _rWorld)
{
	m_matWorld = _rWorld;
	//Setting the world matrix on the device
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
}

void CD3D9Renderer::SetViewMatrix(D3DXMATRIX& _rView)
{
	m_matView = _rView;
}

void CD3D9Renderer::SetProjectionMatrix(D3DXMATRIX& _rProjection)
{
	m_matProjection = _rProjection;
}

void CD3D9Renderer::SetClearColour(float _fRed, float _fGreen, float _fBlue)
{
	m_ClearColor = D3DCOLOR_COLORVALUE(_fRed, _fGreen, _fBlue, 1.0f);
}

void CD3D9Renderer::Render(unsigned int _uiStaticID)
{
	(*m_pvectBuffer)[_uiStaticID]->Render(m_pDevice);
}

void CD3D9Renderer::Clear(bool _bTarget, bool _bDepth, bool _bStencil)
{
	DWORD dwClearFlags = 0;
		
	if (_bStencil)
	//If clearing a stencil
	{
		dwClearFlags = dwClearFlags | D3DCLEAR_STENCIL;
	}
	
	if (_bTarget)
	//If clearing a target area
	{
		dwClearFlags = dwClearFlags | D3DCLEAR_TARGET;
	}
	
	if (_bDepth)
	//If clearing a the Z Buffer
	{
		dwClearFlags = dwClearFlags | D3DCLEAR_ZBUFFER;
	}
	
	//Clear the current scene, or backbuffer
	m_pDevice->Clear(0, NULL, dwClearFlags, m_ClearColor, 1.0f, 0);
}

void CD3D9Renderer::StartRender(bool _bTarget, bool _bDepth, bool _bStencil)
{
	//Clears the scene, creating a black scene to draw onto
	Clear(_bTarget, _bDepth, _bStencil);
	//Begin the current frame, so things can be drawn to its back buffer
	m_pDevice->BeginScene();
}

void CD3D9Renderer::EndRender()
{
	//End the current frame
	m_pDevice->EndScene();
	//Present the scene to the screen
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void CD3D9Renderer::RenderDebugOutput(std::string _strInfo, int _iXPos, int _iYPos, D3DCOLOR _color)
{//TO DO:
//	//Return the Font Description
//	D3DXFONT_DESCA fontDesc;
//	m_pDebugFont->GetDescA(&fontDesc);
//
//	//Create the text space as a RECT based on the string length and character sizes
//	RECT rect = { _iXPos, _iYPos, _iXPos + (_strInfo.length() * (fontDesc.Width + 3)), (_iYPos + fontDesc.Height + 3) };
//
//	m_pDebugFont->DrawTextA(NULL,				//Not used
//						_strInfo.c_str(),		//The String to draw to the screen
//						-1,						//String is null terminated
//						&rect,					//RECT to draw the text into
//						DT_TOP | DT_LEFT,		//Top justified and left Aligned
//						_color);				//The color of text
}

void CD3D9Renderer::CreateViewMatrix(D3DXVECTOR3 _vPosition, D3DXVECTOR3 _vLookAt, D3DXVECTOR3 _vUp)
{
	//Create and populate the view Matrix
	D3DXMatrixLookAtLH(&m_matView, &_vPosition, &_vLookAt, &_vUp);

	//Setting the View Matrix on the Device
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void CD3D9Renderer::CalculateProjectionMatrix(float _fFov, float _fNear, float _fFar)
{
	//Calculate aspect ratio
	float fAspectRatio = ((float)m_iScreenWidth / (float)m_iScreenHeight);

	//Calculate the Projection matrix of the D3D Device
	D3DXMatrixPerspectiveFovLH( &m_matProjection,
								_fFov,						//The horizontal field of view
								(FLOAT)fAspectRatio,		//Aspect ratio
								_fNear,						//The near view-plane
								_fFar);						//The far view-plane

	//Set the Projection matrix of the D3D Device
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);

}

int CD3D9Renderer::CreateOffScreenSurface(std::string _strFileName, D3DXIMAGE_INFO& _rImageInfo)
{
	//Create a surface pointer
	IDirect3DSurface9* pSurface = 0;

	//Retrieve the Image info
	HRESULT hr = D3DXGetImageInfoFromFileA(_strFileName.c_str(), &_rImageInfo);

	//Create the off screen surface
	m_pDevice->CreateOffscreenPlainSurface(_rImageInfo.Width, _rImageInfo.Height, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, &pSurface, 0);
	HRESULT hResult = D3DXLoadSurfaceFromFileA(pSurface, 0, 0, _strFileName.c_str(), 0, D3DX_FILTER_NONE, 0, &_rImageInfo);

	//If the surface wasn't loaded for any reason return 0 representing failure
	if (hResult != D3D_OK)
	{
		return 0;
	}

	//Add the surface to the map, increment the key
	m_pSurfaceMap->insert(std::pair<int, IDirect3DSurface9*>(++m_iSurfaceKeyCount, pSurface));

	//Return the Key id for this surface;
	return m_iSurfaceKeyCount;
}

int	CD3D9Renderer::CreateStaticBuffer(	VertexType _VertexType,
										eIGPrimitiveType _ePrimitiveType,
										unsigned int _uiTotalVerts,
										unsigned int _uiTotalIndices,
										int _iStride,
										void* _pData,
										eIGIndexType _eIndexType,
										void* _pIndices)
{
	//Set the ID to Invalid
	int iID = g_kiInvalidStaticBuffer;

	//Create and Initialise a new Static Buffer
	CStaticBuffer* pNewBuffer = new CStaticBuffer();
	if (pNewBuffer->Initialise( m_pDevice,
								m_pvectBuffer->size(),
								_VertexType,
								_ePrimitiveType,
								_uiTotalVerts,
								_uiTotalIndices,
								_iStride,
								_pData,
								_eIndexType,
								_pIndices))
	{
		//Add new Buffer to the Static Buffer Container
		m_pvectBuffer->push_back(pNewBuffer);
		iID = pNewBuffer->GetID();
	}
	else
	{
		//Delete allocated memory if the initialization failed
		delete pNewBuffer;
		pNewBuffer = 0;
	}

	//Return the ID of the static buffer
	return iID;
}


void CD3D9Renderer::SetBackgroundColor(DWORD _Color)
{
	IDirect3DSurface9* pBackBuffer;
	m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDevice->ColorFill(pBackBuffer, 0, _Color);
}

void CD3D9Renderer::FillRectColor(DWORD _Color, RECT _rect)
{
	IDirect3DSurface9* pBackBuffer;
	m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pDevice->ColorFill(pBackBuffer, &_rect, _Color);
}

void CD3D9Renderer::CreateTextFont(UINT uiHeight, UINT uiWidth, char* _strFontType, eTextType _textType)
{
	D3DXFONT_DESCA fontDesc;
	//Font attributes
	ZeroMemory(&fontDesc, sizeof(D3DXFONT_DESCA));
	fontDesc.Height = uiHeight;
	fontDesc.Width = uiWidth;
	fontDesc.Weight = 1;
	fontDesc.MipLevels = D3DX_DEFAULT;
	fontDesc.Italic = false;
	fontDesc.CharSet = 0;
	fontDesc.OutputPrecision = 100;
	fontDesc.Quality = 100;
	fontDesc.PitchAndFamily = 0;
	strcpy_s(fontDesc.FaceName, _strFontType);

	//Set the passed in texType font of the Device to the created font
	ID3DXFont* pFont;
	D3DXCreateFontIndirectA(m_pDevice, &fontDesc, &pFont);

	//Save the created font to the correct font pointer
	switch (_textType)
	{
		case TEXT_TITLE:
		{
			m_pTitleFont = pFont;
		}break;
		case TEXT_MENU:
		{
			m_pMenuFont = pFont;
		}break;
		case TEXT_LIST:
		{
			m_pListFont = pFont;
		}
		default:
			break;
	}
}

void CD3D9Renderer::RenderText(std::string _strText, RECT _rect, DWORD _color, eTextType _textType, DWORD _Format)
{
	//Get the font pointer based on text type
	ID3DXFont* pFont = FontSelect(_textType);
	
	//If font pointer exist 
	if (pFont != 0)
	{
		//Return the Font Description
		D3DXFONT_DESCA fontDesc;
		pFont->GetDescA(&fontDesc);

		static_cast<D3DCOLOR>(_color);

		pFont->DrawTextA(NULL,						//Not used
						 _strText.c_str(),			//The String to draw to the screen
						 -1,						//String is null terminated
						 &_rect,					//RECT to draw the text into
						 _Format,					//Text box format
						 _color);					//The color of text

	}
}

int CD3D9Renderer::GetFontHeight(eTextType _textType)
{
	//Get the font pointer based on text type
	ID3DXFont* pFont = FontSelect(_textType);
	
	//If font pointer exist 
	if (pFont != 0)
	{
		//Get the Font info
		D3DXFONT_DESCA fontDesc;
		pFont->GetDescA(&fontDesc);

		//Return the height of the font
		return fontDesc.Height;
	}
	else
	{
		//Incompatible font type return -1 as error
		return -1;
	}

}

int CD3D9Renderer::GetFontWidth(eTextType _textType)
{
	//Get the font pointer based on text type
	ID3DXFont* pFont = FontSelect(_textType);
	
	//If font pointer exist 
	if (pFont != 0)
	{
		//Get the Font info
		D3DXFONT_DESCA fontDesc;
		pFont->GetDescA(&fontDesc);

		//Return the width of the font
		return fontDesc.Width;
	}
	else
	{
		//Incompatible font type return -1 as error
		return -1;
	}

}

ID3DXFont* CD3D9Renderer::FontSelect(eTextType _textType)
{
	ID3DXFont* pFont = 0;
	//Select which font to use based on the type of text
	switch (_textType)
	{
	case TEXT_TITLE:
	{
		pFont = m_pTitleFont;
	}break;

	case TEXT_MENU:
	{
		pFont = m_pMenuFont;
	}break;
	
	case TEXT_LIST:
	{
		pFont = m_pListFont;
	}break;
	default:
		break;
	}

	return pFont;
}