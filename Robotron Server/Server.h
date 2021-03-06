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
	* SendData: Send Data packet to a specific Client
	* @author: Jc Fowles
	* @parameter: std::string _strUser: User name of the specific client to send the data to
	* @parameter: ClientDataPacket* _pDataToSend: Data packet to send to the client
	* @return: bool: Successful Sending
	********************/
	bool SendData(std::string _strUser, ClientDataPacket* _pDataToSend);
	
	/***********************
	* SendData: Send Data packet to a specific Socket address
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Data packet to be sent
	* @Parameter: sockaddr_in _socAddrToSendTo: Socket address data is to be sent to
	* @return: bool: Successful Sending
	********************/
	bool SendData(ClientDataPacket* _pDataToSend, sockaddr_in _socAddrToSendTo);
	
	/***********************
	* ReceiveData: Receive data from a client
	* @author: Jc Fowles
	* @parameter: _pReceivedData: Data that will be received
	* @return: bool: Successful Retrieval
	********************/
	bool ReceiveData(ServerDataPacket* _pReceivedData);
	
	/***********************
	* Broadcast: Broadcast the Data
	* @author: Jc Fowles
	* @Parameter: ClientDataPacket * _pDataToSend: Data to be broadcated
	* @return: bool: Successful broadcasting
	********************/
	bool Broadcast(ClientDataPacket* _pDataToSend);
		
	/***********************
	* getActive: Return weather the server is active
	* @author: Jc Fowles
	* @return: bool: Weather the server is active
	********************/
	bool getActive(){ return m_bIsActive; };

	/***********************
	* SetActive: Set weather the server is active
	* @author: Jc Fowles
	* @parameter: _bActive: weather the server is active
	* @return: void
	********************/
	void SetActive(bool _bActive){ m_bIsActive = _bActive; };
		
	/***********************
	* GetHostClientInfo: Returns the Hosting clients information
	* @author: Jc Fowles
	* @return: std::pair<std::string, sockaddr_in>*: Hosting clients information: Username , and socket address
	********************/
	std::pair<std::string, sockaddr_in>* GetHostClientInfo() const { return m_HostClientInfo; }
	
	/***********************
	* SetHostClientInfo: Sets the Hosting clients information
	* @author: Jc Fowles
	* @Parameter: std::string _userName: User name of hosting client
	* @Parameter: sockaddr_in _sockAddr: Socket adder of hosting client
	* @return: void:
	********************/
	void SetHostClientInfo(std::string _userName, sockaddr_in _sockAddr) { *m_HostClientInfo = std::pair < std::string, sockaddr_in >(_userName, _sockAddr); }

	/***********************
	* GetServerSocketAddress: Get the Server Socket Address
	* @author: Jc Fowles
	* @return: sockaddr_in: The server Socket Address
	********************/
	sockaddr_in GetServerSocketAddress() const { return m_ServerSocketAddress; }
		
	//Member variables
public:
	
private:

	bool m_bIsActive;
	bool m_bWaitOnRecieve;

	SOCKET m_hServerSocket;
	sockaddr_in m_ServerSocketAddress;
		
	std::pair<std::string, sockaddr_in>* m_HostClientInfo;
		
};

#endif //__CSERVER_H__