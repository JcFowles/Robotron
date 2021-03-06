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
	delete m_pUnderLay;
	m_pUnderLay = 0;
	
	//Delete Camera
	delete m_pCamera;
	m_pCamera = 0;

	delete m_pDebugCam;
	m_pDebugCam = 0;

	//delete Mesh
	delete m_pPlayerMesh;
	m_pPlayerMesh = 0;
	
	delete m_pProjectileMesh;
	m_pProjectileMesh = 0;

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
	// Delete Bullet list
	std::map <UINT, CProjectileObj* >::iterator itterBullet = m_pListBullets->begin();
	while (itterBullet != m_pListBullets->end())
	{
		delete itterBullet->second;
		itterBullet->second = 0;

		itterBullet++;
	}
	m_pListBullets->clear();
	delete m_pListBullets;
	m_pListBullets = 0;


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

bool CGame::Initialise(IRenderer* _RenderManager, std::string _ControllingPlayer, bool _bSinglePlayer)
{	
	m_bSinglePlayer = _bSinglePlayer;
	m_strPlayerName = _ControllingPlayer;
		
	m_plistPlayers = new std::map < std::string, CPlayerObj* >;
	m_pListEnemies = new std::map < UINT, CEnemyObj* > ;
	m_pEnemyMesh = new std::map < eEnemyTypes, CMesh* > ;
	m_iEnemyIDs = new std::map < eEnemyTypes, UINT > ;
	m_iPowerUpIDs = new std::map < ePowerType, UINT > ;
	m_pListPowerUps = new std::map < UINT, CPowerUpObj* > ;
	m_pPowerUpMesh = new std::map < ePowerType, CMesh* > ;
	m_pListBullets = new std::map < UINT, CProjectileObj* > ;

	m_pRenderManager = _RenderManager;
		
	//Set up the projection matrix	
	m_pRenderManager->CalculateProjectionMatrix(D3DXToRadian(45), 0.01f, 100000.0f);

	//Create and set up the terrain
	m_pTerrain = new CTerrain();
	ScalarVertex TerrainScalar = { kfScalarWidth, kfScalarHeight, kfScalarDepth };
	std::string strHightMapPath = "Assets\\Heightmap.bmp";
	std::string strTexturePath = "Assets\\Floor.png";
	VALIDATE(m_pTerrain->Initialise(m_pRenderManager, strHightMapPath, strTexturePath, TerrainScalar,20 ));
	m_pTerrain->SetCenter({ 0, 0, 0 });

	m_pUnderLay = new CTerrain();
	strHightMapPath = "Assets\\Heightmap.bmp";
	strTexturePath = "Assets\\Space.png";
	TerrainScalar = { kfScalarWidth*1.6f, kfScalarHeight, kfScalarDepth*1.6f };
	VALIDATE(m_pUnderLay->Initialise(m_pRenderManager, strHightMapPath, strTexturePath, TerrainScalar, 1));
	m_pUnderLay->SetCenter({ 0, -30, 0 });
	
	//Debug camera initialize
	m_pDebugCam = new CDebugCamera();
	VALIDATE(m_pDebugCam->Initialise(m_pRenderManager));
	m_bDebug = false;

	//Create player assets
	m_iPlayerMaterialID = CreatePlayerAssest();

	//Create the lust enemy assets
	m_iEnemyIDs->insert(std::pair<eEnemyTypes, UINT>(ET_LUST, CreateEnemyAssest(ET_LUST)));
	m_iEnemyIDs->insert(std::pair<eEnemyTypes, UINT>(ET_SLOTH, CreateEnemyAssest(ET_SLOTH)));
	m_iEnemyIDs->insert(std::pair<eEnemyTypes, UINT>(ET_WRATH, CreateEnemyAssest(ET_WRATH)));

	//Create the Shield power up assets
	m_iPowerUpIDs->insert(std::pair<ePowerType, UINT>(PU_TEN, CreatePowerUpAssest(PU_TEN)));
	m_iPowerUpIDs->insert(std::pair<ePowerType, UINT>(PU_FIFTY, CreatePowerUpAssest(PU_FIFTY)));
	m_iPowerUpIDs->insert(std::pair<ePowerType, UINT>(PU_HUNDRED, CreatePowerUpAssest(PU_HUNDRED)));
	
	//Create Bullet asset
	m_iBulletMaterialID = CreateProjectileAssest();
		
	//Initialise DirectionLight
	D3DLightParameter DirectiomLightParam;
	DirectiomLightParam.bIsTurnedOn = true;
	DirectiomLightParam.eLightType = D3DLIGHT_DIRECTIONAL;
	DirectiomLightParam.colorDiffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f); //If changing this change in updateDirtional light too
	DirectiomLightParam.vecDirection = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	//Create the directionLight
	m_iDirectionID = m_pRenderManager->CreateLights(DirectiomLightParam);

	//Update the Direction Light
	m_pRenderManager->UpdateDirectionLight(m_iDirectionID, true);
	

	D3DLightParameter SpotLightParam;
	SpotLightParam.eLightType = D3DLIGHT_SPOT;
	SpotLightParam.bIsTurnedOn = false;
	SpotLightParam.fAttnLinear = 0.01f;
	SpotLightParam.fAttnConstant = 0.0f;
	SpotLightParam.fAttnExponential = 0.0f;
	SpotLightParam.fFallOff = 1.0f;
	SpotLightParam.fInnerAngle = (10.0f);
	SpotLightParam.fOuterAngle = (100.0f);
	SpotLightParam.fRange = 50.0f;
	SpotLightParam.vecDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	SpotLightParam.vecPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SpotLightParam.colorDiffuse = D3DXCOLOR(-10.0f, -10.0f, -10.0f, -10.0f);

	m_iSpotLightID = m_pRenderManager->CreateLights(SpotLightParam);

	m_bLightning = false;

	bToggle = 0;

	return true;
		
}

