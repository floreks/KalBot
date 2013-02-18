/*
Made by BakaBug - http://www.BakaBug.net
don't share it ! or it will get fixed xD
*/
#include "stdafx.h"
#include "Crypt.h"
#include "Vari.h"

int __cdecl UpdateUNKNOW(char *var1) 
{
 int result=0; // eax@3 
 
 unsigned int v2; // [sp+4h] [bp-4h]@1 
 v2 = 4; 
  while ( v2 < 4 * *((_DWORD *)var1 + 129) ) 
  { 
	*(_DWORD *)&var1[4 * v2 + 256] = something9[var1[4 * v2 + 256]] ^ something10[(unsigned __int8)(*(_WORD *)&var1[4 * v2 + 256] >> 8)] ^ something11[(*(_DWORD *)&var1[4 * v2 + 256] >> 16) & 0xFF] ^ something12[(unsigned __int16)(*(_DWORD *)&var1[4 * v2 + 256] >> 16) >> 16]; 
	result = v2++ + 1;
  } 
  
  return result;
}

/*
int FifeTimesXOR(unsigned int s1[257],unsigned int v1,unsigned int s2[257],unsigned int v2,unsigned int s3[257],unsigned int v3,unsigned int s4[257],unsigned int v4,char* s5,unsigned int v5)
{
     return s1[(unsigned __int8)v1] ^ s2[(unsigned __int16)((_WORD)v2 >> 8)] ^ s3[((unsigned int)v3 >> 16) & 0xFF] ^ s4[(unsigned int)v4 >> 24] ^ *((_DWORD *)s5 - v5);
}
*/

#define FifeTimesXOR(s1,v1,s2,v2,s3,v3,s4,v4,s5,v5) s1[(unsigned __int8)v1] ^ s2[(unsigned __int16)((_WORD)v2 >> 8)] ^ s3[((unsigned int)v3 >> 16) & 0xFF] ^ s4[(unsigned int)v4 >> 24] ^ *((_DWORD *)s5 + v5)

