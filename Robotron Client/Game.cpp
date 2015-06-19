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
	delete m_pLustMesh;
	m_pLustMesh = 0;

	//Delete player list
	delete m_plistPlayers;
	m_plistPlayers = 0;

	//Delete enemy list
	delete m_pListEnemies;
	m_pListEnemies = 0;

	//Delete player avatar
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

bool CGame::Initialise(IRenderer* _RenderManager, std::string _ControllingPlayer)
{	

	m_strPlayerName = _ControllingPlayer;
		
	m_plistPlayers = new std::map < std::string, CPlayerObj >;
	m_pListEnemies = new std::map < UINT, CEnemyObj > ;
	
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
	//Initialise the camera position
	D3DXVECTOR3 D3DPosition = { 0.0f, 100.0f, 0.0f };
	//Initialise the Camera target
	D3DXVECTOR3 D3DLookAt = { 0.0f, 0.0f, 0.0f };
	m_pCamera->Initialise(D3DPosition, D3DLookAt, false);
	m_pCamera->Process(m_pRenderManager);
		
	//Create player assets
	m_iPlayerMaterialID = CreatePlayerAssest();

	
	
	//CreateEnemyLust();

	/*std::string strFilePath = "Assets\\Wrath.png";
	iTextureID = _RenderManager->CreateTexture(strFilePath);
	m_pWrathMesh = CreateCubeMesh(1.2f, iTextureID);

	std::string strFilePath = "Assets\\Wrath.png";
	iTextureID = _RenderManager->CreateTexture(strFilePath);
	m_pSlothMesh = CreateCubeMesh(2.2f, iTextureID);*/

		
	

	return true;
		
}

void CGame::UpdatePlayerList(std::vector<std::string> _Players, ClientDataPacket* _pClientPacket)
{
	

	
	
}



void CGame::CreateEnemyLust()
{
	//Create the Texture for the LUST enemy
	std::string strFilePath = "Assets\\Lust.png";
	int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
	m_pLustMesh = CreateCubeMesh(0.8f, iTextureID);

	//Create the material for the LUST enemy
	MaterialValues Material;
	Material.f4Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.fPower = { 1.0f };

	//Get the material Id used to reference the created material
	int iMatID = m_pRenderManager->CreateMaterial(Material);

	//Create Lust enemies
	CEnemyObj enemyLust(ET_LUST);
	//Set the position of LUST
	float3 Pos = { 0.0f, 20.0f, 5.0f };

	//Initialise the enemy Lust with the Cube Mesh and set its coordinates
	enemyLust.Initialise(m_pRenderManager, iMatID, m_pLustMesh, Pos);

	enemyLust.GetObjectID();

	m_pListEnemies->insert(std::pair<UINT, CEnemyObj >(enemyLust.GetObjectID(), enemyLust));

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
	
	std::map<std::string, CPlayerObj>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
	m_pPlayerAvatar = &playerItter->second;

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
	std::map< UINT, CEnemyObj>::iterator iterEnemy = m_pListEnemies->begin();
	std::map< UINT, CEnemyObj>::iterator iterEnemyEnd = m_pListEnemies->end();
	while (iterEnemy != iterEnemyEnd)
	{
		//Draw the player avatar
		iterEnemy->second.Draw();

		iterEnemy++;
	}

}

CMesh* CGame::CreateCubeMesh(float _fCubeSize, int iTextureID)
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

bool CGame::AddPlayer(ClientDataPacket* _pClientPacket, std::string _strPlayerToAdd)
{
	PlayerStates tempState;
	//Find the player to add
	for (UINT iPlayer = 0; iPlayer < NetworkValues::MAX_USERS; iPlayer++)
	{
		std::string strNameCheck(_pClientPacket->serverInfo.activeClientList[iPlayer].cUserName);
		if (_strPlayerToAdd == strNameCheck)
		{
			tempState = _pClientPacket->PlayerInfo[iPlayer];
		}
	}
	
	std::pair<std::map<std::string, CPlayerObj>::iterator, bool> MapClientIter;
		
	//Create player object
	CPlayerObj tempPlayer;
	//Initialise the player Object with the Cube Mesh and set its coordinates
	tempPlayer.Initialise(m_pRenderManager, m_iPlayerMaterialID, m_pPlayerMesh, tempState.f3Positions);

	//Add the player to the map
	std::string strName(tempState.cPlayerName);
	MapClientIter = m_plistPlayers->insert(std::pair<std::string, CPlayerObj>(strName, tempPlayer));
	
	//If this is the controlling players object save it
	if (tempState.cPlayerName == m_strPlayerName)
	{
		//Find the controlling player in the map
		std::map<std::string, CPlayerObj>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
		m_pPlayerAvatar = &playerItter->second;
	}

	//Return the bool part(second)
	//This will hold true if a new element was added 
	//Or false if the element already exists
	return MapClientIter.second;
}

void CGame::AddAllPlayers(ClientDataPacket* _pClientPacket)
{
	for (int iPlayer = 0; iPlayer < _pClientPacket->serverInfo.iNumClients; iPlayer++)
	{
		
		PlayerStates CurrentPlayerState = _pClientPacket->PlayerInfo[iPlayer];
		
		std::pair<std::map<std::string, CPlayerObj>::iterator, bool> MapClientIter;

		//Create player object
		CPlayerObj tempPlayer;
		//Initialise the player Object with the Cube Mesh and set its coordinates
		tempPlayer.Initialise(m_pRenderManager, m_iPlayerMaterialID, m_pPlayerMesh, CurrentPlayerState.f3Positions);

		//Add the player to the map
		MapClientIter = m_plistPlayers->insert(std::pair<std::string, CPlayerObj>(CurrentPlayerState.cPlayerName, tempPlayer));

		//If this is the controlling players object save it
		if (CurrentPlayerState.cPlayerName == m_strPlayerName)
		{
			//Find the controlling player in the map
			std::map<std::string, CPlayerObj>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
			m_pPlayerAvatar = &playerItter->second;
		}
	}
}

void CGame::RemovePlayer(std::string _strLeftPlayer)
{
	m_plistPlayers->erase(_strLeftPlayer);
}

int CGame::CreatePlayerAssest()
{
	//Create the texture for the Player object
	std::string strFilePath = "Assets\\CompanionCube.png";
	int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
	m_pPlayerMesh = CreateCubeMesh(1.0f, iTextureID);

	//Create the material for the players object
	MaterialValues Material;
	Material.f4Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.fPower = { 1.0f };

	//Get the material Id used to reference the created material
	int iMatID = m_pRenderManager->CreateMaterial(Material);

	return iMatID;
}