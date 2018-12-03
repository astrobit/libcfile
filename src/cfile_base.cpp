#define __STDC_WANT_LIB_EXT1__ 1
#include <cstdio>
#include <cfile.hpp>
#include <cstdarg>
#include <cfile_inst.hpp>
#include <cstring>

using namespace cfile;

//cfile_base *				new_cfile(const char * i_pFilename, const char * i_pAccess);
//cfile_base *				new_cfile_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type);
//void						delete_cfile(cfile_base * i_pUnique);
//void						delete_cfile_deleter(void * i_pUnique);


cfile_base * __stdcall	cfile::new_cfile(const char * i_pFilename, const char * i_pAccess)
{
	cfile_base_inst * pInst = new cfile_base_inst; //reinterpret_cast<shared_file_inst*>(malloc(sizeof(shared_file_inst)));
	if (pInst != nullptr)
	{
			//printf("constructing\n");
//		pInst->construct();
		if (i_pFilename != nullptr && i_pFilename[0] != 0)
		{
			if (i_pAccess != nullptr && i_pAccess[0] != 0)
			{
				//printf("opening (f,a)\n");
				pInst->open(i_pFilename,i_pAccess);
			}
			else
			{
				//printf("openinf (f,'rt')\n");
				pInst->open(i_pFilename,"rt");
			}
		}
	}
	return pInst;// reinterpret_cast<cfile_base *>(pInst);
}
cfile_base * __stdcall	cfile::new_cfile_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type)
{
	cfile_base_inst * pInst = new cfile_base_inst; //reinterpret_cast<shared_file_inst*>(malloc(sizeof(shared_file_inst)));
	if (pInst != nullptr)
	{
//		pInst->construct();
		pInst->open_enum(i_pFilename,i_eAccess_Mode,i_eData_Type);
	}
	return pInst;// reinterpret_cast<cfile_base *>(pInst);
}

void __stdcall cfile::delete_cfile(cfile_base * i_pFile)
{
	if (i_pFile != nullptr)
	{
		//cfile_base_inst * pInst = reinterpret_cast<cfile_base_inst *>(i_pFile);
//		pInst->destruct();
		delete i_pFile;
//		free(pInst);
	}
}



cfile_base_inst::cfile_base_inst(void) : m_pFile(), m_pMutex()
{
}

cfile_base_inst::~cfile_base_inst(void)
{
	close();
}


bool cfile_base_inst::open(const char *i_sFile, const char * i_sAccess_Type)
{
	bool bRet = false;
	//printf("in open\n");
	if (close() && i_sFile != nullptr && i_sFile[0] != 0 && i_sAccess_Type != nullptr)
	{
		//printf("open: opening file\n");
		FILE * pFile = nullptr;
#if (defined (_MSC_VER) && (_MSC_VER > 1200)) // this is a guess as to version
		errno_t nErr = fopen_s(&pFile, i_sFile, i_sAccess_Type);
		bRet = nErr == 0;
#elif defined(__STDC_LIB_EXT1__)
		errno_t nErr = std::fopen_s(&pFile, i_sFile, i_sAccess_Type);
		bRet = nErr == 0;
#else
		pFile = std::fopen(i_sFile, i_sAccess_Type);
		bRet = (pFile != nullptr);
#endif
		if (bRet)
		{
			bRet = false; // not ready to say we've succeeded
			std::shared_ptr<FILE> pFileNew = std::shared_ptr<FILE>(pFile, &std::fclose);
			if (pFileNew != nullptr)
			{
				//printf("open: allocating mutex\n");
				std::shared_ptr<std::mutex> mtx = std::make_shared<std::mutex>();
				if (mtx != nullptr)
				{
					//printf("open: lock and swap\n");
					std::lock_guard<std::mutex> lock(*mtx.get());
					m_pMutex = mtx;
					m_pFile = pFileNew;
					bRet = (m_pFile != nullptr);
				}
				else
					pFileNew.reset(); // if we can't get the mutex, close the file.
			}
		}
	}
	return bRet;
}
bool cfile_base_inst::close(void)
{
	//fprintf(stdout,"in close\n");
	if (m_pFile != nullptr)
	{
		std::mutex * pMutex = m_pMutex.get();
		if (pMutex != nullptr)
			//fprintf(stdout,"close: file non null\n");
		{
			//fprintf(stdout,"close: mutex non null\n");
			std::lock_guard<std::mutex> lock(*pMutex); // hold mutex while file is closed
			m_pFile.reset();
		} // mutex released
	}
	if (m_pMutex != nullptr)
		m_pMutex.reset();
	return m_pFile == nullptr;
}

