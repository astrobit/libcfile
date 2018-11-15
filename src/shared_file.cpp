#include <cfile.hpp>
#include <memory>
#include <cstdio>
using namespace cfile;

//std::mutex g_mtxOpenClose;

bool shared_file::open(const std::string &i_sFile, const std::string &i_sAccess_Type)
{
	bool bRet = false;
	if (close())
	{
		std::shared_ptr<FILE> pFileNew = std::shared_ptr<FILE>(std::fopen(i_sFile.c_str(),i_sAccess_Type.c_str()),&std::fclose);
		if (pFileNew != nullptr)
		{
			std::shared_ptr<std::mutex> mtx = std::make_shared<std::mutex>();
			if (mtx != nullptr)
			{
				std::lock_guard<std::mutex> lock(*mtx.get());
				m_mtx = mtx;
				m_pFile = pFileNew;
				bRet = (m_pFile != nullptr);
			}
		}
	}
	return bRet;
}
bool shared_file::close(void)
{
	if (m_pFile != nullptr)
	{
		if (m_mtx != nullptr) // we should never get here is m_mtx is null, but just in case...
		{
			std::lock_guard<std::mutex> lock(*m_mtx.get()); // hold mutex while file is closed
			m_pFile.reset();
		} // mutex released
		m_mtx.reset(); // reset the mutex so that if this instance is shared it won't lock
	}
	return m_pFile == nullptr;
}
	

shared_file::shared_file(void)
{
	m_pFile = nullptr;
	m_mtx = nullptr;
}
shared_file::shared_file(const std::string &i_sFile, const std::string &i_sAccess_Type)
{
	{
		m_pFile = nullptr;
		m_mtx = nullptr;
	}
	open(i_sFile,i_sAccess_Type);
}
shared_file::~shared_file(void)
{
	close();
}
shared_file::shared_file(const std::string &i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type)
{
	m_pFile = nullptr;
	open(i_sFile,i_eAccess_Mode,i_eData_Type);
}


FILE * shared_file::get_file_pointer(void) const
{
	return m_pFile.get();
}
std::mutex * shared_file::get_mutex_pointer(void) const
{
	return m_mtx.get();
}



void shared_file::swap(shared_file & io_File)
{
	if (io_File.get_file_pointer() != get_file_pointer()) // if the pointers are already the same, don't do anything. Solves a potential deadlock for shared files 
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



