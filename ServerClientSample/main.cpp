#undef UNICODE

#include "pch.h"
#include "Server.h"

/*
to run over the internet:
	open firewall on specified port: 5400.
	create static ip address for computer.
	enable port forwarding for that ip and port on router.
	cry.

*/

void callback(double ya, double dso) {

}

int __cdecl main(void)
{
	std::string ip = "192.168.1.44";
	PCSTR port = "5400";

	UserManager clientManager;

	Server Server(port, &clientManager); 

	if (Server.Init()) { //initiate connection
		Server.Run();
	}
	Server.Close();
	return 0;
}