void CGame::Process(ClientDataPacket* _pClientPacket)
{
	//process players
	ProcessPlayers(_pClientPacket);
	//Process Enemies
	ProcessEnemies(_pClientPacket);
	//Process Projectiles
	ProcessProjectiles(_pClientPacket);
	//Process power ups
	ProcessPowerUps(_pClientPacket);
	//Process Camera
	ProcessCamera();

	m_pClientPacket = _pClientPacket;

	//Toggle th toggle
	bToggle = !bToggle;
		
}

void CGame::ProcessPlayers(ClientDataPacket* _pClientPacket)
{
	//Process players
	std::map<std::string, CPlayerObj*>::iterator iterPlayer;
	for (UINT iPlayer = 0; iPlayer < _pClientPacket->iNumPlayers; iPlayer++)
	{
		PlayerStates PlayerInfo = _pClientPacket->PlayerInfo[iPlayer];
		UINT uiID = PlayerInfo.uiPlayerID;
		std::string strName(PlayerInfo.cPlayerName);
		iterPlayer = m_plistPlayers->find(strName);

		if (iterPlayer != m_plistPlayers->end())
		{
			
			//Set player position
			iterPlayer->second->SetPosition(_pClientPacket->PlayerInfo[iPlayer].f3Positions);
			//Set player Direction
			iterPlayer->second->SetDirection(_pClientPacket->PlayerInfo[iPlayer].f3Direction);
			//Set Player Health
			iterPlayer->second->SetHealth(_pClientPacket->PlayerInfo[iPlayer].iHealth);
			//Set Player life count
			iterPlayer->second->SetLifeCount(_pClientPacket->PlayerInfo[iPlayer].iLifeCount);
			//Set Player Score
			iterPlayer->second->SetScore(_pClientPacket->PlayerInfo[iPlayer].uiScore);
			//Set Player Score
			iterPlayer->second->SetDamage(_pClientPacket->PlayerInfo[iPlayer].iDamage);
			//Set Player Score
			iterPlayer->second->SetMaxHealth(_pClientPacket->PlayerInfo[iPlayer].iMaxHealth);
			//Set Player Score
			iterPlayer->second->SetWaveNumber(_pClientPacket->iWaveNumber);
			//Set Player Score
			iterPlayer->second->SetEnemyCount(_pClientPacket->iNumEnemies);
			//toggle toggle value to state that this has been updated
			iterPlayer->second->m_bToggle = bToggle;
			//set alive status of player
			iterPlayer->second->SetIsAlive(_pClientPacket->PlayerInfo[iPlayer].bAlive);

			if (_pClientPacket->PlayerInfo[iPlayer].bAlive)
			{
				//Update the lights
				if (iterPlayer->second->GetLightRange() > 5.0f)
				{
					iterPlayer->second->SetLightRange(1.0f);
				}
				else
				{
					iterPlayer->second->SetLightRange(iterPlayer->second->GetLightRange() + 0.05f);
				}

				m_pRenderManager->UpdatePointLight(iterPlayer->second->GetLightID(), true, iterPlayer->second->GetPosition(), iterPlayer->second->GetLightRange());
			
			}
			else
			{
				//PLayer died turn there light off
				m_pRenderManager->UpdatePointLight(iterPlayer->second->GetLightID(), false, iterPlayer->second->GetPosition(), iterPlayer->second->GetLightRange());
			}

			//Lightning strike
			m_pRenderManager->UpdateDirectionLight(m_iDirectionID, m_bLightning);

			if (iPlayer == m_iIndexOfClientPlayer)
			{
				m_pPlayerAvatar = iterPlayer->second;

			}
			
		}
		else
		{
			//Player not found, re Add him
			AddPlayer(_pClientPacket, strName);
		}
	}

	//We have a player to delete
	std::string strToDelete = "  ";
	std::map<std::string, CPlayerObj*>::iterator ObjectCheck = m_plistPlayers->begin();
	while (ObjectCheck != m_plistPlayers->end())
	{
		//If this object has not been toggled, it has not been updated
		//which means that it doesnt exist on The server, so delete it
		if (ObjectCheck->second->m_bToggle != bToggle)
		{
			strToDelete = ObjectCheck->first;
			delete ObjectCheck->second;
		}

		//Check next object
		ObjectCheck++;
	}
	m_plistPlayers->erase(strToDelete);


}

