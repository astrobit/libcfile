#include <cstdio>
#include <mutex>
#include <memory>

namespace cfile
{
	enum class access_mode{read,write,append,read_update,write_update,append_update};
	enum class data_type{binary,text};

	class cfile_base
	{
	public:
	protected:
	#if ((defined(_MSC_VER) && _MSVC_LANG < 201703L) || (defined(__GNUC__) && __cplusplus < 201701L))
		static std::mutex m_mtxSwap;
	#endif
		virtual FILE * get_file_pointer(void) const = 0;
		virtual std::mutex * get_mutex_pointer(void) const  = 0;
	public:
		// pure virtual open and close
		virtual bool 		close(void) = 0;
		virtual bool 		open(const std::string &i_sFile, const std::string &i_sAccess_Type) = 0;
		// alternate variant of open that uses slightly more user friendly method
		virtual bool 		open(const std::string &i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type);

		virtual bool 		flush(void) const;
		virtual size_t 		read(void * o_lpBuffer, size_t i_nSize_Bytes) const;
		virtual size_t 		write(void * o_lpBuffer, size_t i_nSize_Bytes) const;
		virtual size_t 		printf(const std::string & i_sFormat, ...) const;
		virtual size_t 		scanf(const std::string & i_sFormat, ...) const;
		virtual std::string gets(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const;
		virtual std::string gets_stripped(size_t i_tSize_Bytes = static_cast<size_t>(-1)) const;
		virtual size_t 		puts(const std::string i_sString) const;
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

			

	class unique_file : public cfile_base
	{
	private:
		FILE *				m_pFile;
		mutable std::mutex	m_mtx;

		virtual FILE * get_file_pointer(void) const;
		virtual std::mutex * get_mutex_pointer(void) const;

	public:
		unique_file(void);
		unique_file(const std::string &i_sFile, const std::string &i_sAccess_Type);
		unique_file(const std::string &i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type);
		~unique_file(void);

		// dont allow copy. Use shared_file if copy is required
		unique_file(const unique_file & i_cRHO) = delete;
		unique_file & operator =(const unique_file & i_cRHO) = delete;

		bool 		close(void);
		using cfile_base::open;
		bool 		open(const std::string &i_sFile, const std::string &i_sAccess_Type);
		void		swap(unique_file & i_cRHO); 
	};
	

	class shared_file : public cfile_base
	{
	private:
		std::shared_ptr<FILE>	m_pFile;
		mutable std::shared_ptr<std::mutex>		m_mtx;

		virtual FILE * get_file_pointer(void) const;
		virtual std::mutex * get_mutex_pointer(void) const;

	public:
		shared_file(void);
		shared_file(const std::string &i_sFile, const std::string &i_sAccess_Type);
		shared_file(const std::string &i_sFile, access_mode i_eAccess_Mode, data_type i_eData_Type);
		~shared_file(void);

		bool 		close(void);
		using cfile_base::open;
		bool 		open(const std::string &i_sFile, const std::string &i_sAccess_Type);
		void		swap(shared_file & i_cRHO); 

	};
}

