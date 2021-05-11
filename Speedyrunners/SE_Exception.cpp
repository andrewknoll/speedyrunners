
// exceptions_Exception_Handling_Differences3.cpp
// compile with: /EHa
#include <stdio.h>
#include <eh.h>
#include <windows.h>
#include "SE_Exception.h"

void SE_Exception::trans_func(unsigned int u, _EXCEPTION_POINTERS* pExp) {
	throw SE_Exception(u);
}