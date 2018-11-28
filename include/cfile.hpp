#pragma once
#include <cstdio>

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

		void * operator new(size_t) = delete;
		void operator delete(void *) = delete;
	};


	EXPORT class unique_file : virtual public cfile_base
	{
	public:
		unique_file(void){}
		unique_file(const unique_file & i_cRHO) = delete;
		unique_file & operator =(const unique_file & i_cRHO) = delete;
		virtual void		swap(unique_file & i_cRHO) = 0; 

		void * operator new(size_t) = delete;
		void operator delete(void *) = delete;
	};

	EXPORT class shared_file : virtual public cfile_base
	{
	public:
		virtual void		swap(shared_file & i_cRHO) = 0; 

		void * operator new(size_t) = delete;
		void operator delete(void *) = delete;
	};

	EXPORT extern unique_file *		new_unique_file(const char * i_pFilename = nullptr, const char * i_pAccess = nullptr);
	EXPORT extern unique_file *		new_unique_file_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type);
	EXPORT extern void				delete_unique_file(unique_file * i_pUnique);

	EXPORT extern shared_file *		new_shared_file(const char * i_pFilename = nullptr, const char * i_pAccess = nullptr);
	EXPORT extern shared_file *		new_shared_file_enum(const char * i_pFilename, access_mode i_eAccess_Mode, data_type i_eData_Type);
	EXPORT extern void				delete_shared_file(shared_file * i_pShared);

	EXPORT extern void release_string(const char * i_lpszString);
}

