#define PVPGN_TWILIGHT_VERSION "0.2.5"
#define PVPGN_TWILIGHT_MODULE "PVPGN"
#define __TEST__ 0
/**************************************************/
/*/												 /*/
/*/				   Comments & Notes				 /*/
/*/												 /*/
/**************************************************/
/*
	Sat, 2 Jan 2010, 8:43am: This is all 1 file for now so that it is easy to use
							 gonna fix it later when stuff gets bigger...
	
	Mon, 1 Feb 2010, 6:12pm: converted core to ANSI C, to avoid wierd
							linux/ruby linkage bug 
*/

#if ( __TEST__ )
	#include <stdio.h>
#else
	//#include "d2bitstream.h"
	//#include "twilight.h"
	#include "ruby.h"
	#include <stdio.h>
#endif

//bit rotation macros, instead of the MSVC _rotl and _rotr instrisics(taken from pvpgn)
#define ROTL(x,n,w) (((x) << ((n) & (w - 1))) | ((x) >> (((-(n)) & (w - 1)))))
#define ROTL32(x,n) ROTL(x,n,32)
#define ROTL16(x,n) ROTL(x,n,16)

//The Ruby Module Name
#define RUBY_Module vModule
//Register a varibale arg func
#define RUBY_RegisterFunc(x) rb_define_module_function(RUBY_Module,#x,RUBY_##x,-1)
//Register a set arg func
#define RUBY_RegisterFuncEx(x,a) rb_define_module_function(RUBY_Module,#x,RUBY_##x,a)
//Register a new class, returns a VALUE
#define RUBY_RegisterClass(c) rb_define_class_under(RUBY_Module,#c,rb_cObject)
//Register an allocator for this class
#define RUBY_RegisterClassAlloc(c,x) rb_define_alloc_func(c,x)
//Register a variable arg func for a class
#define RUBY_RegisterClassFunc(c,x) rb_define_method(c,#x,x,-1)
//Register a set arg func for a class
#define RUBY_RegisterClassFuncEx(c,x,a) rb_define_method(c,#x,x,a)
//Register a global variable
#define RUBY_RegisterGlobalVariable(x,y) rb_define_global_const(#x,y)
//Register a Module Variable
#define RUBY_RegisterVariable(x,y) rb_define_const(RUBY_Module,#x,y)


/**************************************************/
/*/												 /*/
/*/					D2GS & Diablo II	 		 /*/
/*/												 /*/
/**************************************************/
/*
//removed pending C conversion :(
class cD2SaveFile
{
	public:
		enum eD2GameFlags
		{
			GFLAG_NEWBIE		=		0x001,
			GFLAG_HARDCORE		=		0x004,
			GFLAG_HASDIED		=		0x008,
			GFLAG_EXPANSION		=		0x020,
			GFLAG_NOTLADDER		=		0x040
		};

	protected:
		char szName[16];
		int nLevel;
		int nClass;
		uint_32 fFlags;
		int nDifficulty;
		int nAct;
		
	public:
	//object core
		cD2SaveFile(const char* szName);
		~cD2SaveFile();
		
	//funcs
	
	//inlines
	inline const char* GetClass() const
	{
		static const char* szClasses[7] =
		{
			"Amazon",
		    "Sorceress",
		    "Necromancer",
		    "Paladin",
		    "Barberian",
		    "Druid",
		    "Assassin"
		};
		
		return szClasses[nClass];
	}
	
	inline int GetLevel() const
	{
		return nLevel;
	}

	inline int GetDifficulty() const
	{
		return nDifficulty;
	}
	
	inline int GetAct() const
	{
		return nAct;
	}
	
	inline bool CheckFlag(uint_32 fFlag)
	{
		return (fFlags & fFlag);
	}

	//statics
};*/

	/******* TODO ********/
	// - base stat parsing
	// - item parsing
	// - skill parsing
	// - merc data
	// - ???
	// - profit!

//needs the trading stuff - item parsing basically

//needs the shared stash stuff - more item parsing plus some block management

//needs to be converted into a struct now :/

/**************************************************/
/*/												 /*/
/*/					   PVPGN					 /*/
/*/												 /*/
/**************************************************/