__int16 __cdecl EnCryptIt(char *dest, char *source, char *UNKNOW)
{
  char *v4; // [sp+24h] [bp-4h]@1
  int v5; // [sp+14h] [bp-14h]@5
  int v6; // [sp+18h] [bp-10h]@5
  int v7; // [sp+1Ch] [bp-Ch]@5
  int v8; // [sp+20h] [bp-8h]@5
  signed int v9; // [sp+0h] [bp-28h]@5
  int v10; // [sp+4h] [bp-24h]@9
  int v11; // [sp+8h] [bp-20h]@9
  int v12; // [sp+Ch] [bp-1Ch]@9
  int v13; // [sp+10h] [bp-18h]@9
  
  v4 = UNKNOW; //here it set aeskey1
  //if ( !(*(UNKNOW + 520) & 1) )
  // return 0;
  if ( !(*(UNKNOW + 520) & 1) ) //&1 =  for encrypt , &2 = for decrypr
  {
    UpdateUNKNOW(UNKNOW);
    *(UNKNOW + 520) = (unsigned __int8)*(UNKNOW + 520) ^ 3;
  }
  v5 = *(_DWORD *)v4 ^ *(_DWORD *)dest;
  v6 = *((_DWORD *)v4 + 1) ^ *((_DWORD *)dest + 1);
  v7 = *((_DWORD *)v4 + 2) ^ *((_DWORD *)dest + 2);
  v8 = *((_DWORD *)v4 + 3) ^ *((_DWORD *)dest + 3);
  v4 += 4 * (4 * *((_DWORD *)UNKNOW + 129) - 36); //here it sets the aeskey2
  v9 = *((_DWORD *)UNKNOW + 129);
  if ( v9 != 10 )
  {
    if ( v9 != 12 )
    {
      if ( v9 != 14 )
        goto LABEL_12;
	  v10 = FifeTimesXOR(something13, v8, something14, v7, something15, v6, something16, v5, v4 ,-16);
      v11 = FifeTimesXOR(something13, v5, something14, v8, something15, v7, something16, v6, v4 ,-15);
      v12 = FifeTimesXOR(something13, v6, something14, v5, something15, v8, something16, v7, v4 ,-14);
      v13 = FifeTimesXOR(something13, v7, something14, v6, something15, v5, something16, v8, v4 ,-13);
      
	  v5  = FifeTimesXOR(something13, v13, something14, v12, something15, v11, something16, v10, v4,-12);
      v6  = FifeTimesXOR(something13, v10, something14, v13, something15, v12, something16, v11, v4,-11);
      v7  = FifeTimesXOR(something13, v11, something14, v10, something15, v13, something16, v12, v4,-10);
      v8  = FifeTimesXOR(something13, v12, something14, v11, something15, v10, something16, v13, v4,-9);
    }
	v10 = FifeTimesXOR(something13, v8, something14, v7, something15, v6, something16, v5, v4 ,-8);
	v11 = FifeTimesXOR(something13, v5, something14, v8, something15, v7, something16, v6, v4 ,-7);
    v12 = FifeTimesXOR(something13, v6, something14, v5, something15, v8, something16, v7, v4 ,-6);
    v13 = FifeTimesXOR(something13, v7, something14, v6, something15, v5, something16, v8, v4 ,-5);

	v5  = FifeTimesXOR(something13, v13, something14, v12, something15, v11, something16, v10, v4,-4);
    v6  = FifeTimesXOR(something13, v10, something14, v13, something15, v12, something16, v11, v4,-3);
    v7  = FifeTimesXOR(something13, v11, something14, v10, something15, v13, something16, v12, v4,-2);
    v8  = FifeTimesXOR(something13, v12, something14, v11, something15, v10, something16, v13, v4,-1);
  }
  int i;
  for (i=0;i<4;i++)
  {
    /* PART 1: */
	v10 = FifeTimesXOR(something13, v8, something14, v7, something15, v6, something16, v5, v4 ,i*8+0); 
	v11 = FifeTimesXOR(something13, v5, something14, v8, something15, v7, something16, v6, v4 ,i*8+1);
	v12 = FifeTimesXOR(something13, v6, something14, v5, something15, v8, something16, v7, v4 ,i*8+2);
	v13 = FifeTimesXOR(something13, v7, something14, v6, something15, v5, something16, v8, v4 ,i*8+3);

	/* PART 2: */
	v5  = FifeTimesXOR(something13, v13, something14, v12, something15, v11, something16, v10, v4,i*8+4);
	v6  = FifeTimesXOR(something13, v10, something14, v13, something15, v12, something16, v11, v4,i*8+5);
	v7  = FifeTimesXOR(something13, v11, something14, v10, something15, v13, something16, v12, v4,i*8+6);
	v8  = FifeTimesXOR(something13, v12, something14, v11, something15, v10, something16, v13, v4,i*8+7);
  }
  /* PART x: */
  v10 = FifeTimesXOR(something13, v8, something14, v7, something15, v6, something16, v5, v4 ,32);
  v11 = FifeTimesXOR(something13, v5, something14, v8, something15, v7, something16, v6, v4 ,33);
  v12 = FifeTimesXOR(something13, v6, something14, v5, something15, v8, something16, v7, v4 ,34);
  v13 = FifeTimesXOR(something13, v7, something14, v6, something15, v5, something16, v8, v4 ,35);

  /* PART last: */
  v5  = FifeTimesXOR(something17, v13, something18, v12, something19, v11, something20, v10, v4,36);
  v6  = FifeTimesXOR(something17, v10, something18, v13, something19, v12, something20, v11, v4,37);
  v7  = FifeTimesXOR(something17, v11, something18, v10, something19, v13, something20, v12, v4,38);
  v8  = FifeTimesXOR(something17, v12, something18, v11, something19, v10, something20, v13, v4,39);
LABEL_12:
  *(_DWORD *)source = v5;
  *((_DWORD *)source + 1) = v6;
  *((_DWORD *)source + 2) = v7;
  *((_DWORD *)source + 3) = v8;
  return 1;
}

