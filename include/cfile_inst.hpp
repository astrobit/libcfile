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
		virtual bool 		__LIBCFILE_DECL open_enum(const char * i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type);

		virtual bool 		__LIBCFILE_DECL flush(void) const;
		virtual size_t 		__LIBCFILE_DECL read(void * o_lpBuffer, size_t i_nSize_Bytes) const;
		virtual size_t 		__LIBCFILE_DECL write(void * o_lpBuffer, size_t i_nSize_Bytes) const;
		virtual size_t 		__LIBCFILE_DECL printf(const char * i_sFormat, ...) const;
		virtual size_t 		__LIBCFILE_DECL scanf(const char * i_sFormat, ...) const;
		virtual const char *		__LIBCFILE_DECL gets(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const;
		virtual const char *		__LIBCFILE_DECL gets_stripped(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const;
		virtual size_t 		__LIBCFILE_DECL puts(const char * i_sString) const;
		virtual fpos_t 		__LIBCFILE_DECL getpos(void) const;
		virtual bool 		__LIBCFILE_DECL setpos(fpos_t i_cPos) const;
		virtual bool 		__LIBCFILE_DECL eof(void) const;
		virtual bool 		__LIBCFILE_DECL error(void) const;
		virtual bool 		__LIBCFILE_DECL rewind(size_t i_nDistance = static_cast<size_t>(-1)) const;
		virtual bool 		__LIBCFILE_DECL fast_forward(size_t i_nDistance) const;
		virtual bool 		__LIBCFILE_DECL seek(long int i_nDistance, int i_nOrigin) const;
		virtual bool 		__LIBCFILE_DECL rewind_to_start(void) const;
		virtual bool 		__LIBCFILE_DECL forward_to_end(void) const;
		virtual void 		__LIBCFILE_DECL clear_error(void) const;
		virtual char 		__LIBCFILE_DECL getc(void) const;
		virtual char 		__LIBCFILE_DECL putc(char i_nChar) const;
		virtual size_t 		__LIBCFILE_DECL tell(void) const;
		virtual bool 		__LIBCFILE_DECL is_open(void) const;


		virtual bool 		__LIBCFILE_DECL close(void);
		virtual bool 		__LIBCFILE_DECL open(const char * i_sFile, const char * i_sAccess_Type);
	};

	char * allocate_string(size_t i_tSize);
}
