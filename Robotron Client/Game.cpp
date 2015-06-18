#include "Game.h"

//Global statics
CGame* CGame::s_pGame = 0;

CGame::CGame()
{
}

CGame::~CGame()
{
	//Delete Terrain
	delete m_pTerrain;
	m_pTerrain = 0;

	//Delete Camera
	delete m_pCamera;
	m_pCamera = 0;

	//delete Mesh
	delete m_pPlayerMesh;
	m_pPlayerMesh = 0;
	
	//Delete player list
	delete m_plistPlayers;
	m_plistPlayers = 0;

	//Delete p[layer avatar
	delete m_pPlayerAvatar;
	m_pPlayerAvatar = 0;
	

}

CGame& CGame::GetInstance()
{
	//Create the game instance if it doesnt exist 
	if (s_pGame == 0)
	{
		s_pGame = new CGame();
	}
	return (*s_pGame);
}

void CGame::DestroyInstance()
{
	delete s_pGame;
	s_pGame = 0;
}

bool CGame::Initialise(IRenderer* _RenderManager, std::vector<PlayerStates> _Players, int _iIndexOfClientPlayer)
{	

	if (_iIndexOfClientPlayer > NetworkValues::MAX_USERS)
	{
		return false;
	}

	m_iNumberPlayers = _Players.size();
	m_iIndexOfClientPlayer = _iIndexOfClientPlayer;
	m_plistPlayers = new std::map < std::string, CPlayerObj >;
	m_pPlayerAvatar = new CPlayerObj();
	m_pRenderManager = _RenderManager;
		
	//Set up the projection matrix	
	m_pRenderManager->CalculateProjectionMatrix(D3DXToRadian(45), 0.01f, 100000.0f);

	//Create and set up the terrain
	m_pTerrain = new CTerrain();
	ScalarVertex TerrainScalar = { 1.0f, 0.05f, 1.0f };
	std::string strImagePath = "Assets\\Heightmap.bmp";
	m_pTerrain->Initialise(m_pRenderManager, strImagePath, TerrainScalar);
	m_pTerrain->SetCenter({ 0, 0, 0 });

	//Create the Camera
	m_pCamera = new CCameraStatic();
	//Get the position of the client player and set the static camera to that position only 100 units above it
	D3DXVECTOR3 D3DPosition = { _Players[_iIndexOfClientPlayer].f3Positions.x, _Players[_iIndexOfClientPlayer].f3Positions.y + 2, _Players[_iIndexOfClientPlayer].f3Positions.z };
	//And then set the look at vector of the camera to look at the client players position
	D3DXVECTOR3 D3DLookAt = { _Players[_iIndexOfClientPlayer].f3Positions.x, _Players[_iIndexOfClientPlayer].f3Positions.y, _Players[_iIndexOfClientPlayer].f3Positions.z };
	m_pCamera->Initialise(D3DPosition, D3DLookAt, false);
	m_pCamera->Process(m_pRenderManager);

	//Create player mesh Mesh

	std::string strFilePath = "Assets\\CompanionCube.png";
	int iTextureID = _RenderManager->CreateTexture(strFilePath);
	m_pPlayerMesh = CreatePlayerMesh(1.0f, iTextureID);
	
	MaterialValues Material;
	Material.f4Ambient	= { 0.0f, 0.0f, 0.0f, 1.0f };
	Material.f4Diffuse	= { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.fPower		= { 1.0f };
	
	for (int iPlayer = 0; iPlayer < m_iNumberPlayers; iPlayer++)
	{
		//Create player object
		CPlayerObj tempAvatar;
		//Initialise the player Object with the Cube Mesh and set its coordinates
		tempAvatar.Initialise(_RenderManager, Material, m_pPlayerMesh, _Players[iPlayer].f3Positions );
	
		std::string strPlayerName(_Players[iPlayer].cPLayerName);

		//Add the player and its object to the list of players
		m_plistPlayers->insert(std::pair<std::string, CPlayerObj>(strPlayerName, tempAvatar));

		//Initialise player object
		if (iPlayer == m_iIndexOfClientPlayer)
		{
			*m_pPlayerAvatar = tempAvatar;
		}

	}

	return true;
		
}

void CGame::Process(ClientDataPacket* _pClientPacket)
{
	
	
	std::map< std::string, CPlayerObj>::iterator iterPlayer = m_plistPlayers->begin();
	std::map< std::string, CPlayerObj>::iterator iterPlayerEnd = m_plistPlayers->end();
	int iPlayer = 0;
	while (iterPlayer != iterPlayerEnd)
	{
		//Draw the player avatar
		iterPlayer->second.SetPosition(_pClientPacket->PlayerInfo[iPlayer].f3Positions);
		
		if (iPlayer == m_iIndexOfClientPlayer)
		{
			*m_pPlayerAvatar = iterPlayer->second;

		}


		iPlayer++;
		iterPlayer++;
	}
	
	

	D3DXVECTOR3 d3dVPos;
	d3dVPos.x = m_pPlayerAvatar->GetPosition().x;
	d3dVPos.y = m_pPlayerAvatar->GetPosition().y;
	d3dVPos.z = m_pPlayerAvatar->GetPosition().z;

	D3DXVECTOR3 d3dVDirection;
	d3dVDirection.x = m_pPlayerAvatar->GetDirection().x;
	d3dVDirection.y = m_pPlayerAvatar->GetDirection().y;
	d3dVDirection.z = m_pPlayerAvatar->GetDirection().z;

	D3DXVECTOR3 d3dVUp;
	d3dVUp.x = m_pPlayerAvatar->GetUpVector().x;
	d3dVUp.y = m_pPlayerAvatar->GetUpVector().y;
	d3dVUp.z = m_pPlayerAvatar->GetUpVector().z;

	//Set the camera
	m_pCamera->SetCamera(d3dVDirection, d3dVPos, d3dVUp);
	m_pCamera->Process(m_pRenderManager);
		
}

void CGame::Draw()
{

	//Draw every passed in from the server
	m_pTerrain->Draw(m_pRenderManager);
		
	//Loop through all players
	std::map< std::string, CPlayerObj>::iterator iterPlayer = m_plistPlayers->begin();
	std::map< std::string, CPlayerObj>::iterator iterPlayerEnd = m_plistPlayers->end();
	while (iterPlayer != iterPlayerEnd)
	{
		//Draw the player avatar
		iterPlayer->second.Draw();

		iterPlayer++;
	}

}


CMesh* CGame::CreatePlayerMesh(float _fCubeSize, int iTextureID)
{
	//Create the Cube mesh
	CMesh* meshCube = new CMesh(m_pRenderManager, _fCubeSize, iTextureID);
	meshCube->SetPrimitiveType(IGPT_TRIANGLELIST);
	
	//Create the cube using Vertex with normals and UV Coords
	
	float fTemp = _fCubeSize / 2;

	//Front Face
	meshCube->AddVertex(CVertexUV(float3(-fTemp, fTemp, -fTemp),	float3(-1.0f, 1.0f, -1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, fTemp, -fTemp),		float3(1.0f, 1.0f, -1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, -fTemp, -fTemp),	float3(1.0f, -1.0f, -1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, -fTemp, -fTemp),	float3(-1.0f, -1.0f, -1.0f),	float2(0.0f, 1.0f)));

	//Left Face
	meshCube->AddVertex(CVertexUV(float3(-fTemp, fTemp, fTemp),		float3(-1.0f, 1.0f, 1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, fTemp, -fTemp),	float3(-1.0f, 1.0f, -1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, -fTemp, -fTemp),	float3(-1.0f, -1.0f, -1.0f),	float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, -fTemp, fTemp),	float3(-1.0f, -1.0f, 1.0f),		float2(0.0f, 1.0f)));

	//Right Face
	meshCube->AddVertex(CVertexUV(float3(fTemp, fTemp, -fTemp),		float3(1.0f, 1.0f, -1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, fTemp, fTemp),		float3(1.0f, 1.0f, 1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, -fTemp, fTemp),		float3(1.0f, -1.0f, 1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, -fTemp, -fTemp),	float3(1.0f, -1.0f, -1.0f),		float2(0.0f, 1.0f)));

	//Back Face
	meshCube->AddVertex(CVertexUV(float3(fTemp, fTemp, fTemp),		float3(1.0f, 1.0f, 1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, fTemp, fTemp),		float3(-1.0f, 1.0f, 1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, -fTemp, fTemp),	float3(-1.0f, -1.0f, 1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, -fTemp, fTemp),		float3(1.0f, -1.0f, 1.0f),		float2(0.0f, 1.0f)));

	//Top Face
	meshCube->AddVertex(CVertexUV(float3(-fTemp, fTemp, fTemp),		float3(-1.0f, 1.0f, 1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, fTemp, fTemp),		float3(1.0f, 1.0f, 1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, fTemp, -fTemp),		float3(1.0f, 1.0f, -1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, fTemp, -fTemp),	float3(-1.0f, 1.0f, -1.0f),		float2(0.0f, 1.0f)));

	//Bottom Face
	meshCube->AddVertex(CVertexUV(float3(-fTemp, -fTemp, -fTemp),	float3(-1.0f, -1.0f, -1.0f),	float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, -fTemp, -fTemp),	float3(1.0f, -1.0f, -1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(fTemp, -fTemp, fTemp),		float3(1.0f, -1.0f, 1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-fTemp, -fTemp, fTemp),	float3(-1.0f, -1.0f, 1.0f),		float2(0.0f, 1.0f)));

	//Add the Indices
	std::vector<int> vecIndices = { 0,1,2,
									0,2,3,
									4,5,6,
									4,6,7,
									8,9,10,
									8,10,11,
									12,13,14,
									12,14,15,
									16,17,18,
									16,18,19,
									20,21,22,
									20,22,23
									};

	//Create the static buffer
	meshCube->SetIndexList(vecIndices);
	meshCube->CreateStaticBuffer();
	//Return the mesh
	return meshCube;
}