//
//  TestPeerConnectionClient.h
//  TestPeerConnectionClient
//
//  Created by Manjesh Malavalli on 10/14/11.
//  Copyright 2011 XVDTH. All rights reserved.
//

#ifndef TestPeerConnectionClient_TestPeerConnectionClient_h
#define TestPeerConnectionClient_TestPeerConnectionClient_h

#include <map>
#include <string>
#include "WPLThreadSafeMessageQueue.h"
#include "WPLCall.h"
#include "talk/base/sigslot.h"
#include "talk/base/scoped_ptr.h"
#include "talk/base/physicalsocketserver.h"

typedef std::map<int, std::string> Peers;
typedef std::map<std::string, std::string> ParsedCommand;

namespace GoCast
{
    /**
    	Handles p2p signaling and voice connection management.
        This class has been designed to interact with WebRTC's
        sample 'peerconnection_server'. The interaction involves
        registering the client's peer name, followed by receiving
        a unique peer id. Right after login, and from time to time,
        it also receives notifications from the server when someone
        comes online or goes offline. It also manages instances of
        the GoCast::PeerConnectionObserver class, which in turn
        manages the creation/teardown of the capture/encode/transmit
        and receive/decode/render pipelines. GoCast::PeerConnectionClient
        receives signaling messages from its observers and forwards them
        to the server, which in turn forwards them to the appropriate peers.
        It also forwards the signaling messages t receives from the server
        and forwards them to the appropriate observers.
     
        NOTE: There are two aspects to setting up a successful connection 
              between peers. One is having a mechanism to exchange signaling 
              messages between peers. This is achieved here through the peers 
              signing in to a common server, and forwarding any signaling 
              messages to it, as it knows the location of the intended recipients.
              All this is handled through TCP connections. The second aspect is
              the voice data connection between peers, which is handled through
              RTP/RTCP(UDP) connections, with STUN/TURN/ICE for NAT traversal.
              GoCast::PeerConnectionClient handles the first aspect, while
              GoCast::PeerConnectionObserver handles the second.
     */
    class PeerConnectionClient : public sigslot::has_slots<>
    {
    public:
        /**
        	Describes the different states of the GoCast::PeerConnectionClient
            instance.
         */
        enum State
        {
            NOT_CONNECTED,
            SIGNING_IN,
            CONNECTED,
            SIGNING_OUT_WAITING,
            SIGNING_OUT,
        };
        
        /**
        	Constructor.
        	@param pMsgQ Message queue to store commands to be executed.
        	@param pEvtQ Event queue to store generated events that are to be fired into JavaScript.
        	@param peerName Unique peer name
        	@param serverLocation Public IP of the signin server
        	@param serverPort Public port on which the signin server is listening.
        	@returns N/A
         */
        PeerConnectionClient(ThreadSafeMessageQueue* pMsgQ,
                             ThreadSafeMessageQueue* pEvtQ,
                             const std::string& peerName,
                             const std::string& serverLocation,
                             const int serverPort);
        
        /**
        	Destructor
        	@returns N/A
         */
        virtual ~PeerConnectionClient();
        
        /**
        	Returns the unique peer id
        	@returns int
         */
        int id(void) const;
        
        bool is_connected() const;
        const Peers& peers() const;
        bool Connect(const std::string& server, int port,
                     const std::string& client_name);
        bool SendToPeer(int peer_id,
                        const std::string& message);
        bool SignOut();
        bool ExecuteNextCommand(bool& bQuitCommand);

    protected:
        void Close();
        bool ConnectControlSocket();
        void OnConnect(talk_base::AsyncSocket* socket);
        void OnHangingGetConnect(talk_base::AsyncSocket* socket);
        void OnMessageFromPeer(int peer_id, const std::string& message);
        
        // Quick and dirty support for parsing HTTP header values.
        bool GetHeaderValue(const std::string& data, size_t eoh,
                            const char* header_pattern, size_t* value);
        bool GetHeaderValue(const std::string& data, size_t eoh,
                            const char* header_pattern, std::string* value);
        
        // Returns true if the whole response has been read.
        bool ReadIntoBuffer(talk_base::AsyncSocket* socket, std::string* data,
                            size_t* content_length);
        void OnRead(talk_base::AsyncSocket* socket);
        void OnHangingGetRead(talk_base::AsyncSocket* socket);
        
        // Parses a single line entry in the form "<name>,<id>,<connected>"
        bool ParseEntry(const std::string& entry, std::string* name, int* id,
                        bool* connected);
        int GetResponseStatus(const std::string& response);
        bool ParseServerResponse(const std::string& response, size_t content_length,
                                 size_t* peer_id, size_t* eoh);
        void OnClose(talk_base::AsyncSocket* socket, int err);
        
    protected:
        talk_base::SocketAddress server_address_;
        talk_base::scoped_ptr<talk_base::AsyncSocket> control_socket_;
        talk_base::scoped_ptr<talk_base::AsyncSocket> hanging_get_;
        std::string onconnect_data_;
        std::string control_data_;
        std::string notification_data_;
        
    protected:
        Call* m_pCall;
        ThreadSafeMessageQueue* m_pMsgQ;
        ThreadSafeMessageQueue* m_pEvtQ;
        Peers peers_;
        State state_;
        int my_id_;
        
    protected:
        std::string m_PeerName;
        std::string m_ServerLocation;
        int m_ServerPort;
    };
}
#endif
