#ifndef AL_PARAMETERSERVER_H
#define AL_PARAMETERSERVER_H

/*	Allolib --
	Multimedia / virtual environment application class library

	Copyright (C) 2009. AlloSphere Research Group, Media Arts & Technology, UCSB.
    Copyright (C) 2012-2018. The Regents of the University of California.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

		Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.

		Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

		Neither the name of the University of California nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.

	File description:
    Expose parameters on the network
	File author(s):
	Andrés Cabrera mantaraya36@gmail.com
*/

#include <mutex>

#include "al/core/protocol/al_OSC.hpp"
#include "al/util/ui/al_Parameter.hpp"
#include "al/util/ui/al_ParameterBundle.hpp"

namespace al
{

constexpr int handshakeServerPort = 16987;
constexpr int listenerFirstPort = 14000;

class OSCNode {
public:
    OSCNode() {
        mCommandHandler.node = this;
    }

    void startCommandListener(std::string address = "0.0.0.0") {
        int offset = 0;
        while(!mNetworkListener.open(listenerFirstPort + offset, address.c_str()) & (offset < 128) ) {
            offset++;
        }
        if (offset < 128) {
            mNetworkListener.start();
            mNetworkListener.handler(mCommandHandler);
            std::cout << " OSCNotifier listening on " << address << ":" << listenerFirstPort + offset << std::endl;
        } else {
            std::cerr << "Could not start listener on address " << address << std::endl;
        }

        // Broadcast handshake
        // FIXME broadcast on all network interfaces
        osc::Send handshake(handshakeServerPort, "127.0.0.1");
        handshake.send("/handshake", listenerFirstPort + offset);
    }
protected:
    virtual void runCommand(osc::Message &m) = 0;

private:

    class CommandHandler: public osc::PacketHandler {
    public:
        OSCNode *node;
        virtual void onMessage(osc::Message &m) override {
            std::cout << "command handler got" << std::endl;
//            m.print();
            node->runCommand(m);
        }
    } mCommandHandler;
    osc::Recv mNetworkListener;

};

class OSCNotifier {
public:
    OSCNotifier();
    virtual ~OSCNotifier();

    /**
     * @brief addListener enables notifiying via OSC that a preset has changed
     * @param IPaddress The IP address of the listener
     * @param oscPort The network port so send the value changes on
     */
    virtual void addListener(std::string IPaddress, int oscPort) {
        mListenerLock.lock();
        mOSCSenders.push_back(new osc::Send(oscPort, IPaddress.c_str()));
        mListenerLock.unlock();
//		std::cout << "Registered listener " << IPaddress << ":" << oscPort<< std::endl;
    }

    /**
     * @brief Notify the listeners of value changes
     * @param OSCaddress The OSC path to send the value on
     * @param value The value to send
     *
     * This will send all registered data to the listeners. This is useful to
     * force a resfresh of an interface, e.g. when it just came online and is
     * unaware of state. Otherwise, when calling addListener, you should
     * register to be notified when the data changes to only do notifications
     * then.
     *
     */
    void notifyListeners(std::string OSCaddress, float value);
    void notifyListeners(std::string OSCaddress, int value);
    void notifyListeners(std::string OSCaddress, std::string value);
    void notifyListeners(std::string OSCaddress, Vec3f value);
    void notifyListeners(std::string OSCaddress, Vec4f value);
    void notifyListeners(std::string OSCaddress, Pose value);
    void notifyListeners(std::string OSCaddress, Color value);

    void notifyListeners(std::string OSCaddress, ParameterMeta *param);

    void send(osc::Packet &p) {
        mListenerLock.lock();
        for(osc::Send *sender: mOSCSenders) {
            sender->send(p);
            std::cout << "Notifying " << sender->address() << ":" << sender->port() << std::endl;
        }
        mListenerLock.unlock();
    }

    void startHandshakeServer(std::string address = "0.0.0.0") {
        if (mHandshakeServer.open(handshakeServerPort, address.c_str())) {
            mHandshakeServer.handler(mHandshakeHandler);
            mHandshakeServer.start();
            std::cout << "Handshake server running on " << address << ":" << handshakeServerPort << std::endl;
        } else {
            std::cout << "failed to start handshake server" << std::endl;
        }
        for (int i = 0; i < 100; i++) { // Check to see if there are any listeners already running
            osc::Send handshake(listenerFirstPort + i, "127.0.0.1");
            handshake.send("/requestHandshake", handshakeServerPort);
        }
    }

protected:
    std::mutex mListenerLock;
    std::vector<osc::Send *> mOSCSenders;

    class HandshakeHandler: public osc::PacketHandler {
    public:
        OSCNotifier *notifier;
        virtual void onMessage(osc::Message &m) override {
            if (m.addressPattern() == "/handshake" && m.typeTags() == "i") {
                std::unique_lock<std::mutex> lk(notifier->mNodeLock);
                int commandPort;
                m >> commandPort;
                notifier->mNodes.push_back({m.senderAddress(), commandPort});
                std::cout << "handshake from " << m.senderAddress() << ":" << commandPort << std::endl;
                
                osc::Send listenerRequest(commandPort, m.senderAddress().c_str());
                listenerRequest.send("/requestListenerInfo", notifier->mHandshakeServer.port());
            } else if (m.addressPattern() == "/registerListener" && m.typeTags() == "i") {
                int listenerPort;
                m >> listenerPort;
                notifier->addListener(m.senderAddress(), listenerPort);
                std::cout << "Registered listener " << m.senderAddress() << ":" << listenerPort << std::endl;
            } else {
                std::cout << "Unhandled command" << std::endl;
                m.print();
            }
        }
    } mHandshakeHandler;