static void PVPGN_Hash(unsigned long* pHash, unsigned long* pTemp)
{
    unsigned long i;
    unsigned long a;
    unsigned long b;
    unsigned long c;
    unsigned long d;
    unsigned long e;
    unsigned long g;
	 
    for(i = 0; i < 64; i++)
    {
        pTemp[i + 16] = ROTL32(1,pTemp[i] ^ pTemp[i + 8] ^ pTemp[i + 2] ^ pTemp[i + 13]);
	}

    a = pHash[0];
    b = pHash[1];
    c = pHash[2];
    d = pHash[3];
    e = pHash[4];

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

static void PVPGN_Set16(unsigned long* pDest, const unsigned char* pSource, signed int nSize)
{   
    unsigned long i;
	unsigned long dwPos;
	
    if(pDest == 0 || pSource == 0 || nSize < 0)
    {
        return;
	}
        
    for(dwPos = 0, i = 0; i < 16; i++)
    {
        pDest[i] = 0;
        if(dwPos < nSize)
	       pDest[i] |= ((unsigned long)pSource[dwPos]);
	   
        dwPos++;
        if(dwPos < nSize)
	        pDest[i] |= ((unsigned long)pSource[dwPos]) << 8;

	    dwPos++;
        if(dwPos < nSize)
	        pDest[i] |= ((unsigned long)pSource[dwPos]) << 16;

	    dwPos++;
        if(dwPos < nSize)
	        pDest[i] |= ((unsigned long)pSource[dwPos]) << 24;
	    
        dwPos++;
    }
}

static void UTILITY_ToLower(char* pString)
{
    char nChar;
	if(pString == 0)
	{
		return;
	}
		
    while((nChar = *pString))
    {
        *pString++ = ((nChar < 'A' || nChar > 'Z') ? nChar : (nChar + ' '));
	}
}

static void UTILITY_HexToText(char* pBuffer, unsigned int dwHex, unsigned int bUpper)
{
	static const char szHex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	static const char szHexU[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};	 
	unsigned long i;
	
	if(pBuffer == 0)
	{
		return;
	}
		
	if(!bUpper)
	{
		for(i = 7; i > -1; i--)
		{
			int nChar;
			nChar = dwHex % 16;
			dwHex >>= 4; 
			pBuffer[i] = szHex[nChar];
		}
	}
	else
	{
		for(i = 7; i > -1; i--)
		{
			int nChar;
			nChar = dwHex % 16;
			dwHex >>= 4; 
			pBuffer[i] = szHexU[nChar];
		}		
	}
}

static char* PVPGN_CreateHash(char* pHash, signed int nSize, char* pHashOut)
{
    unsigned long dwLength; 
    unsigned char* pData = (unsigned char*)pHash;
    unsigned long dwHash[5] = {0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};
    unsigned long dwTemp[80];
    
    if(pHash == 0 || nSize < 1)
    {
        return 0;
	}

    UTILITY_ToLower(pHash);    
    while(nSize > 0)
    {
        dwLength = (nSize > 64) ? 64 : nSize;
        PVPGN_Set16(dwTemp,pData,dwLength);
        PVPGN_Hash(dwHash,dwTemp);
        nSize -= dwLength;
        pData += dwLength;
    }
    
    pHashOut[40] = 0;
   	/*
	UTILITY_HexToText(&pHashOut[0],dwHash[0],0);
    UTILITY_HexToText(&pHashOut[8],dwHash[1],0);
   	UTILITY_HexToText(&pHashOut[16],dwHash[2],0);
   	UTILITY_HexToText(&pHashOut[24],dwHash[3],0);
   	UTILITY_HexToText(&pHashOut[32],dwHash[4],0);
	*/	   	   		  	

	sprintf(pHashOut,"%x%x%x%x%x",dwHash[0],dwHash[1],dwHash[2],dwHash[3],dwHash[4]);
   	return pHashOut;
}
 
#if ( __TEST__ )
/**************************************************/
/*/												 /*/
/*/					Testing						 /*/
/*/												 /*/
/**************************************************/
int main(int argc, char *argv[])
{
    char szInput[24];
    char szHashOut[64];
    printf("Please Enter Password: ");
    scanf("Please Enter Password: %s",szInput);
    printf("Hash: %s\n",PVPGN_CreateHash(szInput,strlen(szInput),szHashOut));
    system("PAUSE");	
    return 0;
}
#else
/**************************************************/
/*/												 /*/
/*/					Ruby Interface				 /*/
/*/												 /*/
/**************************************************/

/*
* call-seq:
*	PVPGN.BNHash(string_to_hash)
*
* Creates the PVPGN hash for the give string, it is automatically
* converted to a lower case string, the input is also automatically
* converted to lower case
*/
static VALUE RUBY_BNHash(VALUE vPass)
{
	static char szHashOut[64];
	char* szPass;
	int nSize;
	
	szPass = StringValuePtr(vPass);
	if((nSize =	strlen(szPass)) < 1)
	{
		rb_raise(rb_eRuntimeError,"RUBY_BNHash(): Invalid Password Size");
	}	
	
	PVPGN_CreateHash(szPass,nSize,szHashOut);
 	return rb_str_new2(szHashOut);
}

void Init_pvpgn()
{
	VALUE RUBY_Module;
	
	RUBY_Module = rb_define_module(PVPGN_TWILIGHT_MODULE);
	RUBY_RegisterVariable(Version,rb_str_new2(PVPGN_TWILIGHT_VERSION));
	RUBY_RegisterFuncEx(BNHash,1);	
}
#endif