bool cfile_base_inst::flush(void) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile)
		{
			bRet = (std::fflush(pFile) == 0);
		}
	}
	return bRet;
}


bool cfile_base_inst::open_enum(const char * i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type)
{
	char pMode[4] = {0,0,0,0};
	switch (i_eAccess_Mode)
	{
	case access_mode::read_update:
		pMode[1] = '+';
	default:
	case access_mode::read:
		pMode[0] = 'r';
		break;
	case access_mode::write_update:
		pMode[1] = '+';
	case access_mode::write:
		pMode[0] = 'w';
		break;
	case access_mode::append_update:
		pMode[1] = '+';
	case access_mode::append:
		pMode[0] = 'a';
		break;
	}
	size_t	tIdx = 1;
	if (pMode[1] != 0)
		tIdx = 2;

	if (i_eData_Type == data_type::binary)
		pMode[tIdx] = 'b';
	else
		pMode[tIdx] = 't';
	return open(i_sFile,pMode);
}
size_t cfile_base_inst::read(void * o_lpBuffer, size_t i_nSize_Bytes) const
{
	size_t nRet = 0;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			nRet = std::fread(o_lpBuffer,i_nSize_Bytes,1,pFile);
		}
	}
	return nRet;
}
size_t cfile_base_inst::write(void * o_lpBuffer, size_t i_nSize_Bytes) const
{
	size_t nRet = 0;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			nRet = std::fwrite(o_lpBuffer,i_nSize_Bytes,1,pFile);
		}
	}
	return nRet;
}
size_t cfile_base_inst::printf(const char * i_sFormat, ...) const
{
	size_t nRet = 0;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			va_list args;
			va_start(args, i_sFormat);
			nRet = static_cast<size_t>(std::vfprintf(pFile,i_sFormat, args));
			va_end(args);
		}
	}
	return nRet;
}

size_t cfile_base_inst::scanf(const char * i_sFormat, ...) const
{
	size_t nRet = 0;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			va_list args;
			va_start(args, i_sFormat);
			nRet = static_cast<size_t>(std::vfscanf(pFile,i_sFormat, args));
			va_end(args);
		}
	}
	return nRet;
}

const char * cfile_base_inst::gets(size_t i_tSize_Bytes) const
{
	std::string sRet;

	int nC;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			bool bEOL = false;
			// read to EOF or end of line
			while (!bEOL && sRet.size() < i_tSize_Bytes && (nC = std::fgetc(pFile)) != EOF)
			{
				bEOL = (nC == 10 || nC == 13);
				sRet.push_back(static_cast<char>(nC));
			}
			// read the reast of the end of line or to EOF
			while (bEOL && sRet.size() < i_tSize_Bytes && (nC = std::fgetc(pFile)) != EOF)
			{
				bEOL = (nC == 10 || nC == 13);
				if (bEOL)
					sRet.push_back(static_cast<char>(nC));
				else
					std::fseek(pFile,-1,SEEK_CUR);
			}
		}
	}
	size_t nAlloc = sRet.size() + 2;
	char * lpRet = allocate_string(nAlloc);
#if (defined(__STDC_LIB_EXT1__) || (defined(_MSC_VER) && _MSC_VER > 1200))
	strcpy_s(lpRet, nAlloc,sRet.c_str());
#else
	strcpy(lpRet, sRet.c_str());
#endif
	return lpRet;
}

const char * cfile_base_inst::gets_stripped(size_t i_tSize_Bytes) const
{
	std::string sRet;
	int nC;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			bool bEOL = false;
			// read to EOF or end of line
			while (!bEOL && sRet.size() < i_tSize_Bytes && (nC = std::fgetc(pFile)) != EOF)
			{
				bEOL = (nC == 10 || nC == 13);
				sRet.push_back(static_cast<char>(nC));
			}
			// read the reast of the end of line or to EOF
			while (bEOL && sRet.size() < i_tSize_Bytes && (nC = std::fgetc(pFile)) != EOF)
			{
				bEOL = (nC == 10 || nC == 13);
				if (!bEOL)
					std::fseek(pFile,-1,SEEK_CUR);
			}
		}
	}
	size_t nAlloc = sRet.size() + 2;
	char * lpRet = allocate_string(nAlloc);
