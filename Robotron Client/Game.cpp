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
	
	//delete each mesh in map
	std::map < eEnemyTypes, CMesh* >::iterator meshItter = m_pEnemyMesh->begin();
	std::map < eEnemyTypes, CMesh* >::iterator meshItterEnd = m_pEnemyMesh->end();
	while (meshItter != meshItterEnd)
	{
		delete meshItter->second;
		meshItter->second = 0;

		meshItter++;
	}
	//Delete the mesh map
	m_pEnemyMesh->clear();
	delete m_pEnemyMesh;
	m_pEnemyMesh = 0;

	std::map < ePowerType, CMesh* >::iterator meshpowItter = m_pPowerUpMesh->begin();
	std::map < ePowerType, CMesh* >::iterator meshpowItterEnd = m_pPowerUpMesh->end();
	while (meshpowItter != meshpowItterEnd)
	{
		delete meshpowItter->second;
		meshpowItter->second = 0;

		meshpowItter++;
	}
	//Delete the mesh map
	m_pPowerUpMesh->clear();
	delete m_pPowerUpMesh;
	m_pPowerUpMesh = 0;

	//Delete player list
	std::map < std::string, CPlayerObj* >::iterator itterPlayer = m_plistPlayers->begin();
	while (itterPlayer != m_plistPlayers->end())
	{
		delete itterPlayer->second;
		itterPlayer->second = 0;

		itterPlayer++;
	}
	m_plistPlayers->clear();
	delete m_plistPlayers;
	m_plistPlayers = 0;

	//Delete enemy list
	std::map <UINT, CEnemyObj* >::iterator itterEnemy = m_pListEnemies->begin();
	while (itterEnemy != m_pListEnemies->end())
	{
		delete itterEnemy->second;
		itterEnemy->second = 0;

		itterEnemy++;
	}
	m_pListEnemies->clear();
	delete m_pListEnemies;
	m_pListEnemies = 0;

	//Delete PowerUp list
	std::map <UINT, CPowerUpObj* >::iterator itterPowUp = m_pListPowerUps->begin();
	while (itterPowUp != m_pListPowerUps->end())
	{
		delete itterPowUp->second;
		itterPowUp->second = 0;

		itterPowUp++;
	}
	m_pListPowerUps->clear();
	delete m_pListPowerUps;
	m_pListPowerUps = 0;

	delete m_iEnemyIDs;
	m_iEnemyIDs = 0;

	delete m_iPowerUpIDs;
	m_iPowerUpIDs = 0;

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
		
	m_plistPlayers = new std::map < std::string, CPlayerObj* >;
	m_pListEnemies = new std::map < UINT, CEnemyObj* > ;
	m_pEnemyMesh = new std::map < eEnemyTypes, CMesh* > ;
	m_iEnemyIDs = new std::map < eEnemyTypes, UINT > ;
	m_iPowerUpIDs = new std::map < ePowerType, UINT > ;
	m_pListPowerUps = new std::map < UINT, CPowerUpObj* > ;
	m_pPowerUpMesh = new std::map < ePowerType, CMesh* > ;

	m_pRenderManager = _RenderManager;
		
	//Set up the projection matrix	
	m_pRenderManager->CalculateProjectionMatrix(D3DXToRadian(45), 0.01f, 100000.0f);

	//Create and set up the terrain
	m_pTerrain = new CTerrain();
	ScalarVertex TerrainScalar = { 1.0f, 0.0f, 1.0f };
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

	//Create the lust enemy assets
	m_iEnemyIDs->insert(std::pair<eEnemyTypes, UINT>(ET_LUST, CreateEnemyAssest(ET_LUST)));

	//Create the Shield power up assets
	m_iPowerUpIDs->insert(std::pair<ePowerType, UINT>(PU_SHIELD, CreatePowerUpAssest(PU_SHIELD)));
	
	//Initialise DirectionLight
	D3DLightParameter DirectiomLightParam;
	DirectiomLightParam.eLightType = D3DLIGHT_DIRECTIONAL;
	DirectiomLightParam.colorDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	DirectiomLightParam.vecDirection = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	//Create the directionLight
	m_iDirectionID = m_pRenderManager->CreateLights(DirectiomLightParam);

	//Update the Direction Light
	m_pRenderManager->UpdateDirectionLight(m_iDirectionID, true);

	DirectiomLightParam.colorDiffuse = D3DXCOLOR(1.0f, 2.0f, 8.0f, 1.0f);
	m_iLightningID = m_pRenderManager->CreateLights(DirectiomLightParam);
	
	//Update the Direction Light
	m_pRenderManager->UpdateDirectionLight(m_iLightningID, false);

	m_bLightning = false;

	return true;
		
}

