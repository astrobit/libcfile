#include <cfile.hpp>
#include <cfile_inst.hpp>
#include <vector>
#include <map>

std::vector<std::unique_ptr<char[]>> g_vAllocated_Pointers;
std::map<const char *, size_t> g_mapPointers;

std::mutex g_mString_Add;

char * cfile::allocate_string(size_t i_tSize)
{
	std::lock_guard<std::mutex> lock(g_mString_Add);
	size_t tIdx = g_vAllocated_Pointers.size();
	g_vAllocated_Pointers.push_back(std::make_unique<char[]>(i_tSize));
	char * lpInst = g_vAllocated_Pointers.back().get();
	if (lpInst != nullptr)
	{
		g_mapPointers[lpInst] = tIdx;
	}
	return lpInst;
}


void __stdcall cfile::release_string(const char * i_lpszString)
{
	std::lock_guard<std::mutex> lock(g_mString_Add);
	// make sure that the pointer is not already null and that we are the owner
	if (i_lpszString != nullptr && g_mapPointers.count(i_lpszString) != 0)
	{
		size_t tIdx = g_mapPointers.at(i_lpszString);
		g_vAllocated_Pointers[tIdx].release();
	}
}