#if (defined(__STDC_LIB_EXT1__) || (defined(_MSC_VER) && _MSC_VER > 1200))
	strcpy_s(lpRet, nAlloc, sRet.c_str());
#else
	strcpy(lpRet, sRet.c_str());
#endif
	return lpRet;
}

size_t cfile_base_inst::puts(const char * i_sString) const
{
	size_t tRet = 0;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			tRet = std::fwrite(i_sString,strlen(i_sString),1,pFile);
		}
	}
	return tRet;
}

fpos_t cfile_base_inst::getpos(void) const
{
	bool bSuccess = false;
	fpos_t cPos;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			std::fgetpos(pFile,&cPos);
			bSuccess = true;
		}
	}
	if (!bSuccess)
		memset(&cPos, 0, sizeof(cPos));
	return cPos;
}

bool cfile_base_inst::setpos(fpos_t i_cPos) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			bRet = (std::fsetpos(pFile,&i_cPos) == 0);
		}
	}
	return bRet;
}

bool cfile_base_inst::eof(void) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
		{
			bRet = (std::feof(pFile) != 0);
		}
	}
	return bRet;
}

bool cfile_base_inst::error(void) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			bRet = (std::ferror(pFile) != 0);
	}
	return bRet;
}

bool cfile_base_inst::rewind(size_t i_nDistance) const
{
	bool bRet = false;
	if  (i_nDistance == static_cast<size_t>(-1))
	{
		std::mutex * pMutex = m_pMutex.get();
		if (pMutex != nullptr)
		{
			std::lock_guard<std::mutex> lock(*pMutex);
			FILE * pFile = m_pFile.get();
			if (pFile != nullptr)
				bRet = (std::fseek(pFile,0,SEEK_SET) == 0);
		}
	}
	else
	{
		long int nDistance = static_cast<long int>(i_nDistance);
		nDistance *= -1;
		if (nDistance > 0) // sign change - need to do two rewinds
		{
			rewind(i_nDistance >> 1);
			nDistance = -1 * static_cast<long int>(i_nDistance - (i_nDistance >> 1));
		}
		std::mutex * pMutex = m_pMutex.get();
		if (pMutex != nullptr)
		{
			std::lock_guard<std::mutex> lock(*pMutex);
			FILE * pFile = m_pFile.get();
			if (pFile != nullptr)
				bRet = (std::fseek(pFile,nDistance,SEEK_CUR) == 0);
		}
	}
	return bRet;
}
bool cfile_base_inst::fast_forward(size_t i_nDistance) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,static_cast<long int>(i_nDistance),SEEK_CUR) == 0);
	}
	return bRet;
}
bool cfile_base_inst::seek(long int i_nDistance, int i_nOrigin) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,i_nDistance,i_nOrigin) == 0);
	}
	return bRet;
}
bool cfile_base_inst::rewind_to_start(void) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,0,SEEK_SET) == 0);
	}
	return bRet;
}
bool cfile_base_inst::forward_to_end(void) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,0,SEEK_END) == 0);
	}
	return bRet;
}
void cfile_base_inst::clear_error(void) const
{
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			std::clearerr(pFile);
	}
}

char cfile_base_inst::getc(void) const
{
	char nRet = EOF;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			nRet = static_cast<char>(std::fgetc(pFile));
	}
	return nRet;
}

char cfile_base_inst::putc(char i_nChar) const
{
	char nRet = EOF;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			nRet = static_cast<char>(std::fputc(i_nChar,pFile));
	}
	return nRet;
}

size_t cfile_base_inst::tell(void) const
{
	size_t nRet = 0;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = m_pFile.get();
		if (pFile != nullptr)
			nRet = static_cast<size_t>(std::ftell(pFile));
	}
	return nRet;
}

bool cfile_base_inst::is_open(void) const
{
	bool bRet = false;
	std::mutex * pMutex = m_pMutex.get();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		bRet = (m_pFile.get() != nullptr);
	}
	return bRet;
}