void CGame::UpdatePlayerList(std::vector<std::string> _Players, ClientDataPacket* _pClientPacket)
{
	

	
	
}



void CGame::SpawnWave(ClientDataPacket* _pClientPacket)
{
	//Spawn Enemies
	for (UINT iEnemy = 0; iEnemy < _pClientPacket->iNumEnemies; iEnemy++)
	{

		EnemyStates CurrentEnemyState = _pClientPacket->EnemyInfo[iEnemy];

		std::pair<std::map<UINT, CEnemyObj*>::iterator, bool> MapEnemyIter;

		//Create enemy object
		CEnemyObj* tempEnemy = new CEnemyObj(CurrentEnemyState.Etype);
		//Initialise the enemy Object with the Cube Mesh and set its coordinates
		std::map<eEnemyTypes, CMesh* >::iterator EnemyMesh = m_pEnemyMesh->find(CurrentEnemyState.Etype);
		tempEnemy->Initialise(m_pRenderManager, m_iPlayerMaterialID, EnemyMesh->second, CurrentEnemyState.uiEnemyID, CurrentEnemyState.f3Positions);

		//Add the player to the map
		MapEnemyIter = m_pListEnemies->insert(std::pair<UINT, CEnemyObj*>(CurrentEnemyState.uiEnemyID, tempEnemy));

	}

	//Spawn Power Ups
	for (UINT iPowerUp = 0; iPowerUp < _pClientPacket->iNumPowerUps; iPowerUp++)
	{

		PowerUpStates CurrentPowUpState = _pClientPacket->PowUpInfo[iPowerUp];
		
		std::pair<std::map<UINT, CPowerUpObj*>::iterator, bool> MapEnemyIter;

		//Create enemy object
		CPowerUpObj* tempEnemy = new CPowerUpObj(CurrentPowUpState.Etype);
		//Initialise the enemy Object with the Cube Mesh and set its coordinates
		std::map<ePowerType, CMesh* >::iterator EnemyMesh = m_pPowerUpMesh->find(CurrentPowUpState.Etype);
		tempEnemy->Initialise(m_pRenderManager, m_iPlayerMaterialID, EnemyMesh->second, CurrentPowUpState.uiPowUpID, CurrentPowUpState.f3Positions);

		//Add the player to the map
		MapEnemyIter = m_pListPowerUps->insert(std::pair<UINT, CPowerUpObj*>(CurrentPowUpState.uiPowUpID, tempEnemy));

	}
	
}

