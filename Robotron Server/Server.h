//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CClient.h
// Description : Header file containing declarations for CClient Class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//

#pragma once
#if !defined(__CSERVER_H__)
#define __CSERVER_H__


//Local Includes
#include "../Shared/Network_Defines.h"

class CServer
{
public:
	/***********************
	* CServer: Constructor of the Server instance
	* @author: Jc Fowles
	* @return:
	********************/
	CServer();

	/***********************
	* ~CServer: Destructor of the Server instance
	* @author: Jc Fowles
	* @return:
	********************/
	~CServer();

	/***********************
	* Initialize: Initializes the Server Socket and Address
	* @author: Jc Fowles
	* @return: bool: Successful Initialization
	********************/
	bool Initialise();

	/***********************
	* SendData: Send Data packet to the Client
	* @author: Jc Fowles
	* @param: _DataToSend: Data packet to send to the client
	* @return: bool: Successful Sending
	********************/
	bool SendData(ClientDataPacket* _pDataToSend);

	/***********************
	* ReceiveData: Receive data from the client
	* @author: Jc Fowles
	* @param: _pReceivedData: Data that will be received
	* @return: bool: Successful Retrieval
	********************/
	bool ReceiveData(ServerDataPacket* _pReceivedData);
	
	/***********************
	* getActive: Return weather the server is active
	* @author: Jc Fowles
	* @return: bool: Weather the server is active
	********************/
	bool getActive(){ return m_bIsActive; };

	/***********************
	* SetActive: Set weather the server is active
	* @author: Jc Fowles
	* @param: _bActive: weather the server is active
	* @return: void
	********************/
	void SetActive(bool _bActive){ m_bIsActive = _bActive; };
	

	//Member variables
public:
	
private:

	bool m_bIsActive;
	
	SOCKET m_hServerSocket;

	sockaddr_in m_ServerSocketAddress;
	
	sockaddr_in m_ClientSocketAddress;

	

	bool m_bWaitOnRecieve;
};

#endif //__CSERVER_H__