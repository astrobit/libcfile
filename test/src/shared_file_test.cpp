#include <cfile.hpp>
#include <cstring>


void test_shared_file(cfile::shared_file * pFile)
{
	if (pFile != nullptr)
	{
		pFile->open("testfile.txt","wt");
//		printf("opened (write)\n");
		pFile->puts("this is a string\n");
//		printf("puts\n");
		pFile->close();
//		printf("close (write)\n");
		pFile->open("testfile.txt","rt");
//		printf("opened (read)\n");
		std::string sString = pFile->gets();
//		printf("gets %s\n",sString.c_str());
		if(strcmp(sString.c_str(),"this is a string\n") == 0)
			printf("read/write match\n");
		else
			printf("read/write mismatch\n");
		pFile->close();
//		printf("close (read)\n");
	}
}

int main(int i_iNum_Args, char * i_ppArgs[])
{

	cfile::shared_file cSF;
//	printf("file instantiated\n");
	test_shared_file(&cSF);

	cfile::shared_file *pSF = new cfile::shared_file;
	if (pSF != nullptr)
	{
//		printf("SF file alocated\n");
		test_shared_file(pSF);
		delete pSF;
//		printf("SF file deleted\n");
		pSF = nullptr;
	}
	else
		printf("new SF failed\n");

	cfile::shared_file pSFA = new cfile::shared_file[3];
	if (pSFA != nullptr)
	{
//		printf("SF file array alocated\n");
		for (size_t nI = 0; nI < 3; nI++)
			test_shared_file(&pSFA[nI]);
		delete [] pSFA;
//		printf("SF file array deleted\n");
		pSF = nullptr;
	}
	else
		printf("new SF array failed\n");

	return 0;
}