__int16 __cdecl DeCryptIt(char *dest, char *source, char *UNKNOW)
{
  char *v4; // [sp+24h] [bp-4h]@1
  int v5; // [sp+14h] [bp-14h]@5
  int v6; // [sp+18h] [bp-10h]@5
  int v7; // [sp+1Ch] [bp-Ch]@5
  int v8; // [sp+20h] [bp-8h]@5
  signed int v9; // [sp+0h] [bp-28h]@5
  int v10; // [sp+4h] [bp-24h]@9
  int v11; // [sp+8h] [bp-20h]@9
  int v12; // [sp+Ch] [bp-1Ch]@9
  int v13; // [sp+10h] [bp-18h]@9

  v4 = &UNKNOW[16 * *((_DWORD *)UNKNOW + 129) + 256]; //AESKey1 - NEED TO BE DUMPED IN RUMTIME
 // if ( !(*(UNKNOW + 520) & 2) )
 //   return 0;
 if ( !(*(UNKNOW + 520) & 2) )
  {
    UpdateUNKNOW(UNKNOW);
    *(UNKNOW + 520) = (unsigned __int8)*(UNKNOW + 520) ^ 3;
  }
  v5 = *(_DWORD *)v4 ^ *(_DWORD *)dest;
  v6 = *((_DWORD *)v4 + 1) ^ *((_DWORD *)dest + 1);
  v7 = *((_DWORD *)v4 + 2) ^ *((_DWORD *)dest + 2);
  v8 = *((_DWORD *)v4 + 3) ^ *((_DWORD *)dest + 3);
  v4 = UNKNOW + 400; //16 bytes vor AES key => AESKey2 - NEED TO BE DUMPED IN RUMTIME
  v9 = *((_DWORD *)UNKNOW + 129);
  if ( v9 != 10 )
  {
    if ( v9 != 12 )
    {
      if ( v9 != 14 )
        goto LABEL_12;
	  v10 = FifeTimesXOR(something4, v6, something3, v7, something2, v8, something1, v5, v4 ,16);
	  v11 = FifeTimesXOR(something4, v7, something3, v8, something2, v5, something1, v6, v4 ,17);
	  v12 = FifeTimesXOR(something4, v8, something3, v5, something2, v6, something1, v7, v4 ,18);
	  v13 = FifeTimesXOR(something4, v5, something3, v6, something2, v7, something1, v8, v4 ,19);

      v5 = FifeTimesXOR(something4, v11, something3, v12, something2, v13, something1, v10, v4 ,12);
	  v6 = FifeTimesXOR(something4, v12, something3, v13, something2, v10, something1, v11, v4 ,13);
	  v7 = FifeTimesXOR(something4, v13, something3, v10, something2, v11, something1, v12, v4 ,14);
	  v8 = FifeTimesXOR(something4, v10, something3, v11, something2, v12, something1, v13, v4 ,15);
    }
	v10 = FifeTimesXOR(something4, v6, something3, v7, something2, v8, something1, v5, v4 ,8);
	v11 = FifeTimesXOR(something4, v7, something3, v8, something2, v5, something1, v6, v4 ,9);
	v12 = FifeTimesXOR(something4, v8, something3, v5, something2, v6, something1, v7, v4 ,10);
	v13 = FifeTimesXOR(something4, v5, something3, v6, something2, v7, something1, v8, v4 ,11);

	v5 = FifeTimesXOR(something4, v11, something3, v12, something2, v13, something1, v10, v4 ,4);
	v6 = FifeTimesXOR(something4, v12, something3, v13, something2, v10, something1, v11, v4 ,5);
	v7 = FifeTimesXOR(something4, v13, something3, v10, something2, v11, something1, v12, v4 ,6);
	v8 = FifeTimesXOR(something4, v10, something3, v11, something2, v12, something1, v13, v4 ,7);
  }
  int i;
  for (i=0;i<4;i++)
  {
	/* PART 1: */
	v10 = FifeTimesXOR(something4, v6, something3, v7, something2, v8, something1, v5, v4 ,-i*8+0); //3 => -21
	v11 = FifeTimesXOR(something4, v7, something3, v8, something2, v5, something1, v6, v4 ,-i*8+1);
	v12 = FifeTimesXOR(something4, v8, something3, v5, something2, v6, something1, v7, v4 ,-i*8+2);
	v13 = FifeTimesXOR(something4, v5, something3, v6, something2, v7, something1, v8, v4 ,-i*8+3); //3 => -18
	/* PART 2: */
	v5 = FifeTimesXOR(something4, v11, something3, v12, something2, v13, something1, v10, v4 ,-i*8-4); //3 => -28
	v6 = FifeTimesXOR(something4, v12, something3, v13, something2, v10, something1, v11, v4 ,-i*8-3);
	v7 = FifeTimesXOR(something4, v13, something3, v10, something2, v11, something1, v12, v4 ,-i*8-2);
	v8 = FifeTimesXOR(something4, v10, something3, v11, something2, v12, something1, v13, v4 ,-i*8-1); //3 => -22
	//printf("Part1: end %d\n",-i*8-1);
  }
  /* PART x: */
  v10 = FifeTimesXOR(something4, v6, something3, v7, something2, v8, something1, v5, v4 ,-32); 
  v11 = FifeTimesXOR(something4, v7, something3, v8, something2, v5, something1, v6, v4 ,-31);
  v12 = FifeTimesXOR(something4, v8, something3, v5, something2, v6, something1, v7, v4 ,-30); 
  v13 = FifeTimesXOR(something4, v5, something3, v6, something2, v7, something1, v8, v4 ,-29);

  /* PART last: */
  v5 = FifeTimesXOR(something8, v11, something7, v12, something6, v13, something5, v10, v4 ,-36);
  v6 = FifeTimesXOR(something8, v12, something7, v13, something6, v10, something5, v11, v4 ,-35);
  v7 = FifeTimesXOR(something8, v13, something7, v10, something6, v11, something5, v12, v4 ,-34);
  v8 = FifeTimesXOR(something8, v10, something7, v11, something6, v12, something5, v13, v4 ,-33);
LABEL_12:
  *(_DWORD *)source = v5;
  *((_DWORD *)source + 1) = v6;
  *((_DWORD *)source + 2) = v7;
  *((_DWORD *)source + 3) = v8;
  return 1;
}