void CGame::ProcessEnemies(ClientDataPacket* _pClientPacket)
{
	//Process list of Enemies
	std::map<UINT, CEnemyObj*>::iterator enemyIter;
	for (UINT iEnemy = 0; iEnemy < _pClientPacket->iNumEnemies; iEnemy++)
	{
		EnemyStates EnemyInfo = _pClientPacket->EnemyInfo[iEnemy];
		UINT uiID = EnemyInfo.uiEnemyID;
		enemyIter = m_pListEnemies->find(uiID);
		
		if (enemyIter != m_pListEnemies->end())
		{
			if (enemyIter->second->GetEnemyType() == ET_WRATH)
			{
				m_pRenderManager->UpdateSpotLight(m_iSpotLightID, true, enemyIter->second->GetPosition(), enemyIter->second->GetDirection()*5.0f);
			}

			//Set player position
			enemyIter->second->SetPosition(_pClientPacket->EnemyInfo[iEnemy].f3Positions);
			//Set player Direction
			enemyIter->second->SetDirection(_pClientPacket->EnemyInfo[iEnemy].f3Direction);
			//toggle toggle value to state that this has been updated
			enemyIter->second->m_bToggle = bToggle;



		}
		else
		{
			_pClientPacket->singleEnemyInfo = EnemyInfo;
			CreateEnemy(_pClientPacket);
		}
	}

	//We have a bullet to delete
	int IDToDelete = 0;
	std::map<UINT, CEnemyObj*>::iterator ObjectCheck = m_pListEnemies->begin();
	while (ObjectCheck != m_pListEnemies->end())
	{
		//If this object has not been toggled, it has not been updated
		//which means that it doesnt exist on The server, so delete it
		if (ObjectCheck->second->m_bToggle != bToggle)
		{
			IDToDelete = ObjectCheck->second->GetObjectID();
			delete ObjectCheck->second;

			if (enemyIter->second->GetEnemyType() == ET_WRATH)
			{
				m_pRenderManager->UpdateSpotLight(m_iSpotLightID, false, enemyIter->second->GetPosition(), enemyIter->second->GetDirection());
			}

		}

		//Check next object
		ObjectCheck++;
	}
	m_pListEnemies->erase(IDToDelete);
}

void CGame::ProcessProjectiles(ClientDataPacket* _pClientPacket)
{
	
	//Process bullet
	 	
	std::map<UINT, CProjectileObj*>::iterator BulletIter;
	for (UINT iBullet = 0; iBullet < _pClientPacket->iNumProjectiles; iBullet++)
	{
		ProjectileStates BulletInfo = _pClientPacket->ProjectilesInfo[iBullet];
		UINT uiID = BulletInfo.uiProjectileID;
		BulletIter = m_pListBullets->find(uiID);

		if (BulletIter != m_pListBullets->end())
		{
			//Set player position
			BulletIter->second->SetPosition(_pClientPacket->ProjectilesInfo[iBullet].f3Positions);
			//Set player Direction
			BulletIter->second->SetDirection(_pClientPacket->ProjectilesInfo[iBullet].f3Direction);
			//toggle toggle value to state that this has been updated
			BulletIter->second->m_bToggle = bToggle;
		}
		else
		{
			_pClientPacket->singleProjectileInfo = BulletInfo;
			CreateProjectile(_pClientPacket);
		}
	}

	//We have a bullet to delete
	int IDToDelete = 0;
	std::map<UINT, CProjectileObj*>::iterator ObjectCheck = m_pListBullets->begin();
	while (ObjectCheck != m_pListBullets->end())
	{
		//If this object has not been toggled, it has not been updated
		//which means that it doesnt exist on The server, so delete it
		if (ObjectCheck->second->m_bToggle != bToggle)
		{
			IDToDelete = ObjectCheck->second->GetObjectID();
			delete ObjectCheck->second;
		}

		//Check next object
		ObjectCheck++;
	}
	m_pListBullets->erase(IDToDelete);
	
}

void CGame::ProcessPowerUps(ClientDataPacket* _pClientPacket)
{
	//Process power ups
	std::map<UINT, CPowerUpObj*>::iterator PowUpIter;
	for (UINT iPower = 0; iPower < _pClientPacket->iNumPowerUps; iPower++)
	{
		PowerUpStates PowUpInfo = _pClientPacket->PowUpInfo[iPower];
		UINT uiID = PowUpInfo.uiPowUpID;
		PowUpIter = m_pListPowerUps->find(uiID);

		if (PowUpIter != m_pListPowerUps->end())
		{
			//Set player position
			PowUpIter->second->SetPosition(_pClientPacket->PowUpInfo[iPower].f3Positions);
			//Set player Direction
			PowUpIter->second->SetDirection(_pClientPacket->PowUpInfo[iPower].f3Direction);
			//toggle toggle value to state that this has been updated
			PowUpIter->second->m_bToggle = bToggle;

		}
		else //object not created on client, but exist on the server so create it
		{
			_pClientPacket->singlePowUpInfo = PowUpInfo;
			CreatePowerUp(_pClientPacket);
		}
	}

	//We have a bullet to delete
	int IDToDelete = 0;
	std::map<UINT, CPowerUpObj*>::iterator ObjectCheck = m_pListPowerUps->begin();
	while (ObjectCheck != m_pListPowerUps->end())
	{
		//If this object has not been toggled, it has not been updated
		//which means that it doesnt exist on The server, so delete it
		if (ObjectCheck->second->m_bToggle != bToggle)
		{
			IDToDelete = ObjectCheck->second->GetObjectID();
			delete ObjectCheck->second;
		}

		//Check next object
		ObjectCheck++;
	}
	m_pListPowerUps->erase(IDToDelete);
}

