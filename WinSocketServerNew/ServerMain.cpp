
#include"ServerNet.h"



int main()

{

	ServerNet serverNet;

	int iRlt = serverNet.ServerInit("127.0.0.1", 8888);

	if (iRlt == 0)

	{

		printf("server init successful.\n");

		serverNet.ServerRun();

	}

	else

		printf("server init failed with error: %d\n", iRlt);

	system("pause");

}
