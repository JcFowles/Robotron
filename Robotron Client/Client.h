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

#ifndef __CCLIENT_H__
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
	* @return: bool: Successful Initialization
	********************/
	bool Initialise();

	/***********************
	* SendData: Send Data packet to the server
	* @author: Jc Fowles
	* @param: _DataToSend: Data packet to send to the server
	* @return: bool: Successful Sending
	********************/
	bool SendData(ServerDataPacket* _pDataToSend);

	/***********************
	* ReceiveData: Receive data from the Server
	* @author: Jc Fowles
	* @param: _pReceivedData: Data that will be received
	* @return: bool: Successful Retrieval
	********************/
	bool ReceiveData(ClientDataPacket* _pReceivedData);
		
	/***********************
	* Broadcast: Broadcast the Data
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Data to be broadcated
	* @return: bool: Successful broadcasting
	********************/
	bool Broadcast(ServerDataPacket* _pDataToSend);
	
	/***********************
	* getActive: Return weather the Client is active
	* @author: Jc Fowles
	* @return: bool: Weather the Client is active
	********************/
	bool GetActive(){ return m_bIsActive; };

	/***********************
	* SetActive: Set weather the Client is active
	* @author: Jc Fowles
	* @param: _bActive: weather the Client is active
	* @return: void
	********************/
	void SetActive(bool _bActive){ m_bIsActive = _bActive; };

	/***********************
	* GetClientSocketAddress: Get the Client Socket Address
	* @author: Jc Fowles
	* @return: sockaddr_in: The Client Socket Address
	********************/
	sockaddr_in GetClientSocketAddress() const { return m_ClientSocketAddress; }
	
	/***********************
	* GetServerSocketAddress: Return the socket address of the sever
	* @author: Jc Fowles
	* @return: sockaddr_in: The socket address of the sever
	********************/
	sockaddr_in GetServerSocketAddress() const { return m_ServerSocketAddress; }
	
	/***********************
	* SetServerSocketAddress: Set the socket address of the sever
	* @author: Jc Fowles
	* @Parameter: sockaddr_in _serverSocAddr: socket address to store as the server socket address
	* @return: void:
	********************/
	void SetServerSocketAddress(sockaddr_in _serverSocAddr) { m_ServerSocketAddress = _serverSocAddr; }
	
	/***********************
	* Reset: Reset the client by resetting its server address, as there is no server
	* @author: Jc Fowles
	* @return: void: 
	********************/
	void Reset();

	//Member variables
public:
		
private:

	bool m_bIsActive;
	
	SOCKET m_hClientSocket;
	
	sockaddr_in m_ClientSocketAddress;
	
	sockaddr_in m_ServerSocketAddress;
	
	unsigned int uiServerPort;

	bool m_bWaitOnRecieve;
		
};

#endif //__CCLIENT_H__