//
//  TestPeerConnectionObserver.h
//  TestPeerConnectionClient
//
//  Created by Manjesh Malavalli on 10/20/11.
//  Copyright 2011 XVDTH. All rights reserved.
//

#ifndef TestPeerConnectionClient_TestPeerConnectionObserver_h
#define TestPeerConnectionClient_TestPeerConnectionObserver_h

#include "WPLThreadSafeMessageQueue.h"
#include "talk/app/webrtc/peerconnection.h"
#include "talk/app/webrtc/peerconnectionfactory.h"
#include "talk/base/scoped_ptr.h"

namespace GoCast
{
    class PeerConnectionObserver : public webrtc::PeerConnectionObserver
    {
    public:
        //Constructor and destructor
        PeerConnectionObserver(ThreadSafeMessageQueue* pMsgQ);
        virtual ~PeerConnectionObserver();
        
        //Get functions
        bool IsConnectionActive(void) const;
        
        //Set functions
        void SetPeerId(int peerId) { m_PeerId = peerId; }
        void SetPeerName(const std::string& peerName) { m_PeerName = peerName; }
        
        //Class-specific functions
        virtual void OnMessageFromRemotePeer(int peerId, const std::string& msg);
        virtual void ConnectToPeer(int peerId, const std::string& peerName);
        virtual bool DisconnectFromCurrentPeer(void);

    protected:
        //Member functions
        bool InitPeerConnection(void);
        void DeletePeerConnection(void);
        
        //webrtc::PeerConnectionObserver implementation
        virtual void OnError(void);
        virtual void OnMessage(const std::string& msg) {}
        virtual void OnSignalingMessage(const std::string& msg);
        virtual void OnAddStream(const std::string& streamId, bool video);
        virtual void OnRemoveStream(const std::string& streamId, bool video);
        
        //Class-specific functions
        virtual void ShareLocalAudioStream(void);
        
    protected:
        ThreadSafeMessageQueue* m_pMsgQ;
        talk_base::scoped_ptr<webrtc::PeerConnection> m_pPeerConnection;
        talk_base::scoped_ptr<webrtc::PeerConnectionFactory> m_pPeerConnectionFactory;
        talk_base::scoped_ptr<talk_base::Thread> m_pWorkerThread;
        int m_PeerId;
        std::string m_PeerName;
        bool m_bAudioStreamShared;
    };
}

#endif
