#include "Game.h"

//Global statics
CGame* CGame::s_pGame = 0;

CGame::CGame()
{
}

CGame::~CGame()
{
	delete m_pClock;
	m_pClock = 0;
	delete m_pTerrain;
	m_pTerrain = 0;
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
	m_pPlayers = new std::vector < PlayerStates > ;
	*m_pPlayers = _Players;

	if (_iIndexOfClientPlayer > NetworkValues::MAX_USERS)
	{
		return false;
	}

	m_iNumberPlayers = _Players.size();
	m_pPlayerObj = new std::vector < C3DObject* > ;

	m_pRenderManager = _RenderManager;

	m_pClock = new CClock();
	VALIDATE(m_pClock->Initialise());

	//Set up the projection matrix	
	m_pRenderManager->CalculateProjectionMatrix(D3DXToRadian(45), 0.01f, 100000.0f);

	//Create and set up the terrain
	m_pTerrain = new CTerrain();
	ScalarVertex TerrainScalar = { 1.0f, 0.05f, 1.0f };
	std::string strImagePath = "Assets\\Heightmap.bmp";
	m_pTerrain->Initialise(m_pRenderManager, strImagePath, TerrainScalar);
	m_pTerrain->SetCenter(0, 0, 0);

	//Create the Camera
	m_pCamera = new CCameraStatic();
	D3DXVECTOR3 D3DPosition = { _Players[_iIndexOfClientPlayer].f3Positions.x, _Players[_iIndexOfClientPlayer].f3Positions.y + 100, _Players[_iIndexOfClientPlayer].f3Positions.z };
	D3DXVECTOR3 D3DLookAt = { _Players[_iIndexOfClientPlayer].f3Positions.x, _Players[_iIndexOfClientPlayer].f3Positions.y, _Players[_iIndexOfClientPlayer].f3Positions.z };
	m_pCamera->Initialise(D3DPosition, D3DLookAt, true);
	m_pCamera->Process(m_pRenderManager);

	//Create Mesh
	m_pCubeMesh = CreateCubeMesh(1.0f);
	
	
	for (int iPlayer = 0; iPlayer < m_iNumberPlayers; iPlayer++)
	{

		//Create Cube
		C3DObject* tempPlayerObj = new C3DObject();
		//Initialise the Cube Object with the Cube Mesh and set its coordinates
		tempPlayerObj->Initialise(m_pCubeMesh, _Players[iPlayer].f3Positions.x, _Players[iPlayer].f3Positions.y, _Players[iPlayer].f3Positions.z);

		m_pPlayerObj->push_back(tempPlayerObj);
	}

	return true;

	
}

void CGame::Process()
{
	//Process clock and get the delta tick
	m_pClock->Process();
	float fDT = m_pClock->GetDeltaTick();

	m_pCamera->Process(m_pRenderManager);
}

void CGame::Draw()
{
	m_pTerrain->Draw(m_pRenderManager);

	for (unsigned int iPlayer = 0; iPlayer < m_pPlayerObj->size(); iPlayer++)
	{
		(*m_pPlayerObj)[iPlayer]->Draw(m_pRenderManager);
	}
		
}

/***********************
* CreateCubeMesh: Creates a cube Mesh with origin in its very center
* @author: Jc Fowles
* @parameter: float _fCubeSize: Size of cube
* @return: CMesh*: Pointer to a created Cube mesh
********************/
CMesh* CGame::CreateCubeMesh(float _fCubeSize)
{
	//Create the Cube mesh
	CMesh* meshCube = new CMesh(m_pRenderManager);
	meshCube->SetPrimitiveType(IGPT_TRIANGLELIST);


	//Create the cube using vector normals
	std::vector<D3DXVECTOR3> vectTempPosition = { { -_fCubeSize / 2, _fCubeSize / 2, -_fCubeSize / 2 },
	{ _fCubeSize / 2, _fCubeSize / 2, -_fCubeSize / 2 },
	{ _fCubeSize / 2, -_fCubeSize / 2, -_fCubeSize / 2 },
	{ -_fCubeSize / 2, -_fCubeSize / 2, -_fCubeSize / 2 },
	{ -_fCubeSize / 2, _fCubeSize / 2, _fCubeSize / 2 },
	{ _fCubeSize / 2, _fCubeSize / 2, _fCubeSize / 2 },
	{ _fCubeSize / 2, -_fCubeSize / 2, _fCubeSize / 2 },
	{ -_fCubeSize / 2, -_fCubeSize / 2, _fCubeSize / 2 } };
	//Calculate the normals of the cube
	for (unsigned int i = 0; i < vectTempPosition.size(); i++)
	{
		D3DXVECTOR3 tempNormal;
		D3DXVec3Normalize(&tempNormal, &vectTempPosition[i]);
		meshCube->AddVertex(CVertexNormal(vectTempPosition[i].x, vectTempPosition[i].y, vectTempPosition[i].z, tempNormal.x, tempNormal.y, tempNormal.z));
	}

	//Add the Indices
	std::vector<int> vecIndices = { 0, 1, 3,
		1, 2, 3,
		1, 5, 2,
		5, 6, 2,
		5, 4, 6,
		4, 7, 6,
		4, 0, 7,
		0, 3, 7,
		4, 5, 0,
		5, 1, 0,
		3, 2, 7,
		2, 6, 7 };

	//Create the static buffer
	meshCube->SetIndexList(vecIndices);
	meshCube->CreateStaticBuffer();
	//Return the mesh
	return meshCube;
}

void CGame::RemovePlayer(std::string _strPlayerName)
{
	for (unsigned int iPlayer = 0; iPlayer < m_pPlayerObj->size(); iPlayer++)
	{
		std::string strNameCheck((*m_pPlayers)[iPlayer].cPLayerName);

		if (strNameCheck == _strPlayerName)
		{			
			m_pPlayerObj->erase(m_pPlayerObj->begin() + iPlayer);
			m_pPlayers->erase(m_pPlayers->begin() + iPlayer);
		}

	}
}