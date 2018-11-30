#pragma once
#include <cstdio>
#include <memory>
#include <string>
//
#if defined(_WIN32) || defined(_WIN64)
	#define EXPORT	__declspec(dllexport)
#else
	#define EXPORT
#endif

namespace cfile
{
	enum class access_mode{read,write,append,read_update,write_update,append_update};
	enum class data_type{binary,text};

	EXPORT class cfile_base
	{
	public:
		// pure virtual open and close
		virtual bool 		close(void) = 0;
		virtual bool 		open(const char * i_sFile, const char * i_sAccess_Type) = 0;
		// alternate variant of open that uses slightly more user friendly method
		virtual bool 		open_enum(const char * i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type) = 0;

		virtual bool 		flush(void) const = 0;
		virtual size_t 		read(void * o_lpBuffer, size_t i_nSize_Bytes) const = 0;
		virtual size_t 		write(void * o_lpBuffer, size_t i_nSize_Bytes) const = 0;
		virtual size_t 		printf(const char * i_sFormat, ...) const = 0;
		virtual size_t 		scanf(const char * i_sFormat, ...) const = 0;
		virtual const char *		gets(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const = 0;
		virtual const char *		gets_stripped(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const = 0;
		virtual size_t 		puts(const char * i_sString) const = 0;
		virtual fpos_t 		getpos(void) const = 0;
		virtual bool 		setpos(fpos_t i_cPos) const = 0;
		virtual bool 		eof(void) const = 0;
		virtual bool 		error(void) const = 0;		
		virtual bool 		rewind(size_t i_nDistance = static_cast<size_t>(-1)) const = 0;
		virtual bool 		fast_forward(size_t i_nDistance) const = 0;
		virtual bool 		seek(long int i_nDistance, int i_nOrigin) const = 0;
		virtual bool 		rewind_to_start(void) const = 0;
		virtual bool 		forward_to_end(void) const = 0;
		virtual void 		clear_error(void) const = 0;
		virtual char 		getc(void) const = 0;
		virtual char 		putc(char i_nChar) const = 0;
		virtual size_t 		tell(void) const = 0;
		virtual bool 		is_open(void) const = 0;

//		void * operator new(size_t) = delete;
//		void operator delete(void *) = delete;
	};


	EXPORT class unique_file_base : virtual public cfile_base
	{
	public:
		unique_file_base(void){}
		unique_file_base(const unique_file_base & i_cRHO) = delete;
		unique_file_base & operator =(const unique_file_base & i_cRHO) = delete;
		virtual void		swap(unique_file_base & i_cRHO) = 0; 
		virtual bool 		close(void) = 0;
		virtual bool 		open(const char * i_sFile, const char * i_sAccess_Type) = 0;

//		void * operator new(size_t) = delete;
//		void operator delete(void *) = delete;
	};

	EXPORT class shared_file_base : virtual public cfile_base
	{
	public:
		virtual void		swap(shared_file_base & i_cRHO) = 0; 
		virtual bool 		close(void) = 0;
		virtual bool 		open(const char * i_sFile, const char * i_sAccess_Type) = 0;

//		void * operator new(size_t) = delete;
//		void operator delete(void *) = delete;
	};

	EXPORT extern unique_file_base *		new_unique_file(const char * i_pFilename = nullptr, const char * i_pAccess = nullptr);
	EXPORT extern unique_file_base *		new_unique_file_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type);
	EXPORT extern void						delete_unique_file(unique_file_base * i_pUnique);
	EXPORT extern void						delete_unique_file_deleter(void * i_pUnique);

	EXPORT extern unique_file_base *		new_unique_file_array(size_t i_nNum_Files);
	EXPORT extern void						delete_unique_file_array(unique_file_base * i_pUnique);

	EXPORT extern shared_file_base *		new_shared_file(const char * i_pFilename = nullptr, const char * i_pAccess = nullptr);
	EXPORT extern shared_file_base *		new_shared_file_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type);
	EXPORT extern void						delete_shared_file(shared_file_base * i_pShared);
	EXPORT extern void						delete_shared_file_deleter(void * i_pShared);