void CGame::Process(ClientDataPacket* _pClientPacket)
{
		
	std::map< std::string, CPlayerObj*>::iterator iterPlayer = m_plistPlayers->begin();
	std::map< std::string, CPlayerObj*>::iterator iterPlayerEnd = m_plistPlayers->end();
	int iPlayer = 0;
	while (iterPlayer != iterPlayerEnd)
	{
		//Set player position
		iterPlayer->second->SetPosition(_pClientPacket->PlayerInfo[iPlayer].f3Positions);
		//Set player Direction
		iterPlayer->second->SetDirection(_pClientPacket->PlayerInfo[iPlayer].f3Direction);
		
		if (iterPlayer->second->GetLightRange() > 5.0f)
		{
			iterPlayer->second->SetLightRange(1.0f);
		}
		else
		{
			iterPlayer->second->SetLightRange(iterPlayer->second->GetLightRange() + 0.05f);
		}

		m_pRenderManager->UpdatePointLight(iterPlayer->second->GetLightID(), true, iterPlayer->second->GetPosition(), iterPlayer->second->GetLightRange());
				
		m_pRenderManager->UpdateDirectionLight(m_iLightningID, m_bLightning);
				

		if (iPlayer == m_iIndexOfClientPlayer)
		{
			m_pPlayerAvatar = iterPlayer->second;

		}
		
		iPlayer++;
		iterPlayer++;
	}

	//Process list of Enemies
	std::map<UINT, CEnemyObj*>::iterator enemyIter = m_pListEnemies->begin();
	std::map<UINT, CEnemyObj*>::iterator enemyIterEnd = m_pListEnemies->end();
	int iEnemy = 0;
	while (enemyIter != enemyIterEnd)
	{
		//Set player position
		enemyIter->second->SetPosition(_pClientPacket->EnemyInfo[iEnemy].f3Positions);
		//Set player Direction
		enemyIter->second->SetDirection(_pClientPacket->EnemyInfo[iEnemy].f3Direction);
		
		iEnemy++;
		enemyIter++;
	}
			
	std::map<std::string, CPlayerObj*>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
	m_pPlayerAvatar = playerItter->second;

	D3DXVECTOR3 d3dVPos;
	d3dVPos.x = m_pPlayerAvatar->GetPosition().x;
	d3dVPos.y = m_pPlayerAvatar->GetPosition().y;
	d3dVPos.z = m_pPlayerAvatar->GetPosition().z;
	
	D3DXVECTOR3 d3dVUp;
	d3dVUp.x = m_pPlayerAvatar->GetUpVector().x;
	d3dVUp.y = m_pPlayerAvatar->GetUpVector().y;
	d3dVUp.z = m_pPlayerAvatar->GetUpVector().z;

	//Set the camera
	m_pCamera->SetCamera(d3dVPos, d3dVPos, d3dVUp);
	m_pCamera->Process(m_pRenderManager);
		
}

