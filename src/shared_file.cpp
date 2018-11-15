#include <cfile.hpp>
#include <memory>
#include <cstdio>

using namespace cfile;

bool shared_file::open(const std::string &i_sFile, const std::string &i_sAccess_Type)
{
	bool bRet = false;
	if (close())
	{
		std::shared_ptr<FILE> pFileNew = std::shared_ptr<FILE>(std::fopen(i_sFile.c_str(),i_sAccess_Type.c_str()),&std::fclose);
		{
			std::lock_guard<std::mutex> lock(*m_mtx.get());
			m_pFile = pFileNew;
			bRet = (m_pFile != nullptr);
		}
	}
	return bRet;
}
bool shared_file::close(void)
{
	if (m_pFile != nullptr)
	{
		m_pFile.reset();
	}
	return m_pFile == nullptr;
}
	

shared_file::shared_file(void)
{
	m_pFile = nullptr;
	m_mtx = std::make_shared<std::mutex>();
}
shared_file::shared_file(const std::string &i_sFile, const std::string &i_sAccess_Type)
{
	m_pFile = nullptr;
	open(i_sFile,i_sAccess_Type);
	m_mtx = std::make_shared<std::mutex>();
}
shared_file::~shared_file(void)
{
	close();
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