int __cdecl AESDecrypt(char *UNKNOW, char *dest, char *source, int size, char *XORKey)
{
  int result; // eax@1
  int position; // [sp+0h] [bp-28h]@1
  char TEMP_BUFFER[16]; // [sp+18h] [bp-10h]@3
  char TEMP[16]; // [sp+8h] [bp-20h]@3
  int i; // [sp+4h] [bp-24h]@5

  result = size;
  position = size - 16;
  while ( position >= 0 )                                       // 16 bytes block
  {
    memcpy(TEMP_BUFFER, source, 16);
	         //DEST      //SOURCE eigendlcih umgekehrt
    DeCryptIt(TEMP_BUFFER, TEMP, UNKNOW); //komig
    memcpy(dest, TEMP, 16);
    position -= 16;
   // result = (int)(source + 16);
    source += 16;
    dest += 16;
  }
  if ( position != -16 )
  {
    memcpy(TEMP_BUFFER, source, position + 16);
    i = 0;
    while ( i < position + 16 )
    {
      TEMP_BUFFER[i] ^= XORKey[i];                              //do XOR crypt for last 16 Bytes
      ++i;
    }
    memcpy(dest, TEMP_BUFFER, position + 16);
  }
  return result;
}

int __cdecl AESEncrypt(char *UNKNOW, char *dest, char *source, int size, char *XORKey)
{
  int result; // eax@1
  int position; // [sp+0h] [bp-28h]@1
  char TEMP_BUFFER[16]; // [sp+18h] [bp-10h]@3
  char TEMP[16]; // [sp+8h] [bp-20h]@3
  int i; // [sp+4h] [bp-24h]@5

  result = size;
  position = size - 16;
  while ( position >= 0 )                                       // 16 bytes block
  {
    memcpy(TEMP_BUFFER, source, 16);
	         //DEST      //SOURCE eigendlcih umgekehrt
    EnCryptIt(TEMP_BUFFER, TEMP, UNKNOW); //komig
    memcpy(dest, TEMP, 16);
    position -= 16;
   // result = (int)(source + 16);
    source += 16;
    dest += 16;
  }
  if ( position != -16 )
  {
    memcpy(TEMP_BUFFER, source, position + 16);
    i = 0;
    while ( i < position + 16 )
    {
      TEMP_BUFFER[i] ^= XORKey[i];                              //do XOR crypt for last 16 Bytes
      ++i;
    }
    memcpy(dest, TEMP_BUFFER, position + 16);
  }
  return result;
}

signed int __stdcall DecryptAES(char* source, int size)
{
	char* dest=source;
	return AESDecrypt((char*)&UNKNOW, dest, source, size, (char*)&XORKey);
}

signed int __stdcall EncryptAES(char* source, int size)
{
	char* dest=source;
	return AESEncrypt((char*)&UNKNOW, dest, source, size, (char*)&XORKey);
}


int DecryptPacket(char* buf)
{
	return DecryptAES(buf+3,int(*(PWORD(buf))-3));
}

int EncryptPacket(char* buf)
{
	return EncryptAES(buf+3,int(*(PWORD(buf))-3));
}

long DecryptTable(unsigned long index, unsigned char *buf, unsigned long len)
{
	if(!buf || !len)
		return -1;

	index &= 0x3F;
	index <<= 8;
	while (len--)
		buf[len] = DecryptTableData[index + buf[len]];

	return 0;
}

long EncryptTable(unsigned long index, unsigned char *buf, unsigned long len)
{
	if(!buf || !len)
		return -1;

	index &= 0x3F;
	index <<= 8;

	while (len--)
		buf[len] = EncryptTableData[index + buf[len]];

	return 0;
}