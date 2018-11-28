#include <cfile.hpp>
#include <memory>
#include <cstdio>
#include <cfile_inst.hpp>

using namespace cfile;

			
	
unique_file *		new_unique_file(const char * i_pFilename, const char * i_pAccess)
{
	unique_file_inst * pInst = reinterpret_cast<unique_file_inst*>(malloc(sizeof(unique_file_inst)));
	if (pInst != nullptr)
	{
		pInst->construct();
		if (i_pFilename != nullptr && i_pFilename[0] != 0)
		{
			if (i_pAccess != nullptr && i_pAccess[0] != 0)
				pInst->open(i_pFilename,i_pAccess);
			else
				pInst->open(i_pFilename,"rt");
		}
	}
	return reinterpret_cast<unique_file *>(pInst);
}
unique_file *		new_unique_file_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type)
{
	unique_file_inst * pInst = reinterpret_cast<unique_file_inst*>(malloc(sizeof(unique_file_inst)));
	if (pInst != nullptr)
	{
		pInst->construct();
		pInst->open_enum(i_pFilename,i_eAccess_Mode,i_eData_Type);
	}
	return reinterpret_cast<unique_file *>(pInst);
}

void cfile::delete_unique_file(unique_file * i_lpvData)
{
	if (i_lpvData != nullptr)
	{
		unique_file_inst * pInst = reinterpret_cast<unique_file_inst *>(i_lpvData);
		pInst->destruct();
		free(pInst);
	}
}


unique_file_inst::unique_file_inst(void)
{
	construct();
}

void unique_file_inst::construct(void)
{
	m_pFile = nullptr;
}

void unique_file_inst::destruct(void)
{
	close();
}


FILE * unique_file_inst::get_file_pointer(void) const
{
	return m_pFile;
}
std::mutex * unique_file_inst::get_mutex_pointer(void) const
{
	return &m_mtx;
}


bool unique_file_inst::close(void)
{
	bool bRet = false;
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		if (m_pFile != nullptr)
		{
			bRet = (std::fclose(m_pFile) == 0);
			if (bRet)
			{
				m_pFile = nullptr;
			}
		}
		else
			bRet = true; // return success if file already closed
	}
	return bRet;
}		
bool unique_file_inst::open(const char * i_pFile, const char * i_pAccess_Type)
{
	bool bRet = false;
	if (close() && i_pFile != nullptr && i_pFile[0] != 0 && i_pAccess_Type != nullptr && i_pAccess_Type[0] != 0)
	{
		FILE * pFileNew = std::fopen(i_pFile,i_pAccess_Type);
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_pFile = pFileNew;
			bRet = (m_pFile != nullptr);
		}
	}
	return bRet;
}

#if ((defined(_MSC_VER) && _MSVC_LANG < 201703L) || (defined(__GNUC__) && __cplusplus < 201701L) || (defined(__clang__) && __cplusplus < 201701L))
	#define __scoped_lock_available 0
std::mutex g_mtxUF_Swap;
#else
	#define __scoped_lock_available 1
#endif

void unique_file_inst::swap(unique_file & io_File)
{
	unique_file_inst * pRHO = reinterpret_cast<unique_file_inst *>(&io_File);
	if (pRHO->m_pFile != m_pFile) // if the pointers are already the same, don't do anything. Solves a potential deadlock for shared files 
	{
#if __scoped_lock_available
		std::scoped_lock lock(m_mtx,pRHO->m_mtx);
#else
		std::lock_guard<std::mutex> lock(g_mtxUF_Swap); // claim the static swap mutex in order to ensure that only one instance is allowed to swap at a time, preventing deadlocking the claim of the file operations mutexs
		std::lock_guard<std::mutex> lockLHO(m_mtx);
		std::lock_guard<std::mutex> lockRHO(pRHO->m_mtx);
#endif
		std::swap(m_pFile,pRHO->m_pFile);
	}
}	


