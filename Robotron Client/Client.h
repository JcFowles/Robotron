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
#if !defined(__CCLIENT_H__)
#define __CCLIENT_H__

//library Includes
#include <string>

//Local Includes
#include "../Shared/Network_Defines.h"


class CClient
{
public:
	/***********************
	* CClient: Constructor of the Client instance
	* @author: Jc Fowles
	* @return:
	********************/
	CClient();

	/***********************
	* ~CClient: Destructor of the Client instance
	* @author: Jc Fowles
	* @return:
	********************/
	~CClient();

	/***********************
	* Initialize: Initializes the Client Socket and Address
	* @author: Jc Fowles
	* @return: bool: Succesful Initialisation
	********************/
	bool Initialise();

	/***********************
	* SendData: Send Data packet to the server
	* @author: Jc Fowles
	* @param: _DataToSend: Datapacket to send to the server
	* @return: bool: Succesful Sending
	********************/
	bool SendData(ServerDataPacket* _pDataToSend);

	/***********************
	* ReceiveData: Receive data from the Server
	* @author: Jc Fowles
	* @param: _pReceivedData: Data that will be recieved
	* @return: bool: Succesful Retrieval
	********************/
	bool ReceiveData(ClientDataPacket* _pReceivedData);
	
	/***********************
	* getActive: Return weather the Client is active
	* @author: Jc Fowles
	* @return: bool: Weather the Client is active
	********************/
	bool getActive(){ return m_bIsActive; };

	/***********************
	* SetActive: Set weather the Client is active
	* @author: Jc Fowles
	* @param: _bActive: weather the Client is active
	* @return: void
	********************/
	void SetActive(bool _bActive){ m_bIsActive = _bActive; };


	//Member variables
public:
	//TO DO make private
	bool m_bIsActive;
	
private:

	SOCKET m_hClientSocket;

	sockaddr_in m_ClientSocketAddress;
	sockaddr_in m_ServerSocketAddress;
	
	unsigned int uiServerPort;



	
};

#endif //__CCLIENT_H__