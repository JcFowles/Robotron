#pragma once

//Library include


//Local includes
#include "../Shared/Network_Defines.h"
#include "../Shared/Clock.h"
#include "IRenderer.h"
#include "Terrain.h"
#include "Mesh.h"
#include "C3DObject.h"
#include "CameraStatic.h"
#include "../Shared/Graphics_Defines.h"
#include "../Shared/App_Defines.h"



class CGame
{
public:

	//Singleton Methods
	static CGame& GetInstance();
	static void DestroyInstance();

	~CGame(void);

	bool Initialise(IRenderer* _RenderManager, std::vector<PlayerStates> _Players, int _iIndexOfClientPlayer);

	void Draw();

	void Process();
	void ProcessInput(float _fDt);

	//Render
	void RenderSingleFrame();


	CMesh* CreateCubeMesh(float _fCubeSize);

private:
	//Disallowing copies and extra constructions
	CGame();
	CGame(const CGame& _kr);
	CGame& operator= (const CGame& _kr);

	//member variables
private:
	
	IRenderer* m_pRenderManager;

	CClock* m_pClock;
	static CGame* s_pGame;

	C3DObject* m_pPlayer;

	std::vector<C3DObject* >* m_pPlayerObj;

	CMesh* m_pCubeMesh;

	CTerrain* m_pTerrain;
	CCameraStatic* m_pCamera;

	float m_fPlayerSize;
	int m_iNumberPlayers;
};

