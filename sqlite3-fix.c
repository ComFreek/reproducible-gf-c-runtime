/**
 * Relevant for the Windows build only.
 *
 * First, see the comment at https://github.com/GrammaticalFramework/GF/issues/22.
 * This file gets automatically #included by build-win.sh in
 *   "[GF repo as cloned in build-win.sh]/src/runtime/c/sg/sg.c"
 *
 * The contents of this file have been copied together from the SQLite 3.21.0
 * amalgamation:
 *  - https://www.sqlite.org/download.html
 *  - https://www.sqlite.org/2017/sqlite-amalgamation-3210000.zip
 *
 * The hacky approach was as follows:
 *   1. For every undefined reference error, copy the method from
 *      the amalgamation file into this file.
 *   2. For every new error of calling an unknown function, copy that
 *      method from the amalgamation file again into this file.
 *   3. Repeat.
 *
 * Some minor modifications have also been made, e.g. calls to sqlite3Atoi have been
 * replaced by calls to atoi.
 */

#include <ctype.h>

#define sqlite3Isdigit(x)   isdigit((unsigned char)(x))
#define sqlite3StrNICmp sqlite3_strnicmp
#define UpperToLower sqlite3UpperToLower
#define ArraySize(X)    ((int)(sizeof(X)/sizeof(X[0])))

const unsigned char sqlite3UpperToLower[] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
     18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
     36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
     54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99,100,101,102,103,
    104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,
    122, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,
    108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,
    252,253,254,255
};

int sqlite3StrICmp(const char *zLeft, const char *zRight){
  return strcasecmp(zLeft, zRight);
}

int sqlite3_strnicmp(const char *zLeft, const char *zRight, int N){
  register unsigned char *a, *b;
  if( zLeft==0 ){
    return zRight ? -1 : 0;
  }else if( zRight==0 ){
    return 1;
  }
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  while( N-- > 0 && *a!=0 && UpperToLower[*a]==UpperToLower[*b]){ a++; b++; }
  return N<0 ? 0 : UpperToLower[*a] - UpperToLower[*b];
}

int sqlite3Strlen30(const char* s) {
	return strlen(s);
}

u8 getSafetyLevel(const char *z, int omitFull, u8 dflt){
                             /* 123456789 123456789 123 */
  static const char zText[] = "onoffalseyestruextrafull";
  static const u8 iOffset[] = {0, 1, 2,  4,    9,  12,  15,   20};
  static const u8 iLength[] = {2, 2, 3,  5,    3,   4,   5,    4};
  static const u8 iValue[] =  {1, 0, 0,  0,    1,   1,   3,    2};
                            /* on no off false yes true extra full */
  int i, n;
  if( sqlite3Isdigit(*z) ){
    return (u8)atoi(z);
  }
  n = sqlite3Strlen30(z);
  for(i=0; i<ArraySize(iLength); i++){
    if( iLength[i]==n && sqlite3StrNICmp(&zText[iOffset[i]],z,n)==0
     && (!omitFull || iValue[i]<=1)
    ){
      return iValue[i];
    }
  }
  return dflt;
}

u8 sqlite3GetBoolean(const char *z, u8 dflt){
  return getSafetyLevel(z,1,dflt)!=0;
}

const char *sqlite3_uri_parameter(const char *zFilename, const char *zParam){
  if( zFilename==0 || zParam==0 ) return 0;
  zFilename += sqlite3Strlen30(zFilename) + 1;
  while( zFilename[0] ){
    int x = strcmp(zFilename, zParam);
    zFilename += sqlite3Strlen30(zFilename) + 1;
    if( x==0 ) return zFilename;
    zFilename += sqlite3Strlen30(zFilename) + 1;
  }
  return 0;
}

int sqlite3_uri_boolean(const char *zFilename, const char *zParam, int bDflt){
  const char *z = sqlite3_uri_parameter(zFilename, zParam);
  bDflt = bDflt!=0;
  return z ? sqlite3GetBoolean(z, bDflt) : bDflt;
}