void CGame::ProcessCamera()
{

	//Process Camera	
	std::map<std::string, CPlayerObj*>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
	m_pPlayerAvatar = playerItter->second;

	if (m_bDebug) //If debug mode process the debug camera
	{
		m_pDebugCam->Process();
	}
	else
	{
		D3DXVECTOR3 d3dVTarget;
		D3DXVECTOR3 d3dVPos;
		D3DXVECTOR3 d3dVUp;
		d3dVUp.x = m_pPlayerAvatar->GetUpVector().x;
		d3dVUp.y = m_pPlayerAvatar->GetUpVector().y;
		d3dVUp.z = m_pPlayerAvatar->GetUpVector().z;

		d3dVPos.x = m_pPlayerAvatar->GetPosition().x;
		d3dVPos.y = m_pPlayerAvatar->GetPosition().y;
		d3dVPos.z = m_pPlayerAvatar->GetPosition().z;

		d3dVTarget = d3dVPos;
		if (m_pPlayerAvatar->GetIsAlive() == false)
		{			
			d3dVPos.y = m_pPlayerAvatar->GetPosition().y + 50.0f;
		}
		
		
		//Set the camera
		if (m_pCamera == 0)
		{
			m_pCamera = new CCamera();
			//Initialise the camera position
			D3DXVECTOR3 D3DPosition = { d3dVPos.x, d3dVPos.y + 50.0f, d3dVPos.z };
			//Initialise the Camera target
			D3DXVECTOR3 D3DLookAt = { d3dVPos.x, d3dVPos.y, d3dVPos.z };
			(m_pCamera->Initialise(D3DPosition, D3DLookAt, false));
		}
		m_pCamera->SetCamera(d3dVPos, d3dVPos, d3dVUp);
		m_pCamera->Process(m_pRenderManager);
	}
}

void CGame::Draw()
{
	// AB: Object Ordering

	//Draw the under lay
	m_pUnderLay->Draw(m_pRenderManager);
	
	//Draw every passed in from the server
	// AB: Alpha Blending
	m_pRenderManager->EnableAlphaBlend(true);
	m_pTerrain->Draw(m_pRenderManager);
	m_pRenderManager->EnableAlphaBlend(false);

	//Draw players
	DrawPlayers();
	
	//Draw enemies
	DrawEnemies();
	
	//Draw projectiles
	DrawProjectile();

	//Draw Power Ups
	DrawPowerUps();
	
	//Draw HUD
	DrawHUD();
	
	
		
}

void CGame::DrawPlayers()
{
	//Loop through all players
	std::map< std::string, CPlayerObj*>::iterator iterPlayer = m_plistPlayers->begin();
	while (iterPlayer != m_plistPlayers->end())
	{
		//Only Draw players that are alive
		if (iterPlayer->second->GetIsAlive())
		{
			//Draw the player avatar
			iterPlayer->second->Draw();
		}
		iterPlayer++;
	}
}

void CGame::DrawEnemies()
{
	std::map< UINT, CEnemyObj*>::iterator iterEnemy = m_pListEnemies->begin();
	while (iterEnemy != m_pListEnemies->end())
	{
		//Draw the Enemy avatar
		iterEnemy->second->Draw();

		iterEnemy++;
	}
}

void CGame::DrawPowerUps()
{
	//Alpha blend the power ups
	
	std::map< UINT, CPowerUpObj*>::iterator iterPowUP = m_pListPowerUps->begin();
	while (iterPowUP != m_pListPowerUps->end())
	{
		//Draw the Power UP avatar
		iterPowUP->second->Draw();

		iterPowUP++;
	}
	
}

void CGame::DrawHUD()
{
	//Display HUD
	DWORD dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE;
	DWORD dwTextColor = D3DCOLOR_XRGB(165, 0, 0);
	int iLeft = 5;
	int iTop = 15;
	int TextWidth = m_pRenderManager->GetFontWidth(TEXT_LIST);
	int TextHieght = m_pRenderManager->GetFontHeight(TEXT_LIST);
	RECT HudRec;

	//Health
	std::string strHealth = "Health : ";
	strHealth += std::to_string(m_pPlayerAvatar->GetHealth());
	strHealth += " / ";
	strHealth += std::to_string(m_pPlayerAvatar->GetMaxHealth());
	TextWidth = TextWidth * strHealth.length() + 5;
	HudRec.left = iLeft;
	HudRec.top = iTop;
	HudRec.right = 995;
	HudRec.bottom = iTop + TextHieght;
	m_pRenderManager->RenderText(strHealth, HudRec, dwTextColor, TEXT_LIST, dwTextFormat);

	//Wave count
	dwTextFormat = DT_RIGHT | DT_BOTTOM | DT_SINGLELINE;
	std::string strWave = "Wave : ";
	strWave += std::to_string(m_pPlayerAvatar->GetWaveNumber());
	HudRec.top = iTop;
	HudRec.bottom = iTop + TextHieght;
	m_pRenderManager->RenderText(strWave, HudRec, dwTextColor, TEXT_LIST, dwTextFormat);


	//Damage
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE;
	std::string strDamge = "Damage : ";
	strDamge += std::to_string(m_pPlayerAvatar->GetDamge());
	iTop += TextHieght + 1;
	HudRec.top = iTop;
	HudRec.bottom = iTop + TextHieght;
	m_pRenderManager->RenderText(strDamge, HudRec, dwTextColor, TEXT_LIST, dwTextFormat);


	//Damage
	dwTextFormat = DT_RIGHT | DT_BOTTOM | DT_SINGLELINE;
	std::string strEnemies = "Enemies Remaining : ";
	strEnemies += std::to_string(m_pPlayerAvatar->GetEnemyCount());
	HudRec.top = iTop;
	HudRec.bottom = iTop + TextHieght;
	m_pRenderManager->RenderText(strEnemies, HudRec, dwTextColor, TEXT_LIST, dwTextFormat);


	//Score
	dwTextFormat = DT_LEFT | DT_BOTTOM | DT_SINGLELINE;
	std::string strScore = "Score : ";
	strScore += std::to_string(m_pPlayerAvatar->GetScore());
	iTop += TextHieght + 1;
	HudRec.top = iTop;
	HudRec.bottom = iTop + TextHieght;
	m_pRenderManager->RenderText(strScore, HudRec, dwTextColor, TEXT_LIST, dwTextFormat);

	//Life count
	std::string strLife = "Lives : ";
	strLife += std::to_string(m_pPlayerAvatar->GetLifeCount());
	iTop += TextHieght + 1;
	HudRec.top = iTop;
	HudRec.bottom = iTop + TextHieght;
	m_pRenderManager->RenderText(strLife, HudRec, dwTextColor, TEXT_LIST, dwTextFormat);
}

