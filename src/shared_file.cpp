#include <cfile.hpp>
#include <memory>
#include <cstdio>
#include <cfile_inst.hpp>
using namespace cfile;

//std::mutex g_mtxOpenClose;


	
shared_file *		new_shared_file(const char * i_pFilename, const char * i_pAccess)
{
	shared_file_inst * pInst = reinterpret_cast<shared_file_inst*>(malloc(sizeof(shared_file_inst)));
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
	return reinterpret_cast<shared_file *>(pInst);
}
shared_file *		new_shared_file_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type)
{
	shared_file_inst * pInst = reinterpret_cast<shared_file_inst*>(malloc(sizeof(shared_file_inst)));
	if (pInst != nullptr)
	{
		pInst->construct();
		pInst->open_enum(i_pFilename,i_eAccess_Mode,i_eData_Type);
	}
	return reinterpret_cast<shared_file *>(pInst);
}

void cfile::delete_shared_file(shared_file * i_lpvData)
{
	if (i_lpvData != nullptr)
	{
		shared_file_inst * pInst = reinterpret_cast<shared_file_inst *>(i_lpvData);
		pInst->destruct();
		free(pInst);
	}
}



bool shared_file_inst::open(const char *i_sFile, const char * i_sAccess_Type)
{
	bool bRet = false;
	if (close() && i_sFile != nullptr && i_sFile[0] != 0 && i_sAccess_Type != nullptr)
	{
		std::shared_ptr<FILE> pFileNew = std::shared_ptr<FILE>(std::fopen(i_sFile,i_sAccess_Type),&std::fclose);
		if (pFileNew != nullptr)
		{
			std::shared_ptr<std::mutex> mtx = std::make_shared<std::mutex>();
			if (mtx != nullptr)
			{
				std::lock_guard<std::mutex> lock(*mtx.get());
				m_pMutex = mtx;
				m_pFile = pFileNew;
				bRet = (m_pFile != nullptr);
			}
		}
	}
	return bRet;
}
bool shared_file_inst::close(void)
{
	if (m_pFile != nullptr)
	{
		if (m_pMutex != nullptr) // we should never get here is m_mtx is null, but just in case...
		{
			std::lock_guard<std::mutex> lock(*m_pMutex.get()); // hold mutex while file is closed
			m_pFile.reset();
		} // mutex released
		m_pMutex.reset(); // reset the mutex so that if this instance is shared it won't lock
	}
	return m_pFile == nullptr;
}
	

shared_file_inst::shared_file_inst(void)
{
	construct();
}


void shared_file_inst::construct(void)
{
	m_pFile = nullptr;
	m_pMutex = nullptr;
}
void shared_file_inst::destruct(void)
{
	close();
}



FILE * shared_file_inst::get_file_pointer(void) const
{
	return m_pFile.get();
}
std::mutex * shared_file_inst::get_mutex_pointer(void) const
{
	return m_pMutex.get();
}



#if ((defined(_MSC_VER) && _MSVC_LANG < 201703L) || (defined(__GNUC__) && __cplusplus < 201701L) || (defined(__clang__) &&  __cplusplus < 201701L))
	#define __scoped_lock_available 0
std::mutex g_mtxSF_Swap;
#else
	#define __scoped_lock_available 1
#endif


void shared_file_inst::swap(shared_file & io_File)
{
	shared_file_inst * pRHO = reinterpret_cast<shared_file_inst *>(&io_File);

	if (pRHO->get_file_pointer() != get_file_pointer()) // if the pointers are already the same, don't do anything. Solves a potential deadlock for shared files 
	{
#if __scoped_lock_available
		std::scoped_lock lock(*get_mutex_pointer(),*pRHO->get_mutex_pointer());
#else
		std::lock_guard<std::mutex> lock(g_mtxSF_Swap); // claim the static swap mutex in order to ensure that only one instance is allowed to swap at a time, preventing deadlocking the claim of the file operations mutexs
		std::lock_guard<std::mutex> lockLHO(*get_mutex_pointer());
		std::lock_guard<std::mutex> lockRHO(*pRHO->get_mutex_pointer());
#endif
		std::swap(m_pFile,pRHO->m_pFile);
	}
}	