	EXPORT extern shared_file_base *		new_shared_file_array(size_t i_nNum_Files);
	EXPORT extern void						delete_shared_file_array(shared_file_base * i_pShared);

	EXPORT extern void release_string(const char * i_lpszString);

	class shared_file
	{
	private:
		std::shared_ptr<shared_file_base> m_lpFile;
	public:
		shared_file(void) : m_lpFile(new_shared_file(),delete_shared_file_deleter) {}
		
		bool 		close(void)
		{
			bool bRet = true;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->close();
			}
			return bRet;
		}
		bool open(const char * i_sFile, const char * i_sAccess_Type)
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->open(i_sFile,i_sAccess_Type);
			}
			return bRet;
		}
		// alternate variant of open that uses slightly more user friendly method
		bool open_enum(const char * i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type)
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->open_enum(i_sFile,i_eAccess_Mode,i_eData_Type);
			}
			return bRet;
		}

		bool 		flush(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->flush();
			}
			return bRet;
		}
		size_t 		read(void * o_lpBuffer, size_t i_nSize_Bytes) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->read(o_lpBuffer,i_nSize_Bytes);
			}
			return nNum;
		}
		size_t 		write(void * o_lpBuffer, size_t i_nSize_Bytes) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->write(o_lpBuffer,i_nSize_Bytes);
			}
			return nNum;
		}
		//size_t 		printf(const char * i_sFormat, ...) const; //@@TODO
		//size_t 		scanf(const char * i_sFormat, ...) const; //@@TODO
		std::string		gets(size_t i_nSize_Bytes = static_cast<size_t>(-1)) const
		{
			std::string sRet;
			if (m_lpFile != nullptr)
			{
				const char * lpData = m_lpFile.get()->gets(i_nSize_Bytes);
				sRet = lpData;
				release_string(lpData);
			}
			return sRet;
		}
		std::string		gets_stripped(size_t i_nSize_Bytes = static_cast<size_t>(-1)) const
		{
			std::string sRet;
			if (m_lpFile != nullptr)
			{
				const char * lpData = m_lpFile.get()->gets_stripped(i_nSize_Bytes);
				sRet = lpData;
				release_string(lpData);
			}
			return sRet;
		}
		size_t 		puts(const char * i_sString) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->puts(i_sString);
			}
			return nNum;
		}
		fpos_t 		getpos(void) const
		{
			fpos_t cRet;
			if (m_lpFile != nullptr)
			{
				cRet = m_lpFile.get()->getpos();
			}
			return cRet;
		}

		bool setpos(fpos_t i_cPos) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->setpos(i_cPos);
			}
			return bRet;
		}
		bool 		eof(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->eof();
			}
			return bRet;
		}
		bool 		error(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->error();
			}
			return bRet;
		}
		bool 		rewind(size_t i_nDistance = static_cast<size_t>(-1)) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->rewind(i_nDistance);
			}
			return bRet;
		}
		bool 		fast_forward(size_t i_nDistance) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->fast_forward(i_nDistance);
			}
			return bRet;
		}
		bool 		seek(long int i_nDistance, int i_nOrigin) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->seek(i_nDistance,i_nOrigin);
			}
			return bRet;
		}
		bool 		rewind_to_start(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->rewind_to_start();
			}
			return bRet;
		}
		bool 		forward_to_end(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->forward_to_end();
			}
			return bRet;
		}
		void 		clear_error(void) const
		{
			if (m_lpFile != nullptr)
			{
				m_lpFile.get()->clear_error();
			}
		}
		char 		getc(void) const
		{
			char chRet = 0;
			if (m_lpFile != nullptr)
			{
				chRet = m_lpFile.get()->getc();
			}
			return chRet;
		}
		char 		putc(char i_nChar) const
		{
			char chRet = 0;
			if (m_lpFile != nullptr)
			{
				chRet = m_lpFile.get()->putc(i_nChar);
			}
			return chRet;
		}
		size_t 		tell(void) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->tell();
			}
			return nNum;
		}
		bool 		is_open(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->is_open();
			}
			return bRet;
		}

	};


	class unique_file
	{
	private:
		std::unique_ptr<unique_file_base,decltype(&delete_unique_file_deleter)> m_lpFile;
	public:
		unique_file(void) : m_lpFile(new_unique_file(),&delete_unique_file_deleter) {}
		unique_file(const unique_file &i_cRHO) = delete;
		unique_file & operator =(const unique_file & i_cRHO) = delete;
		
		bool 		close(void)
		{
			bool bRet = true;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->close();
			}
			return bRet;
		}
		bool open(const char * i_sFile, const char * i_sAccess_Type)
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->open(i_sFile,i_sAccess_Type);
			}
			return bRet;
		}
		// alternate variant of open that uses slightly more user friendly method
		bool open_enum(const char * i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type)
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->open_enum(i_sFile,i_eAccess_Mode,i_eData_Type);
			}
			return bRet;
		}

		bool 		flush(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->flush();
			}
			return bRet;
		}
		size_t 		read(void * o_lpBuffer, size_t i_nSize_Bytes) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->read(o_lpBuffer,i_nSize_Bytes);
			}
			return nNum;
		}
		size_t 		write(void * o_lpBuffer, size_t i_nSize_Bytes) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->write(o_lpBuffer,i_nSize_Bytes);
			}
			return nNum;
		}
		//size_t 		printf(const char * i_sFormat, ...) const; //@@TODO
		//size_t 		scanf(const char * i_sFormat, ...) const; //@@TODO
		std::string		gets(size_t i_nSize_Bytes = static_cast<size_t>(-1)) const
		{
			std::string sRet;
			if (m_lpFile != nullptr)
			{
				const char * lpData = m_lpFile.get()->gets(i_nSize_Bytes);
				sRet = lpData;
				release_string(lpData);
			}
			return sRet;
		}
		std::string		gets_stripped(size_t i_nSize_Bytes = static_cast<size_t>(-1)) const
		{
			std::string sRet;
			if (m_lpFile != nullptr)
			{
				const char * lpData = m_lpFile.get()->gets_stripped(i_nSize_Bytes);
				sRet = lpData;
				release_string(lpData);
			}
			return sRet;
		}
		size_t 		puts(const char * i_sString) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->puts(i_sString);
			}
			return nNum;
		}
		fpos_t 		getpos(void) const
		{
			fpos_t cRet;
			if (m_lpFile != nullptr)
			{
				cRet = m_lpFile.get()->getpos();
			}
			return cRet;
		}

		bool setpos(fpos_t i_cPos) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->setpos(i_cPos);
			}
			return bRet;
		}
		bool 		eof(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->eof();
			}
			return bRet;
		}
		bool 		error(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->error();
			}
			return bRet;
		}
		bool 		rewind(size_t i_nDistance = static_cast<size_t>(-1)) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->rewind(i_nDistance);
			}
			return bRet;
		}
		bool 		fast_forward(size_t i_nDistance) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->fast_forward(i_nDistance);
			}
			return bRet;
		}
		bool 		seek(long int i_nDistance, int i_nOrigin) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->seek(i_nDistance,i_nOrigin);
			}
			return bRet;
		}
		bool 		rewind_to_start(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->rewind_to_start();
			}
			return bRet;
		}
		bool 		forward_to_end(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->forward_to_end();
			}
			return bRet;
		}
		void 		clear_error(void) const
		{
			if (m_lpFile != nullptr)
			{
				m_lpFile.get()->clear_error();
			}
		}
		char 		getc(void) const
		{
			char chRet = 0;
			if (m_lpFile != nullptr)
			{
				chRet = m_lpFile.get()->getc();
			}
			return chRet;
		}
		char 		putc(char i_nChar) const
		{
			char chRet = 0;
			if (m_lpFile != nullptr)
			{
				chRet = m_lpFile.get()->putc(i_nChar);
			}
			return chRet;
		}
		size_t 		tell(void) const
		{
			size_t nNum = 0;
			if (m_lpFile != nullptr)
			{
				nNum = m_lpFile.get()->tell();
			}
			return nNum;
		}
		bool 		is_open(void) const
		{
			bool bRet = false;
			if (m_lpFile != nullptr)
			{
				bRet = m_lpFile.get()->is_open();
			}
			return bRet;
		}

	};


}

