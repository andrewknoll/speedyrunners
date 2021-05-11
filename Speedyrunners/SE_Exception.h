#pragma once
#include <eh.h>
class SE_Exception {
private:
	SE_Exception() {}
	unsigned int nSE;
public:
	SE_Exception(SE_Exception& e) : nSE(e.nSE) {}
	SE_Exception(unsigned int n) : nSE(n) {}
	~SE_Exception() {}
	unsigned int getSeNumber() { return nSE; }
};

void SE_trans_func(unsigned int u, _EXCEPTION_POINTERS* pExp) {
	SE_Exception see(u);
	throw see;
}

