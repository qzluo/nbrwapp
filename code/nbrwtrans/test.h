#ifndef __TEST_H__
#define __TEST_H__

#ifndef DLL_NBRWTRANS_API
#define DLL_NBRWTRANS_API __declspec(dllimport)
#endif

extern "C"  DLL_NBRWTRANS_API int testadd(int a, int b); 

class DLL_NBRWTRANS_API mypoint {
public:
    void printPt();
};

#endif  //__TEST_H__