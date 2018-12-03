#pragma once
#include <cstdio>
#include <memory>
#include <string>
//
#if defined(_WIN32) || defined(_WIN64)
#if defined(LIBCFILE_EXPORT) || defined (_WINDLL) || defined(BUILD_DLL)
	#define __LIBCFILE_DLL	__declspec(dllexport)
#elif defined (LIBCFILE_IMPORT)
#define __LIBCFILE_DLL	__declspec(dllimport)
#else
#define __LIBCFILE_DLL
#endif
	#define __LIBCFILE_DECL __cdecl
#else
	#define __LIBCFILE_DLL
	#define __LIBCFILE_DECL
#endif

namespace cfile
{
	enum class access_mode{read,write,append,read_update,write_update,append_update};
	enum class data_type{binary,text};

	class __LIBCFILE_DLL cfile_base
	{
	public:
	    virtual ~cfile_base(void) {}
		// pure virtual open and close
		virtual bool 		__LIBCFILE_DECL close(void) = 0;
		virtual bool 		__LIBCFILE_DECL open(const char * i_sFile, const char * i_sAccess_Type) = 0;
		// alternate variant of open that uses slightly more user friendly method
		virtual bool 		__LIBCFILE_DECL open_enum(const char * i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type) = 0;

		virtual bool 		__LIBCFILE_DECL flush(void) const = 0;
		virtual size_t 		__LIBCFILE_DECL read(void * o_pBuffer, size_t i_nSize_Bytes) const = 0;
		virtual size_t 		__LIBCFILE_DECL write(void * o_pBuffer, size_t i_nSize_Bytes) const = 0;
		virtual size_t 		__LIBCFILE_DECL printf(const char * i_sFormat, ...) const = 0;
		virtual size_t 		__LIBCFILE_DECL scanf(const char * i_sFormat, ...) const = 0;
		virtual const char *		__LIBCFILE_DECL gets(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const = 0;
		virtual const char *		__LIBCFILE_DECL gets_stripped(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const = 0;
		virtual size_t 		__LIBCFILE_DECL puts(const char * i_sString) const = 0;
		virtual fpos_t 		__LIBCFILE_DECL getpos(void) const = 0;
		virtual bool 		__LIBCFILE_DECL setpos(fpos_t i_cPos) const = 0;
		virtual bool 		__LIBCFILE_DECL eof(void) const = 0;
		virtual bool 		__LIBCFILE_DECL error(void) const = 0;
		virtual bool 		__LIBCFILE_DECL rewind(size_t i_nDistance = static_cast<size_t>(-1)) const = 0;
		virtual bool 		__LIBCFILE_DECL fast_forward(size_t i_nDistance) const = 0;
		virtual bool 		__LIBCFILE_DECL seek(long int i_nDistance, int i_nOrigin) const = 0;
		virtual bool 		__LIBCFILE_DECL rewind_to_start(void) const = 0;
		virtual bool 		__LIBCFILE_DECL forward_to_end(void) const = 0;
		virtual void 		__LIBCFILE_DECL clear_error(void) const = 0;
		virtual char 		__LIBCFILE_DECL getc(void) const = 0;
		virtual char 		__LIBCFILE_DECL putc(char i_nChar) const = 0;
		virtual size_t 		__LIBCFILE_DECL tell(void) const = 0;
		virtual bool 		__LIBCFILE_DECL is_open(void) const = 0;
//	protected:
//		virtual void		swap(cfile_base & i_cRHO) = 0;
//		void * operator new(size_t) = delete;
//		void operator delete(void *) = delete;
	};

	extern "C"
	{
		__LIBCFILE_DLL cfile_base *	__LIBCFILE_DECL	new_cfile(const char * i_pFilename = nullptr, const char * i_pAccess = nullptr);
		__LIBCFILE_DLL cfile_base *	__LIBCFILE_DECL	new_cfile_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type);
		__LIBCFILE_DLL void			__LIBCFILE_DECL	delete_cfile(cfile_base * i_pFile);
		__LIBCFILE_DLL void			__LIBCFILE_DECL	release_string(const char * i_psString);
	}

	class cfile_base_deleter
	{
	public:
		void operator()(cfile_base * i_pFile)const
		{
			delete_cfile(i_pFile);
		}
	};




