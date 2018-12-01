#pragma once
#include <cstdio>
#include <mutex>
#include <memory>

namespace cfile
{
	class cfile_base_inst : virtual public cfile_base
	{
	private:
		std::shared_ptr<FILE> m_pFile;
		std::shared_ptr<std::mutex> m_pMutex;
	public:
		cfile_base_inst(void);
		virtual ~cfile_base_inst(void);

	public:
		virtual bool 		open_enum(const char * i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type);

		virtual bool 		flush(void) const;
		virtual size_t 		read(void * o_lpBuffer, size_t i_nSize_Bytes) const;
		virtual size_t 		write(void * o_lpBuffer, size_t i_nSize_Bytes) const;
		virtual size_t 		printf(const char * i_sFormat, ...) const;
		virtual size_t 		scanf(const char * i_sFormat, ...) const;
		virtual const char *		gets(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const;
		virtual const char *		gets_stripped(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const;
		virtual size_t 		puts(const char * i_sString) const;
		virtual fpos_t 		getpos(void) const;
		virtual bool 		setpos(fpos_t i_cPos) const;
		virtual bool 		eof(void) const;
		virtual bool 		error(void) const;		
		virtual bool 		rewind(size_t i_nDistance = static_cast<size_t>(-1)) const;
		virtual bool 		fast_forward(size_t i_nDistance) const;
		virtual bool 		seek(long int i_nDistance, int i_nOrigin) const;
		virtual bool 		rewind_to_start(void) const;
		virtual bool 		forward_to_end(void) const;
		virtual void 		clear_error(void) const;
		virtual char 		getc(void) const;
		virtual char 		putc(char i_nChar) const;
		virtual size_t 		tell(void) const;
		virtual bool 		is_open(void) const;


		virtual bool 		close(void);
		virtual bool 		open(const char * i_sFile, const char * i_sAccess_Type);
	};

	char * allocate_string(size_t i_tSize);
}
