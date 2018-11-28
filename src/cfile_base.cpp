#include <cstdio>
#include <cfile.hpp>
#include <cstdarg>
#include <cfile_inst.hpp>
#include <cstring>

using namespace cfile;




bool cfile_base_inst::flush(void) const
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
size_t cfile_base_inst::write(void * o_lpBuffer, size_t i_nSize_Bytes) const
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
size_t cfile_base_inst::printf(const char * i_sFormat, ...) const
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
			nRet = static_cast<size_t>(std::vfprintf(pFile,i_sFormat, args));
			va_end(args);
		}
	}
	return nRet;
}

size_t cfile_base_inst::scanf(const char * i_sFormat, ...) const
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
			nRet = static_cast<size_t>(std::vfscanf(pFile,i_sFormat, args));
			va_end(args);
		}
	}
	return nRet;
}

const char * cfile_base_inst::gets(size_t i_tSize_Bytes) const
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
	char * lpRet = allocate_string(sRet.size() + 1);
	strcpy(lpRet,sRet.c_str());
	return lpRet;
}

const char * cfile_base_inst::gets_stripped(size_t i_tSize_Bytes) const
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
	char * lpRet = allocate_string(sRet.size() + 1);
	strcpy(lpRet,sRet.c_str());
	return lpRet;
}

size_t cfile_base_inst::puts(const char * i_sString) const
{
	size_t tRet = 0;
	std::mutex * pMutex = get_mutex_pointer();
	if (pMutex != nullptr)
	{
		std::lock_guard<std::mutex> lock(*pMutex);
		FILE * pFile = get_file_pointer();
		if (pFile != nullptr)
		{
			tRet = std::fwrite(i_sString,strlen(i_sString),1,pFile);
		}
	}
	return tRet;
}

fpos_t cfile_base_inst::getpos(void) const
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

bool cfile_base_inst::setpos(fpos_t i_cPos) const
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

bool cfile_base_inst::eof(void) const
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

bool cfile_base_inst::error(void) const
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
	
bool cfile_base_inst::rewind(size_t i_nDistance) const
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
bool cfile_base_inst::fast_forward(size_t i_nDistance) const
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
bool cfile_base_inst::seek(long int i_nDistance, int i_nOrigin) const
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
bool cfile_base_inst::rewind_to_start(void) const
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
bool cfile_base_inst::forward_to_end(void) const
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
void cfile_base_inst::clear_error(void) const
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

char cfile_base_inst::getc(void) const
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

char cfile_base_inst::putc(char i_nChar) const
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

size_t cfile_base_inst::tell(void) const
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

bool cfile_base_inst::is_open(void) const
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