	class cfile_inst_base
	{
	protected:
		virtual cfile_base * get_file_pointer(void) const = 0;
	public:
        virtual ~cfile_inst_base(void){}
		bool 		close(void)
		{
			bool bRet = true;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->close();
			}
			return bRet;
		}
		bool open(const std::string & i_sFile, const std::string & i_sAccess_Type)
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->open(i_sFile.c_str(),i_sAccess_Type.c_str());
			}
			return bRet;
		}
		// alternate variant of open that uses slightly more user friendly method
		bool open_enum(const std::string & i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type)
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->open_enum(i_sFile.c_str(),i_eAccess_Mode,i_eData_Type);
			}
			return bRet;
		}

		bool 		flush(void) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->flush();
			}
			return bRet;
		}
		size_t 		read(void * o_lpBuffer, size_t i_nSize_Bytes) const
		{
			size_t nNum = 0;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				nNum = pFile->read(o_lpBuffer,i_nSize_Bytes);
			}
			return nNum;
		}
		size_t 		write(void * o_lpBuffer, size_t i_nSize_Bytes) const
		{
			size_t nNum = 0;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				nNum = pFile->write(o_lpBuffer,i_nSize_Bytes);
			}
			return nNum;
		}
		//size_t 		printf(const char * i_sFormat, ...) const; //@@TODO
		//size_t 		scanf(const char * i_sFormat, ...) const; //@@TODO
		std::string		gets(size_t i_nSize_Bytes = static_cast<size_t>(-1)) const
		{
			std::string sRet;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				const char * lpData = pFile->gets(i_nSize_Bytes);
				sRet = lpData;
				release_string(lpData);
			}
			return sRet;
		}
		std::string		gets_stripped(size_t i_nSize_Bytes = static_cast<size_t>(-1)) const
		{
			std::string sRet;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				const char * lpData = pFile->gets_stripped(i_nSize_Bytes);
				sRet = lpData;
				release_string(lpData);
			}
			return sRet;
		}
		size_t 		puts(const std::string & i_sString) const
		{
			size_t nNum = 0;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				nNum = pFile->puts(i_sString.c_str());
			}
			return nNum;
		}
		fpos_t 		getpos(void) const
		{
			fpos_t cRet;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				cRet = pFile->getpos();
			}
			return cRet;
		}

		bool setpos(fpos_t i_cPos) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->setpos(i_cPos);
			}
			return bRet;
		}
		bool 		eof(void) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->eof();
			}
			return bRet;
		}
		bool 		error(void) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->error();
			}
			return bRet;
		}
		bool 		rewind(size_t i_nDistance = static_cast<size_t>(-1)) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->rewind(i_nDistance);
			}
			return bRet;
		}
		bool 		fast_forward(size_t i_nDistance) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->fast_forward(i_nDistance);
			}
			return bRet;
		}
		bool 		seek(long int i_nDistance, int i_nOrigin) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->seek(i_nDistance,i_nOrigin);
			}
			return bRet;
		}
		bool 		rewind_to_start(void) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->rewind_to_start();
			}
			return bRet;
		}
		bool 		forward_to_end(void) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->forward_to_end();
			}
			return bRet;
		}
		void 		clear_error(void) const
		{
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				pFile->clear_error();
			}
		}
		char 		getc(void) const
		{
			char chRet = 0;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				chRet = pFile->getc();
			}
			return chRet;
		}
		char 		putc(char i_nChar) const
		{
			char chRet = 0;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				chRet = pFile->putc(i_nChar);
			}
			return chRet;
		}
		size_t 		tell(void) const
		{
			size_t nNum = 0;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				nNum = pFile->tell();
			}
			return nNum;
		}
		bool 		is_open(void) const
		{
			bool bRet = false;
			cfile_base * pFile = get_file_pointer();
			if (pFile != nullptr)
			{
				bRet = pFile->is_open();
			}
			return bRet;
		}

	};

	class shared_file : public cfile_inst_base
	{
	private:
		std::shared_ptr<cfile_base> m_pFile;
	protected:
		virtual cfile_base * get_file_pointer(void) const
		{
			return m_pFile.get();
		}
	public:
		shared_file(void) : m_pFile(new_cfile(), cfile_base_deleter()) {}

	};


	class unique_file : public cfile_inst_base
	{
	private:
		std::unique_ptr<cfile_base,decltype(&delete_cfile)> m_pFile;
	protected:
		virtual cfile_base * get_file_pointer(void) const
		{
			return m_pFile.get();
		}
	public:
		unique_file(void) : m_pFile(new_cfile(),&delete_cfile) {}
		unique_file(const unique_file &i_cRHO) = delete;
		unique_file & operator =(const unique_file & i_cRHO) = delete;

	};



}

