#include "EchoServer.h"
using namespace std;

int main(int argc, const char *argv[])
{
    EchoServer server(InetAddress(8989));
    server.start();
    return 0;
}
