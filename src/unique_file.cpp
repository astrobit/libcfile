#include <cfile.hpp>
#include <memory>
#include <cstdio>

using namespace cfile;

unique_file::unique_file(void)
{
	m_pFile = nullptr;
}
unique_file::unique_file(const std::string &i_sFile, const std::string &i_sAccess_Type)
{
	m_pFile = nullptr;
	open(i_sFile,i_sAccess_Type);
}
unique_file::~unique_file(void)
{
	close();
}


FILE * unique_file::get_file_pointer(void) const
{
	return m_pFile;
}
std::mutex * unique_file::get_mutex_pointer(void) const
{
	return &m_mtx;
}


bool unique_file::close(void)
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
bool unique_file::open(const std::string &i_sFile, const std::string &i_sAccess_Type)
{
	bool bRet = false;
	if (close())
	{
		FILE * pFileNew = std::fopen(i_sFile.c_str(),i_sAccess_Type.c_str());
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_pFile = pFileNew;
			bRet = (m_pFile != nullptr);
		}
	}
	return bRet;
}
void unique_file::swap(unique_file & io_File)
{
	if (io_File.m_pFile != m_pFile) // if the pointers are already the same, don't do anything. Solves a potential deadlock for shared files 
	{
#if ((defined(_MSC_VER) && _MSVC_LANG < 201703L) || (defined(__GNUC__) && __cplusplus < 201701L))
		std::lock_guard<std::mutex> lock(m_mtxSwap); // claim the static swap mutex in order to ensure that only one instance is allowed to swap at a time, preventing deadlocking the claim of the file operations mutexs
		std::lock_guard<std::mutex> lockLHO(*get_mutex_pointer());
		std::lock_guard<std::mutex> lockRHO(*io_File.get_mutex_pointer());
#else
		std::scoped_lock lock(*get_mutex_pointer(),*io_File.get_mutex_pointer());
#endif
		std::swap(m_pFile,io_File.m_pFile);
	}
}	


