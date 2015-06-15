#pragma once

//Library include

//Local includes
#include "../Shared/Network_Defines.h"
#include "../Shared/Clock.h"
#include "../Shared/IRenderer.h"
#include "../Shared/Mesh.h"
#include "../Shared/C3DObject.h"
#include "../Shared/Graphics_Defines.h"
#include "../Shared/App_Defines.h"
#include "Terrain.h"
#include "CameraStatic.h"


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


	CMesh* CreatePlayerMesh(float _fCubeSize);

private:
	//Disallowing copies and extra constructions
	CGame();
	CGame(const CGame& _kr);
	CGame& operator= (const CGame& _kr);

	//member variables
private:
	
	IRenderer* m_pRenderManager;
		
	static CGame* s_pGame;
	
	std::map<std::string, C3DObject*>* m_plistPlayers;
	std::pair<std::string, C3DObject*>* m_pClientAvatar;

	CMesh* m_pPlayerMesh;

	CTerrain* m_pTerrain;
	CCameraStatic* m_pCamera;

	float m_fPlayerSize;
	int m_iNumberPlayers;
};