void CGame::Draw()
{

	//Draw every passed in from the server
	m_pTerrain->Draw(m_pRenderManager);
		
	//Loop through all players
	std::map< std::string, CPlayerObj*>::iterator iterPlayer = m_plistPlayers->begin();
	std::map< std::string, CPlayerObj*>::iterator iterPlayerEnd = m_plistPlayers->end();
	while (iterPlayer != iterPlayerEnd)
	{
		//Draw the player avatar
		iterPlayer->second->Draw();

		iterPlayer++;
	}

	std::map< UINT, CEnemyObj*>::iterator iterEnemy = m_pListEnemies->begin();
	std::map< UINT, CEnemyObj*>::iterator iterEnemyEnd = m_pListEnemies->end();
	while (iterEnemy != iterEnemyEnd)
	{
		//Draw the Enemy avatar
		iterEnemy->second->Draw();

		iterEnemy++;
	}

	std::map< UINT, CPowerUpObj*>::iterator iterPowUP = m_pListPowerUps->begin();
	std::map< UINT, CPowerUpObj*>::iterator iterPowUPEnd = m_pListPowerUps->end();
	while (iterPowUP != iterPowUPEnd)
	{
		//Draw the Power UP avatar
		iterPowUP->second->Draw();

		iterPowUP++;
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
	
	std::pair<std::map<std::string, CPlayerObj*>::iterator, bool> MapClientIter;
		
	//Create player object
	CPlayerObj* tempPlayer = new CPlayerObj(m_pRenderManager);
	//Initialise the player Object with the Cube Mesh and set its coordinates
	tempPlayer->Initialise(m_pRenderManager, m_iPlayerMaterialID, m_pPlayerMesh, tempState.uiPlayerID, tempState.f3Positions);
	
	//Add the player to the map
	std::string strName(tempState.cPlayerName);
	MapClientIter = m_plistPlayers->insert(std::pair<std::string, CPlayerObj*>(strName, tempPlayer));
	
	//If this is the controlling players object save it
	if (tempState.cPlayerName == m_strPlayerName)
	{
		//Find the controlling player in the map
		std::map<std::string, CPlayerObj*>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
		m_pPlayerAvatar = playerItter->second;
	}
	

	//Return the bool part(second)
	//This will hold true if a new element was added 
	//Or false if the element already exists
	return MapClientIter.second;
}

void CGame::AddAllPlayers(ClientDataPacket* _pClientPacket)
{
	for (UINT iPlayer = 0; iPlayer < _pClientPacket->serverInfo.iNumClients; iPlayer++)
	{
		
		PlayerStates CurrentPlayerState = _pClientPacket->PlayerInfo[iPlayer];
		
		std::pair<std::map<std::string, CPlayerObj*>::iterator, bool> MapClientIter;

		//Create player object
		CPlayerObj* tempPlayer = new CPlayerObj(m_pRenderManager);
		//Initialise the player Object with the Cube Mesh and set its coordinates
		tempPlayer->Initialise(m_pRenderManager, m_iPlayerMaterialID, m_pPlayerMesh, CurrentPlayerState.uiPlayerID, CurrentPlayerState.f3Positions);

		//Add the player to the map
		MapClientIter = m_plistPlayers->insert(std::pair<std::string, CPlayerObj*>(CurrentPlayerState.cPlayerName, tempPlayer));

		//If this is the controlling players object save it
		if (CurrentPlayerState.cPlayerName == m_strPlayerName)
		{
			//Find the controlling player in the map
			std::map<std::string, CPlayerObj*>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
			m_pPlayerAvatar = playerItter->second;
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
	Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.fPower = { 1.0f };

	//Get the material Id used to reference the created material
	UINT iMatID = m_pRenderManager->CreateMaterial(Material);
	
	return iMatID;
}

int CGame::CreateEnemyAssest(eEnemyTypes _EnemyType)
{
	std::string strFilePath = "";
	float fEnemySize = 0.0f;
	MaterialValues Material;
	
	switch (_EnemyType)
	{
	case ET_LUST:
	{
		strFilePath = "Assets\\Lust.png";
		fEnemySize = 0.8f;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };

		//Create the Texture for the enemy
		int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
		CMesh* pTempMesh = CreateCubeMesh(fEnemySize, iTextureID);
		//Add mesh to map
		m_pEnemyMesh->insert(std::pair<eEnemyTypes, CMesh* >(ET_LUST, pTempMesh));
 
	}
		break;
	case ET_PRIDE:
		break;
	case ET_WRATH:
		break;
	case ET_GREED:
		break;
	case ET_SLOTH:
		break;
	case ET_ENVY:
		break;
	case ET_GLUTTONY:
		break;
	default:
	{
		strFilePath = "";
		fEnemySize = 0.0f;

		Material.f4Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };
	}
		break;
	}

	//Get the material Id used to reference the created material
	UINT iMatID = m_pRenderManager->CreateMaterial(Material);

	return iMatID;
	
	
	
}

int CGame::CreatePowerUpAssest(ePowerType _Type)
{
	std::string strFilePath = "";
	float fSize = 0.0f;
	MaterialValues Material;

	switch (_Type)
	{
	case PU_SHIELD:
	{
		strFilePath = "Assets\\Wrath.png";
		fSize = 0.5f;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };

		//Create the Texture for the enemy
		int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
		CMesh* pTempMesh = CreateCubeMesh(fSize, iTextureID);
		//Add mesh to map
		m_pPowerUpMesh->insert(std::pair<ePowerType, CMesh* >(PU_SHIELD, pTempMesh));

	}
	
	default:
	{
		strFilePath = "";
		fSize = 0.0f;

		Material.f4Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };
	}
	break;
	}

	//Get the material Id used to reference the created material
	UINT iMatID = m_pRenderManager->CreateMaterial(Material);

	return iMatID;



}