void CGame::DrawProjectile()
{
	std::map< UINT, CProjectileObj*>::iterator iterBullet = m_pListBullets->begin();
	while (iterBullet != m_pListBullets->end())
	{
		//Draw the Power UP avatar
		iterBullet->second->Draw();

		iterBullet++;
	}
}

void CGame::RenderTeamScores(ClientDataPacket* _pClientPacket)
{
	RECT ScoreDisplayRect;
	DWORD dwTextFormat; 
	DWORD dwTextColor = D3DCOLOR_XRGB(255, 255, 255); //Change Color

	//Rect size set up
	ScoreDisplayRect.top = 200;
	ScoreDisplayRect.left = 200;
	ScoreDisplayRect.right = 800;
	ScoreDisplayRect.bottom = 230;

	//Display the headings
	std::string strNameHeading = " Player";
	std::string strScoreHeading = " Score ";
	std::string strHealthHeading = " Health ";

	dwTextFormat = DT_LEFT | DT_VCENTER| DT_SINGLELINE ;
	m_pRenderManager->RenderText(strNameHeading, ScoreDisplayRect, dwTextColor, TEXT_LIST, dwTextFormat);
	dwTextFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	m_pRenderManager->RenderText(strScoreHeading, ScoreDisplayRect, dwTextColor, TEXT_LIST, dwTextFormat);
	dwTextFormat = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
	m_pRenderManager->RenderText(strHealthHeading, ScoreDisplayRect, dwTextColor, TEXT_LIST, dwTextFormat);
		
	ScoreDisplayRect.left += 20;
	ScoreDisplayRect.right -= 30;

	//Loop through each player
	std::map<std::string, CPlayerObj*>::iterator iterPlayer;
	for (UINT iPlayer = 0; iPlayer < _pClientPacket->iNumPlayers; iPlayer++)
	{
		ScoreDisplayRect.top += 30;
		ScoreDisplayRect.bottom += 30;
		
		//Get player info, name, score, and current health
		std::string strPlayerName(_pClientPacket->PlayerInfo[iPlayer].cPlayerName);
		std::string strPlayerScore = std::to_string(_pClientPacket->PlayerInfo[iPlayer].uiScore);
		std::string strPlayerHealth = std::to_string(_pClientPacket->PlayerInfo[iPlayer].iHealth);

		//Render the player information
		dwTextFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
		m_pRenderManager->RenderText(strPlayerName, ScoreDisplayRect, dwTextColor, TEXT_LIST, dwTextFormat);
		dwTextFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		m_pRenderManager->RenderText(strPlayerScore, ScoreDisplayRect, dwTextColor, TEXT_LIST, dwTextFormat);
		dwTextFormat = DT_RIGHT | DT_VCENTER | DT_SINGLELINE;
		m_pRenderManager->RenderText(strPlayerHealth, ScoreDisplayRect, dwTextColor, TEXT_LIST, dwTextFormat);
	}
}

