/**
*  
* Solution to course project #02
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Nikola Groudov
* @idnumber 8MI0600270
* @compiler GCC
*
* 
*   Functions
*   Source file
*/

#include "tables.h"
#include <iostream>

void KeyExpansionCore(unsigned char* in, unsigned char i)
{
    unsigned int* q = (unsigned int*)in;
    *q = (*q >> 8) | ((*q & 0xff) <<24);

    for(int i = 0; i<4;i++)
        in[i]= s_box[in[i]];

    in[0] ^= rcon[i];
}

void KeyExpansion(unsigned char* inputKey, unsigned char* expandedKeys)
{
    for(int i = 0; i < 16; i++)
        expandedKeys[i] = inputKey[i];
    
    int bytesGenerated = 16;
    int rconIteration = 1;
    unsigned char tmp[4];

    while(bytesGenerated < 176)
    {
        for(int i = 0; i< 4; i++)
            tmp[i]= expandedKeys[i + bytesGenerated - 4];

        if(bytesGenerated %16 == 0)
            KeyExpansionCore(tmp, rconIteration++);
        for(int a = 0; a < 4; a++)
        {
            expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated -16] ^ tmp[a];
            bytesGenerated++;
        }
    }

}

void SubBytes(unsigned char* state) 
{
    for(int i = 0; i<16; i++)
        state[i]=s_box[state[i]];
}

void InvSubBytes(unsigned char* state)
{
    for(int i = 0; i < 16;i++)
        state[i]=revs_box[state[i]];
}

void ShiftRows(unsigned char* state)
{
    unsigned char tmp[16];

    tmp[0] = state[0];
    tmp[1] = state[5];
    tmp[2] = state[10];
    tmp[3] = state[15];

    tmp[4] = state[4];
    tmp[5] = state[9];
    tmp[6] = state[14];
    tmp[7] = state[3];

    tmp[8] = state[8];
    tmp[9] = state[13];
    tmp[10] = state[2];
    tmp[11] = state[7];

    tmp[12] = state[12];
    tmp[13] = state[1];
    tmp[14] = state[6];
    tmp[15] = state[11];

    for(int i = 0; i < 16;i++)
        state[i]= tmp[i];

}

void InvShiftRows(unsigned char* state)
{
    unsigned char tmp[16];

    tmp[0] = state[0];
    tmp[5] = state[1];
    tmp[10] = state[2];
    tmp[15] = state[3];

    tmp[4] = state[4];
    tmp[9] = state[5];
    tmp[14] = state[6];
    tmp[3] = state[7];

    tmp[8] = state[8];
    tmp[13] = state[9];
    tmp[2] = state[10];
    tmp[7] = state[11];

    tmp[12] = state[12];
    tmp[1] = state[13];
    tmp[6] = state[14];
    tmp[11] = state[15];

    for(int i = 0; i < 16;i++)
        state[i]= tmp[i];

}

void MixColumns(unsigned char* state) 
{
    unsigned char tmp[16];
    tmp[0] = (unsigned char)(mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3]);
    tmp[1] = (unsigned char)(state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3]);
    tmp[2] = (unsigned char)(state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]]);
    tmp[3] = (unsigned char)(mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]]);

    tmp[4] = (unsigned char)(mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7]);
    tmp[5] = (unsigned char)(state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7]);
    tmp[6] = (unsigned char)(state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]]);
    tmp[7] = (unsigned char)(mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]]);

    tmp[8] = (unsigned char)(mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11]);
    tmp[9] = (unsigned char)(state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11]);
    tmp[10] = (unsigned char)(state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]]);
    tmp[11] = (unsigned char)(mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]]);

    tmp[12] = (unsigned char)(mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15]);
    tmp[13] = (unsigned char)(state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15]);
    tmp[14] = (unsigned char)(state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]]);
    tmp[15] = (unsigned char)(mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]]);

    for(int i = 0 ; i < 16; i++)
        state[i]= tmp[i];
}

unsigned char gmul(unsigned char a, unsigned char b) {
	unsigned char p = 0;
	unsigned char counter;
	unsigned char hi_bit_set;
	for(counter = 0; counter < 8; counter++) {
		if((b & 1) == 1) 
			p ^= a;
		hi_bit_set = (a & 0x80);
		a <<= 1;
		if(hi_bit_set == 0x80) 
			a ^= 0x1b;		
		b >>= 1;
	}
	return p;
}
void InvMixColumns(unsigned char *state) {
        unsigned char a[4];
        unsigned char c;
        for(c=0;c<4;c++) {
                a[c] = state[c];
                }
        state[0] = gmul(a[0],14) ^ gmul(a[3],9) ^ gmul(a[2],13) ^ gmul(a[1],11);
        state[1] = gmul(a[1],14) ^ gmul(a[0],9) ^ gmul(a[3],13) ^ gmul(a[2],11);
        state[2] = gmul(a[2],14) ^ gmul(a[1],9) ^ gmul(a[0],13) ^ gmul(a[3],11);
        state[3] = gmul(a[3],14) ^ gmul(a[2],9) ^ gmul(a[1],13) ^ gmul(a[0],11);
}
void AddRoundKey(unsigned char* state, unsigned char* roundKey)
{
    for(int i = 0; i < 16; i++)
        state[i] ^= roundKey[i];
}

void AES_encrypt(unsigned char* message, unsigned char* key)
{
    unsigned char state[16];
    for(int i = 0; i < 16; i++)
        state[i]=message[i];

    int numberOfRounds = 9;
    unsigned char expandedKey[176];
    KeyExpansion(key, expandedKey);
    AddRoundKey(state, key);

    for (int i = 0; i < numberOfRounds; i++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, expandedKey + (16 * (i + 1)));
    }
        
        SubBytes(state);
        ShiftRows(state);
        AddRoundKey(state, expandedKey + 160);

        for(int i = 0;i<16;i++)
            message[i]= state[i];
}

void AES_decrypt(unsigned char* message, unsigned char* key)
{
    unsigned char state[16];
    for(int i = 0 ; i< 16;i++)
        state[i]=message[i];
    int numberOfRounds = 9;
    InvSubBytes(state);
    InvShiftRows(state);
    for(int i = 0;i<numberOfRounds; i++)
    {
        InvMixColumns(state);
        InvShiftRows(state);
        InvSubBytes(state);
    }
    for(int i =0;i<16;i++)
        message[i]= state[i];
}

int messageSize(char* message)
{
    int counter = 0;
    for(int i = 0; message[i] != '\0'; i++, counter++);
    return counter;
}


