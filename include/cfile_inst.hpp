#pragma once
#include <cstdio>
#include <mutex>
#include <memory>

namespace cfile
{
	class cfile_base_inst : virtual public cfile_base
	{
	public:
	protected:
		virtual FILE * get_file_pointer(void) const = 0;
		virtual std::mutex * get_mutex_pointer(void) const  = 0;
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
	};



	class unique_file_inst : public unique_file_base, public cfile_base_inst
	{
	private:
		FILE *				m_pFile;
		mutable std::mutex	m_mtx;

		virtual FILE * get_file_pointer(void) const;
		virtual std::mutex * get_mutex_pointer(void) const;

	public:
		void construct(void);
		void destruct(void);

		unique_file_inst(void);
		virtual ~unique_file_inst(void);

		// dont allow copy. Use shared_file if copy is required
		unique_file_inst(const unique_file_inst & i_cRHO) = delete;
		unique_file_inst & operator =(const unique_file_inst & i_cRHO) = delete;

		virtual bool 		close(void);
		//using cfile_base::open;
		virtual bool 		open(const char * i_sFile, const char * i_sAccess_Type);
		virtual void		swap(unique_file_base & i_cRHO); 
	};


	class shared_file_inst : public shared_file_base, public cfile_base_inst
	{
	private:
		std::shared_ptr<FILE>	m_pFile;
		mutable std::shared_ptr<std::mutex>		m_pMutex;

		virtual FILE * get_file_pointer(void) const;
		virtual std::mutex * get_mutex_pointer(void) const;

	public:
		void construct(void);
		void destruct(void);

		shared_file_inst(void);
		virtual ~shared_file_inst(void);

		virtual bool 		close(void);
		//using cfile_base::open;
		virtual bool 		open(const char * i_sFile, const char *i_sAccess_Type);
		virtual void		swap(shared_file_base & i_cRHO); 

	};

	char * allocate_string(size_t i_tSize);
}