CMesh* CGame::CreateCubeMesh(float _fCubeSize, int iTextureID)
{
	//Create the Cube mesh
	CMesh* meshCube = new CMesh(m_pRenderManager, _fCubeSize, iTextureID);
	meshCube->SetPrimitiveType(IGPT_TRIANGLELIST);
	
	//Create the cube using Vertex with normals and UV Coords

	//Front Face
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, _fCubeSize, -_fCubeSize),	float3(-1.0f, 1.0f, -1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, _fCubeSize, -_fCubeSize),		float3(1.0f, 1.0f, -1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, -_fCubeSize, -_fCubeSize),	float3(1.0f, -1.0f, -1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, -_fCubeSize, -_fCubeSize),	float3(-1.0f, -1.0f, -1.0f),	float2(0.0f, 1.0f)));

	//Left Face
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, _fCubeSize, _fCubeSize),		float3(-1.0f, 1.0f, 1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, _fCubeSize, -_fCubeSize),	float3(-1.0f, 1.0f, -1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, -_fCubeSize, -_fCubeSize),	float3(-1.0f, -1.0f, -1.0f),	float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, -_fCubeSize, _fCubeSize),	float3(-1.0f, -1.0f, 1.0f),		float2(0.0f, 1.0f)));

	//Right Face
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, _fCubeSize, -_fCubeSize),		float3(1.0f, 1.0f, -1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, _fCubeSize, _fCubeSize),		float3(1.0f, 1.0f, 1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, -_fCubeSize, _fCubeSize),		float3(1.0f, -1.0f, 1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, -_fCubeSize, -_fCubeSize),	float3(1.0f, -1.0f, -1.0f),		float2(0.0f, 1.0f)));

	//Back Face
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, _fCubeSize, _fCubeSize),		float3(1.0f, 1.0f, 1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, _fCubeSize, _fCubeSize),		float3(-1.0f, 1.0f, 1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, -_fCubeSize, _fCubeSize),	float3(-1.0f, -1.0f, 1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, -_fCubeSize, _fCubeSize),		float3(1.0f, -1.0f, 1.0f),		float2(0.0f, 1.0f)));

	//Top Face
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, _fCubeSize, _fCubeSize),		float3(-1.0f, 1.0f, 1.0f),		float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, _fCubeSize, _fCubeSize),		float3(1.0f, 1.0f, 1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, _fCubeSize, -_fCubeSize),		float3(1.0f, 1.0f, -1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, _fCubeSize, -_fCubeSize),	float3(-1.0f, 1.0f, -1.0f),		float2(0.0f, 1.0f)));

	//Bottom Face
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, -_fCubeSize, -_fCubeSize),	float3(-1.0f, -1.0f, -1.0f),	float2(0.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, -_fCubeSize, -_fCubeSize),	float3(1.0f, -1.0f, -1.0f),		float2(1.0f, 0.0f)));
	meshCube->AddVertex(CVertexUV(float3(_fCubeSize, -_fCubeSize, _fCubeSize),		float3(1.0f, -1.0f, 1.0f),		float2(1.0f, 1.0f)));
	meshCube->AddVertex(CVertexUV(float3(-_fCubeSize, -_fCubeSize, _fCubeSize),	float3(-1.0f, -1.0f, 1.0f),		float2(0.0f, 1.0f)));

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
	tempPlayer->Initialise(bToggle,m_pRenderManager, m_iPlayerMaterialID, m_pPlayerMesh, tempState.uiPlayerID, tempState.f3Positions);
	
	//Add the player to the map
	std::string strName(tempState.cPlayerName);
	MapClientIter = m_plistPlayers->insert(std::pair<std::string, CPlayerObj*>(strName, tempPlayer));
	
	//if the player wasnt added 
	if (MapClientIter.second == false)
	{
		//delete the temp variable
		delete tempPlayer;
		tempPlayer = 0;

		//Return the bool part(second)
		//false if the element already exists
		return false;
	}

	//If this is the controlling players object save it
	if (tempState.cPlayerName == m_strPlayerName)
	{
		//Find the controlling player in the map
		std::map<std::string, CPlayerObj*>::iterator playerItter = m_plistPlayers->find(m_strPlayerName);
		m_pPlayerAvatar = playerItter->second;

		//Create the Camera
		m_pCamera = new CCamera();
		//Initialise the camera position
		D3DXVECTOR3 D3DPosition = { playerItter->second->GetPosition().x, playerItter->second->GetPosition().y + 50.0f, playerItter->second->GetPosition().z };
		//Initialise the Camera target
		D3DXVECTOR3 D3DLookAt = { playerItter->second->GetPosition().x, playerItter->second->GetPosition().y, playerItter->second->GetPosition().z };
		(m_pCamera->Initialise(D3DPosition, D3DLookAt, false));
		m_pCamera->Process(m_pRenderManager);

	}
	
	//if the player wasnt added 
	if (MapClientIter.second == false)
	{
		//delete the temp variable
		delete tempPlayer;
		tempPlayer = 0;
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
		tempPlayer->Initialise(bToggle, m_pRenderManager, m_iPlayerMaterialID, m_pPlayerMesh, CurrentPlayerState.uiPlayerID, CurrentPlayerState.f3Positions);

		//Add the player to the map
		MapClientIter = m_plistPlayers->insert(std::pair<std::string, CPlayerObj*>(CurrentPlayerState.cPlayerName, tempPlayer));

		//if the player wasnt added 
		if (MapClientIter.second == false)
		{
			//delete the temp variable
			delete tempPlayer;
			tempPlayer = 0;
		}

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
	//Find player to remove
	std::map<std::string, CPlayerObj*>::iterator playerToRemove = m_plistPlayers->find(_strLeftPlayer);

	if (playerToRemove != m_plistPlayers->end())
	{
		//Turn their light off
		m_pRenderManager->UpdatePointLight(playerToRemove->second->GetLightID(), false, playerToRemove->second->GetPosition(), playerToRemove->second->GetLightRange());
		//Delete the object
		delete playerToRemove->second;
		playerToRemove->second = 0;
		//Remove from list
		m_plistPlayers->erase(_strLeftPlayer);
	}
}