    osc::Recv mHandshakeServer;

    std::vector<std::pair<std::string, uint16_t>> mNodes;
    std::mutex mNodeLock;
private:


};


/**
 * @brief The ParameterServer class creates an OSC server to receive parameter values
 *
 * Parameter objects that are registered with a ParameterServer will receive
 * incoming messages on their OSC address.
 *
 */
class ParameterServer : public osc::PacketHandler, public OSCNotifier, public OSCNode
{
    friend class PresetServer; // To be able to take over the OSC server
public:
    /**
     * @brief ParameterServer constructor
     *
     * @param oscAddress The network address on which to listen to. If empty use all available network interfaces. Defaults to "127.0.0.1".
     * @param oscPort The network port on which to listen. Defaults to 9010.
     *
     * Usage:
     * @code
    Parameter freq("Frequency", "", 440.0);
    Parameter amp("Amplitude", "", 0.1);
    ParameterServer paramServer;
    paramServer << freq << amp;
     @endcode
     */
    // ParameterServer() : mServer(nullptr) {};
    ParameterServer(std::string oscAddress = "", int oscPort = 9010);
    ~ParameterServer();

    /**
     * Open and start receiving osc
     */
    void listen(int oscPort, std::string oscAddress = "");

    /**
     * Register a parameter with the server.
     */
    ParameterServer &registerParameter(ParameterMeta &param);

    /**
     * Remove a parameter from the server.
     */
    void unregisterParameter(ParameterMeta &param);

    /**
     * Register a ParameterBundle parameter with the server.
     */
    ParameterServer &registerParameterBundle(ParameterBundle &bundle);

    /**
     * @brief print prints information about the server to std::out
     *
     * The print function will print the server configuration (address and port)
     * and will list the parameters with their addresses.
     */
    void print(std::ostream &stream = std::cout);

    /**
     * @brief stopServer stops the OSC server thread. Calling this function
     * is sometimes required when this object is destroyed abruptly and the
     * destructor is not called
     */
    void stopServer();

    bool serverRunning() { return (mServer != nullptr); }

    /**
     * @brief Get the list of registered parameters.
     */
    std::vector<Parameter *> parameters();
    std::vector<ParameterString *> stringParameters();
    std::vector<ParameterVec3 *> vec3Parameters();
    std::vector<ParameterVec4 *> vec4Parameters();
    std::vector<ParameterPose *> poseParameters();

    /// Register parameter using the streaming operator
    template<class ParameterType>
    ParameterServer &operator << (ParameterType& newParam){ return registerParameter(newParam); }

    /// Register parameter using the streaming operator
    template<class ParameterType>
    ParameterServer &operator << (ParameterType* newParam){ return registerParameter(*newParam); }

    /// Register parameter bundle using the streaming operator
    ParameterServer &operator << (ParameterBundle& bundle){ return registerParameterBundle(bundle); }

    /**
     * @brief Append a listener to the osc server.
     * @param handler
     * OSC messages received by this server will be forwarded to all
     * registered listeners. This is the mechanism internally used to share a
     * network port between a ParameterServer, a PresetServer and a SequenceServer
     */
    void registerOSCListener(osc::PacketHandler *handler);

    void registerOSCConsumer(osc::MessageConsumer *consumer, std::string rootPath = "");

    void notifyAll();

        /**
         * @brief send all currently regeistered parameter values
         * @param IPaddress
         * @param oscPort
         */
        void sendAllParameters(std::string IPaddress, int oscPort);

    virtual void onMessage(osc::Message& m) override;

    uint16_t serverPort() {return mServer->port();}

    void verbose(bool verbose= true) { mVerbose = verbose;}
    static bool setParameterValueFromMessage(ParameterMeta *param, std::string address, osc::Message &m);

protected:
    static void changeCallback(float value, void *sender, void *userData, void *blockThis);
    static void changeStringCallback(std::string value, void *sender, void *userData, void *blockThis);
    static void changeVec3Callback(Vec3f value, void *sender, void *userData, void *blockThis);
    static void changeVec4Callback(Vec4f value, void *sender, void *userData, void *blockThis);
    static void changePoseCallback(Pose value, void *sender, void *userData, void *blockThis);

    virtual void runCommand(osc::Message &m) override {
        if (m.addressPattern() == "/requestListenerInfo") {
            if (m.typeTags() == "i") {
                int port;
                m >> port;
                mNotifiers.push_back({m.senderAddress(), port});
                std::cout << "Registering primary node: " << m.senderAddress() << ":" << port << std::endl;
                osc::Send listenerRequest(port, m.senderAddress().c_str());
                listenerRequest.send("/registerListener", mServer->port());
            }
        }
    }

    void printParameterInfo(ParameterMeta *p);

    void printBundleInfo(ParameterBundle *bundle, std::string id, int depth = 0);

    void setValuesForBundleGroup(osc::Message &m, std::vector<ParameterBundle *> bundleGroup, std::string rootAddress);

    std::vector<std::pair<std::string, uint16_t>> mNotifiers; // List of primary nodes

    std::vector<osc::PacketHandler *> mPacketHandlers;
    std::vector<std::pair<osc::MessageConsumer *, std::string>> mMessageConsumers;
    osc::Recv *mServer;
    std::vector<ParameterMeta *> mParameters;
    std::map<std::string, std::vector<ParameterBundle *>> mParameterBundles;
    std::map<std::string, int> mCurrentActiveBundle;
    std::mutex mParameterLock;
    bool mVerbose {false};
};

}


#endif // AL_PARAMETERSERVER_H
