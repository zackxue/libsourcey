#include "scy/application.h"
#include "scy/logger.h"
#include "udpechoserver.h"
#include "tcpechoserver.h"


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


using std::endl;
using namespace scy;
using namespace scy::net;


const std::uint16_t TcpPort = 1337;
const std::uint16_t SslPort = 1338;
const std::uint16_t UdpPort = 1339;


struct Servers
{
    UDPEchoServer udp;
    TCPEchoServer tcp;
    TCPEchoServer ssl;
};


static void onShutdown(void* opaque)
{
    auto srvs = reinterpret_cast<Servers*>(opaque);
    srvs->udp.shutdown();
    srvs->tcp.shutdown();
    srvs->ssl.shutdown();
}


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    Logger::instance().setWriter(new AsyncLogWriter);
    {
        Servers srvs;
        srvs.udp.start("0.0.0.0", UdpPort);
        srvs.tcp.start("0.0.0.0", TcpPort);
        srvs.ssl.start("0.0.0.0", SslPort);

        InfoL << "UDP Server listening on " << srvs.udp.socket->address() << endl;
        InfoL << "TCP Server listening on " << srvs.tcp.socket->address() << endl;
        InfoL << "SSL Server listening on " << srvs.ssl.socket->address() << endl;

        uv::waitForShutdown(onShutdown, &srvs);
    }
    Logger::destroy();
    return 0;
}