int CGame::CreatePlayerAssest()
{
	//Create the texture for the Player object
	std::string strFilePath = "Assets\\Angel.png";
	int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
	m_pPlayerMesh = CreateCubeMesh(kfPlayerSize, iTextureID);

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
		fEnemySize = kfLustSize;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 0.00f, 0.0f, 1.0f };
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
	{
		strFilePath = "Assets\\Wrath.png";
		fEnemySize = kfWrathSize;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 0.00f, 0.50f, 1.0f };
		Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };

		//Create the Texture for the enemy
		int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
		CMesh* pTempMesh = CreateCubeMesh(fEnemySize, iTextureID);
		//Add mesh to map
		m_pEnemyMesh->insert(std::pair<eEnemyTypes, CMesh* >(ET_WRATH, pTempMesh));
	}
		break;
	case ET_GREED:
		break;
	case ET_SLOTH:
	{
		strFilePath = "Assets\\Sloth.png";
		fEnemySize = kfSlothSize;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 0.40f, 1.00f, 3.0f, 1.0f };
		Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };

		//Create the Texture for the enemy
		int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
		CMesh* pTempMesh = CreateCubeMesh(fEnemySize, iTextureID);
		//Add mesh to map
		m_pEnemyMesh->insert(std::pair<eEnemyTypes, CMesh* >(ET_SLOTH, pTempMesh));

	}
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
		Material.f4Emissive = { 1.0f, 0.0f, 0.0f, 1.0f };
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
	case PU_TEN:
	{
		strFilePath = "Assets\\10.png";
		fSize = kfTenSize;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };

		//Create the Texture for the enemy
		int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
		CMesh* pTempMesh = CreateCubeMesh(fSize, iTextureID);
		//Add mesh to map
		m_pPowerUpMesh->insert(std::pair<ePowerType, CMesh* >(PU_TEN, pTempMesh));

	}
	break;
	case PU_FIFTY:
	{
		strFilePath = "Assets\\50.png";
		fSize = kfFiftySize;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };

		//Create the Texture for the enemy
		int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
		CMesh* pTempMesh = CreateCubeMesh(fSize, iTextureID);
		//Add mesh to map
		m_pPowerUpMesh->insert(std::pair<ePowerType, CMesh* >(PU_FIFTY, pTempMesh));

	}
	break;
	case PU_HUNDRED:
	{
		strFilePath = "Assets\\100.png";
		fSize = kfHundredSize;

		Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };

		//Create the Texture for the enemy
		int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
		CMesh* pTempMesh = CreateCubeMesh(fSize, iTextureID);
		//Add mesh to map
		m_pPowerUpMesh->insert(std::pair<ePowerType, CMesh* >(PU_HUNDRED, pTempMesh));

	}
	break;
	default:
	{
		strFilePath = "";
		fSize = 0.0f;

		Material.f4Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
		Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.f4Emissive = { 1.0f, 0.0f, 0.0f, 1.0f };
		Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Material.fPower = { 1.0f };
	}
	break;
	}

	//Get the material Id used to reference the created material
	UINT iMatID = m_pRenderManager->CreateMaterial(Material);

	return iMatID;



}

int CGame::CreateProjectileAssest()
{
	std::string strFilePath = "";
	
	MaterialValues Material;
	
	strFilePath = "Assets\\Bullet.png";
	
	Material.f4Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.f4Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	Material.f4Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material.fPower = { 1.0f };

	//Create the Texture/mesh for the Projectile
	int iTextureID = m_pRenderManager->CreateTexture(strFilePath);
	m_pProjectileMesh = CreateCubeMesh(kfBulletSize, iTextureID);
	
	//Get the material Id used to reference the created material
	UINT iMatID = m_pRenderManager->CreateMaterial(Material);

	return iMatID;



}

bool CGame::GameOverCheck()
{
	bool bGameOver = true;
	std::map<std::string, CPlayerObj*>::iterator playerIter = m_plistPlayers->begin();
	std::map<std::string, CPlayerObj*>::iterator playerIterEnd = m_plistPlayers->end();
	int iPlayer = 0;
	while (playerIter != playerIterEnd)
	{
		//Check if any players are alive
		if (playerIter->second->GetIsAlive() == true)
		{
			bGameOver = false;
		}

		//If we in single player check the life count as well
		if (m_bSinglePlayer)
		{
			//Check if any players have lives left
			if (playerIter->second->GetLifeCount() > 0)
			{
				bGameOver = false;
			}
		}
		playerIter++;
	}

	return bGameOver;
}

//Create and Delete Objects

void CGame::CreateEnemy(ClientDataPacket* _pClientPacket)
{
	EnemyStates EnemyInfo = _pClientPacket->singleEnemyInfo;

	std::pair<std::map<UINT, CEnemyObj*>::iterator, bool> MapEnemyIter;

	//Create enemy object
	CEnemyObj* tempEnemy = new CEnemyObj(EnemyInfo.Etype);
	//Initialise the enemy Object with the Cube Mesh and set its coordinates
	std::map<eEnemyTypes, CMesh* >::iterator EnemyMesh = m_pEnemyMesh->find(EnemyInfo.Etype);
	std::map<eEnemyTypes, UINT>::iterator enemyMatID = m_iEnemyIDs->find(EnemyInfo.Etype);
	tempEnemy->Initialise(bToggle, m_pRenderManager, enemyMatID->second, EnemyMesh->second, EnemyInfo.uiEnemyID, EnemyInfo.f3Positions);

	//Add the player to the map
	MapEnemyIter = m_pListEnemies->insert(std::pair<UINT, CEnemyObj*>(EnemyInfo.uiEnemyID, tempEnemy));

	//if the Enemy wasnt added 
	if (MapEnemyIter.second == false)
	{
		//delete the temp variable
		delete tempEnemy;
		tempEnemy = 0;

		return;
	}
	else
	{
		if (tempEnemy->GetEnemyType() == ET_WRATH)
		{
			m_pRenderManager->UpdateSpotLight(m_iSpotLightID, true, tempEnemy->GetPosition(), tempEnemy->GetDirection());
		}
	}

}

