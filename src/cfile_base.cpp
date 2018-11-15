#include <cstdio>
#include <cfile.hpp>
#include <cstdarg>

using namespace cfile;


#if ((defined(_MSC_VER) && _MSVC_LANG < 201703L) || (defined(__GNUC__) && __cplusplus < 201701L))
std::mutex cfile_base::m_mtxSwap;
#endif

bool cfile_base::flush(void) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile)
		{
			bRet = (std::fflush(pFile) == 0);
		}
	}
	return bRet;
}
bool cfile_base::open(const std::string &i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type)
{
	std::string i_sMode;
	switch (i_eAccess_Mode)
	{
	default:
	case access_mode::read:
		i_sMode = "r";
		break;
	case access_mode::read_update:
		i_sMode = "r+";
		break;
	case access_mode::write:
		i_sMode = "w";
		break;
	case access_mode::write_update:
		i_sMode = "w+";
		break;
	case access_mode::append:
		i_sMode = "a";
		break;
	case access_mode::append_update:
		i_sMode = "a+";
		break;
	}
	if (i_eData_Type == data_type::binary)
		i_sMode.push_back('b');
	else
		i_sMode.push_back('t');
	return open(i_sFile,i_sMode);
}
size_t cfile_base::read(void * o_lpBuffer, size_t i_nSize_Bytes) const
{
	size_t nRet = 0;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			nRet = std::fread(o_lpBuffer,i_nSize_Bytes,1,pFile);
		}
	}
	return nRet;
}
size_t cfile_base::write(void * o_lpBuffer, size_t i_nSize_Bytes) const
{
	size_t nRet = 0;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			nRet = std::fwrite(o_lpBuffer,i_nSize_Bytes,1,pFile);
		}
	}
	return nRet;
}
size_t cfile_base::printf(const std::string & i_sFormat, ...) const
{
	size_t nRet = 0;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			va_list args;
			va_start(args, i_sFormat);
			nRet = static_cast<size_t>(std::vfprintf(pFile,i_sFormat.c_str(), args));
			va_end(args);
		}
	}
	return nRet;
}

size_t cfile_base::scanf(const std::string & i_sFormat, ...) const
{
	size_t nRet = 0;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			va_list args;
			va_start(args, i_sFormat);
			nRet = static_cast<size_t>(std::vfscanf(pFile,i_sFormat.c_str(), args));
			va_end(args);
		}
	}
	return nRet;
}

std::string cfile_base::gets(size_t i_tSize_Bytes) const
{
	std::string sRet;
	char nC;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			bool bEOL = false;
			// read to EOF or end of line
			while (!bEOL && sRet.size() < i_tSize_Bytes && (nC = std::fgetc(pFile)) != EOF)
			{
				bEOL = (nC == 10 || nC == 13);
				sRet.push_back(nC);
			}
			// read the reast of the end of line or to EOF
			while (bEOL && sRet.size() < i_tSize_Bytes && (nC = std::fgetc(pFile)) != EOF)
			{
				bEOL = (nC == 10 || nC == 13);
				if (bEOL)
					sRet.push_back(nC);
				else
					std::fseek(pFile,-1,SEEK_CUR);
			}
		}
	}
	return sRet;
}

std::string cfile_base::gets_stripped(size_t i_tSize_Bytes) const
{
	std::string sRet;
	char nC;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			bool bEOL = false;
			// read to EOF or end of line
			while (!bEOL && sRet.size() < i_tSize_Bytes && (nC = std::fgetc(pFile)) != EOF)
			{
				bEOL = (nC == 10 || nC == 13);
				sRet.push_back(nC);
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
	return sRet;
}

size_t cfile_base::puts(const std::string i_sString) const
{
	size_t tRet = 0;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			tRet = std::fwrite(i_sString.c_str(),i_sString.size(),1,pFile);
		}
	}
	return tRet;
}

fpos_t cfile_base::getpos(void) const
{
	fpos_t cPos;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			std::fgetpos(pFile,&cPos);
		}
	}
	return cPos;
}

bool cfile_base::setpos(fpos_t i_cPos) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			bRet = (std::fsetpos(pFile,&i_cPos) == 0);
		}
	}
	return bRet;
}

bool cfile_base::eof(void) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			bRet = (std::feof(pFile) != 0);
		}
	}
	return bRet;
}

bool cfile_base::error(void) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			bRet = (std::ferror(pFile) != 0);
	}
	return bRet;
}
	
bool cfile_base::rewind(size_t i_nDistance) const
{
	bool bRet = false;
	if  (i_nDistance == static_cast<size_t>(-1))
	{
		std::mutex * pMutex = get_mutex_pointer();
		if (pMutex != nullptr)
		{
			std::lock_guard<std::mutex> lock(*pMutex);
			FILE * pFile = get_file_pointer();
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
		std::mutex * pMutex = get_mutex_pointer();
		if (pMutex != nullptr)
		{
			std::lock_guard<std::mutex> lock(*pMutex);
			FILE * pFile = get_file_pointer();
			if (pFile != nullptr)
				bRet = (std::fseek(pFile,nDistance,SEEK_CUR) == 0);
		}
	}
	return bRet;
}
bool cfile_base::fast_forward(size_t i_nDistance) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,static_cast<long int>(i_nDistance),SEEK_CUR) == 0);
	}
	return bRet;
}
bool cfile_base::seek(long int i_nDistance, int i_nOrigin) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,i_nDistance,i_nOrigin) == 0);
	}
	return bRet;
}
bool cfile_base::rewind_to_start(void) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,0,SEEK_SET) == 0);
	}
	return bRet;
}
bool cfile_base::forward_to_end(void) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			bRet = (std::fseek(pFile,0,SEEK_END) == 0);
	}
	return bRet;
}
void cfile_base::clear_error(void) const
{
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			std::clearerr(pFile);
	}
}

char cfile_base::getc(void) const
{
	char nRet = EOF;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			nRet = std::fgetc(pFile);
	}
	return nRet;
}

char cfile_base::putc(char i_nChar) const
{
	char nRet = EOF;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			nRet = std::fputc(i_nChar,pFile);
	}
	return nRet;
}

size_t cfile_base::tell(void) const
{
	size_t nRet = 0;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
			nRet = static_cast<size_t>(std::ftell(pFile));
	}
	return nRet;
}

bool cfile_base::is_open(void) const
{
	bool bRet = false;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		bRet = (get_file_pointer() != nullptr);
	}
	return bRet;
}
