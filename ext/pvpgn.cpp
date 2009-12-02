#define PVPGN_TWILIGHT_VERSION "0.1.0"

#ifdef __TEST__
#include <iostream>
#else
#include "ruby.h"
#endif

typedef unsigned int t_uint32;
typedef signed int t_sint32;

#define ROTL(x,n,w) (((x)<<((n)&(w-1))) | ((x)>>(((-(n))&(w-1)))))

#define ROTL32(x,n) ROTL(x,n,32)
#define ROTL16(x,n) ROTL(x,n,16)

static void PVPGN_Hash(t_uint32* pHash, t_uint32* pTemp)
{
    t_uint32 i;

    for(i = 0; i < 64; i++)
        pTemp[i + 16] = ROTL32(1,pTemp[i] ^ pTemp[i + 8] ^ pTemp[i + 2] ^ pTemp[i + 13]);

    t_uint32 a = pHash[0];
    t_uint32 b = pHash[1];
    t_uint32 c = pHash[2];
    t_uint32 d = pHash[3];
    t_uint32 e = pHash[4];
    t_uint32 g;

    for(i = 0; i < 20; i++)
    {
	   g = pTemp[i] + ROTL32(a,5) + e + ((b & c) | (~b & d)) + 0x5a827999;
	   e = d;
	   d = c;
	   c = ROTL32(b,30);
	   b = a;
	   a = g;
    }

    for(; i < 40; i++)
    {
	   g = (d ^ c ^ b) + e + ROTL32(g,5) + pTemp[i] + 0x6ed9eba1;
	   e = d;
	   d = c;
	   c = ROTL32(b,30);
	   b = a;
	   a = g;
    }

    for (; i < 60; i++)
    {
	   g = pTemp[i] + ROTL32(g,5) + e + ((c & b) | (d & c) | (d & b)) - 0x70e44324;
	   e = d;
	   d = c;
	   c = ROTL32(b,30);
	   b = a;
	   a = g;
    }

    for (; i < 80; i++)
    {
	   g = (d ^ c ^ b) + e + ROTL32(g,5) + pTemp[i] - 0x359d3e2a;
	   e = d;
	   d = c;
	   c = ROTL32(b,30);
	   b = a;
	   a = g;
    }

    pHash[0] += g;
    pHash[1] += b;
    pHash[2] += c;
    pHash[3] += d;
    pHash[4] += e;
}

static void PVPGN_Set16(t_uint32* pDest, const unsigned char* pSource, t_sint32 nSize)
{   
    if(pDest == NULL || pSource == NULL || nSize < 0)
        return;
        
    t_uint32 i, dwPos;
    for(dwPos = 0, i = 0; i < 16; i++)
    {
        pDest[i] = 0;
        if(dwPos < nSize)
	       pDest[i] |= ((t_uint32)pSource[dwPos]);
	   
        dwPos++;
        if(dwPos < nSize)
	        pDest[i] |= ((t_uint32)pSource[dwPos]) << 8;

	    dwPos++;
        if(dwPos < nSize)
	        pDest[i] |= ((t_uint32)pSource[dwPos]) << 16;

	    dwPos++;
        if(dwPos < nSize)
	        pDest[i] |= ((t_uint32)pSource[dwPos]) << 24;
	    
        dwPos++;
    }
}

static void UTILITY_ToLower(char* pString)
{
	if(pString == NULL)
		return;
		
    char nChar = 0;
    while((nChar = *pString))
        *pString++ = ((nChar < 'A' || nChar > 'Z') ? nChar : (nChar + 0x20));
}

static void UTILITY_HexToText(char* pBuffer, t_uint32 dwHex, t_uint32 bUpper = 0)
{
	if(pBuffer == NULL)
		return;
		
	static const char szHex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	static const char szHexU[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};	 
	int i;
	if(!bUpper)
	{
		for(i = 7; i > -1; i--)
		{
			int nChar = dwHex % 16;
			dwHex >>= 4; 
			pBuffer[i] = szHex[nChar];
		}
	}
	else
	{
		for(i = 7; i > -1; i--)
		{
			int nChar = dwHex % 16;
			dwHex >>= 4; 
			pBuffer[i] = szHexU[nChar];
		}		
	}
}

static char* PVPGN_CreateHash(char* pHash, t_sint32 nSize)
{
    if(pHash == NULL || nSize < 1)
        return NULL;

    t_uint32 dwLength = 0, i;        
    unsigned char* pData = (unsigned char*)pHash;
	char pHashOut[41];
    t_uint32 dwHash[5] = {0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};
    t_uint32 dwTemp[80];

    pHashOut[40] = 0;
    UTILITY_ToLower(pHash);    
    while(nSize > 0)
    {
        dwLength = (nSize > 64) ? 64 : nSize;
        PVPGN_Set16(dwTemp,pData,dwLength);
        PVPGN_Hash(dwHash,dwTemp);
        nSize -= dwLength;
        pData += dwLength;
    }
    
    for(i = 0; i < 5; i++)
    	UTILITY_HexToText(&pHashOut[i * 8],dwHash[i]);
      
   	return pHashOut;
} 
#ifdef __TEST__
int main(int argc, char *argv[])
{
    std::string sInput;
    std::cout << "Please Enter Password: ";
    std::cin >> sInput;
    std::cout << "Hash: " << PVPGN_CreateHash((char*)sInput.c_str(),sInput.size()) << std::endl;
    system("PAUSE");	
    return 0;
}
#else
/*
* call-seq:
*	PVPGN.BNHash(string_to_hash)
*
* Creates the PVPGN hash for the give string, it is automatically
* converted to a lower case string, the input is also automatically
* converted to lower case
*/
static VALUE __cdecl RUBY_BNHash(int argc, VALUE* argv, VALUE klass)
{
	VALUE vPassword;
	if(argc != 1)
		rb_raise(rb_eRuntimeError,"Invalid Amount of Arguments");
		
	vPassword = argv[0];
	char* szPass = StringValuePtr(vPassword);
 	return rb_str_new2(PVPGN_CreateHash(szPass,strlen(szPass)));
}

void Init_pvpgn()
{
	static VALUE mPVPGN = rb_define_module("PVPGN");
   	rb_define_module_function(mPVPGN,"BNHash",(VALUE (*)(...))RUBY_BNHash,-1);	
}
#endif