void CGame::CreatePowerUp(ClientDataPacket* _pClientPacket)
{
	PowerUpStates PowUpInfo = _pClientPacket->singlePowUpInfo;

	std::pair<std::map<UINT, CPowerUpObj*>::iterator, bool> MapPowUpIter;

	//Create Power Up object
	CPowerUpObj* tempPowUp = new CPowerUpObj(PowUpInfo.Etype);
	//Initialise the Power Up Object with the Cube Mesh and set its coordinates
	std::map<ePowerType, CMesh* >::iterator powerUpMatMesh = m_pPowerUpMesh->find(PowUpInfo.Etype);
	std::map<ePowerType, UINT>::iterator powerUpMatID = m_iPowerUpIDs->find(PowUpInfo.Etype);
	if (powerUpMatID != m_iPowerUpIDs->end() && powerUpMatMesh != m_pPowerUpMesh->end())
	{
		tempPowUp->Initialise(bToggle, m_pRenderManager, powerUpMatID->second, powerUpMatMesh->second, PowUpInfo.uiPowUpID, PowUpInfo.f3Positions);

		//Add the player to the map
		MapPowUpIter = m_pListPowerUps->insert(std::pair<UINT, CPowerUpObj*>(PowUpInfo.uiPowUpID, tempPowUp));
	}
	else
	{
		//delete the temp variable
		delete tempPowUp;
		tempPowUp = 0;
		
	}
	//if the Enemy wasnt added 
	if (MapPowUpIter.second == false)
	{
		//delete the temp variable
		delete tempPowUp;
		tempPowUp = 0;
	}
}
	
void CGame::CreateProjectile(ClientDataPacket* _pClientPacket)
{
	ProjectileStates ProjectileInfo = _pClientPacket->singleProjectileInfo;

	std::pair<std::map<UINT, CProjectileObj*>::iterator, bool> MapBulletIter;

	//Create enemy object
	CProjectileObj* tempProj = new CProjectileObj();
	//Initialise the enemy Object with the Cube Mesh and set its coordinates
	tempProj->Initialise(bToggle, m_pRenderManager, m_iBulletMaterialID, m_pProjectileMesh, ProjectileInfo.uiProjectileID, ProjectileInfo.f3Positions);

	//Add the player to the map
	MapBulletIter = m_pListBullets->insert(std::pair<UINT, CProjectileObj*>(ProjectileInfo.uiProjectileID, tempProj));

	//if the projectile wasnt added 
	if (MapBulletIter.second == false)
	{
		//delete the temp variable
		delete tempProj;
		tempProj = 0;
	}
}

void CGame::DeleteEnemy(ClientDataPacket* _pClientPacket)
{
	EnemyStates EnemyInfo = _pClientPacket->singleEnemyInfo;

	std::map<UINT, CEnemyObj*>::iterator enemyToRemove = m_pListEnemies->find(EnemyInfo.uiEnemyID);
	if (enemyToRemove == m_pListEnemies->end())
	{
		//Already deleted?
		int c = 9;
	}
	else
	{
		if (enemyToRemove->second->GetEnemyType() == ET_WRATH)
		{
			m_pRenderManager->UpdateSpotLight(m_iSpotLightID, false, enemyToRemove->second->GetPosition(), enemyToRemove->second->GetDirection());
		}

		//Delete the object
		delete enemyToRemove->second;
		enemyToRemove->second = 0;
		//Remove from list
		m_pListEnemies->erase(EnemyInfo.uiEnemyID);
	}
}
	
void CGame::DeletePowerUp(ClientDataPacket* _pClientPacket)
{
	PowerUpStates PowUpInfo = _pClientPacket->singlePowUpInfo;

	std::map<UINT, CPowerUpObj*>::iterator PowUpToRemove = m_pListPowerUps->find(PowUpInfo.uiPowUpID);
	if (PowUpToRemove == m_pListPowerUps->end())
	{
		//Already deleted?
		int c = 9;
	}
	else
	{
		//Delete the object
		delete PowUpToRemove->second;
		PowUpToRemove->second = 0;
		//Remove from list
		m_pListPowerUps->erase(PowUpInfo.uiPowUpID);
	}
}
	
void CGame::DeleteProjectile(ClientDataPacket* _pClientPacket)
{
	ProjectileStates ProjectileInfo = _pClientPacket->singleProjectileInfo;

	std::map<UINT, CProjectileObj*>::iterator bullerToRemove = m_pListBullets->find(ProjectileInfo.uiProjectileID);
	if (bullerToRemove == m_pListBullets->end())
	{
		//Already deleted?
		int c = 9;
	}
	else
	{
		//Delete the object
		delete bullerToRemove->second;
		bullerToRemove->second = 0;
		//Remove from list
		m_pListBullets->erase(ProjectileInfo.uiProjectileID);
	}

}


void CGame::ToggleCamera()
{
	m_pCamera->ToggleType();
}

