// AES c++11 class
// Input key : 16 unsigned char (256 bit)
// Output	 : 16 unsigned char (256 bit)
// Mode		 : AES-128, AES-192, and AES-256
// Support encipher and decipher
#include <bits/stdc++.h>
using namespace std;
#define mul2(x) ( (x << 1) ^ (0x1b & -(x >> 7)) ) //mengalikan bilangan dengan 2 (shift left) kemudian di mod dengan binary(1011)
#define mul3(x) ( ( (x << 1) ^ (0x1b & -(x >> 7)) ) ^ x)//mengalikan dengan 3 (2 xor 1)
// xtime is a macro that finds the product of {02} and the argument to xtime modulo {1b}  
#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))
// Multiply is a macro used to multiply numbers in the field GF(2^8)
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))

class AES
{
	private:

	int 		  Nb = 4;
	int 		  Nk = 0;
	int 		  Nr = 0;
	int 		  workingOn = 128; //default AES-128
	unsigned char state[4][4];
	unsigned char roundKey[240];
	unsigned char key[32] = {0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f}; //default key
	unsigned char sBox[256] =   {
		//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F
	unsigned char rSBox[256] =
		{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb
		, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb
		, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e
		, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25
		, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92
		, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84
		, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06
		, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b
		, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73
		, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e
		, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b
		, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4
		, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f
		, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef
		, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61
		, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
	unsigned char rCon[255] = {
		0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
		0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
		0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
		0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
		0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
		0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
		0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
		0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
		0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
		0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
		0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
		0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
		0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
		0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
		0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
		0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

	public:

	//constructor
	AES(){
		Nk = workingOn / 32;
		Nr = Nk + 6;
		keyExpansion();
	}
	AES(int bits, unsigned char k[32]){
		if ((bits == 128) || (bits == 192) || (bits == 256)){
			workingOn = bits;
		}
		Nk = workingOn / 32;
		Nr = Nk + 6;
		for (int i = 0; i < 32; i++) key[i] = k[i];
		keyExpansion();
	}

	//substitute/inverse substitute byte
	void subBytes(){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				state[i][j] = sBox[ state[i][j] ];
			}	
		}
	}
	void invSubBytes(){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				state[i][j] = rSBox[ state[i][j] ];
			}	
		}
	}

	//shift/inverse shift rows
	void shiftRows(){
		unsigned char temp;
		//shift baris 1 sebanyak 1 ke kiri
		temp 		= state[1][0];
		state[1][0] = state[1][1];
		state[1][1] = state[1][2];
		state[1][2] = state[1][3];
		state[1][3] = temp;
		//shift baris 2 sebanyak 2 ke kiri
		temp		= state[2][0];
		state[2][0]	= state[2][2];
		state[2][2]	= temp;
		temp		= state[2][1];
		state[2][1] = state[2][3];
		state[2][3] = temp;
		//shift baris 3 sebanyak 3 ke kiri
		temp		= state[3][0];
		state[3][0] = state[3][3];
		state[3][3] = state[3][2];
		state[3][2] = state[3][1];
		state[3][1] = temp;
	}
	void invShiftRows(){
		unsigned char temp;
		//shift baris 1 sebanyak 1 ke kanan	
		temp 		= state[1][3];
		state[1][3] = state[1][2];
		state[1][2] = state[1][1];
		state[1][1] = state[1][0];
		state[1][0] = temp;
		//shift baris 2 sebanyak 2 ke kanan	
		temp 		= state[2][0];
		state[2][0] = state[2][2];
		state[2][2] = temp;
		temp 		= state[2][1];
		state[2][1] = state[2][3];
		state[2][3] = temp;
		//shift baris 3 sebanyak 3 ke kanan
		temp 		=state[3][0];
		state[3][0] = state[3][1];
		state[3][1] = state[3][2];
		state[3][2] = state[3][3];
		state[3][3] = temp;
	}

	//mix/inverse mix columns
	void mixColumns(){
		unsigned char temp[4];
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				temp[j] = state[j][i];
			}
			//mul 2 = multiply dengan 0x02
			//mul 3 = multiply dengan 0x03
			//yang tidak di multiply berarti multiply dengan 0x01
			state[0][i] = mul2(temp[0]) ^ mul3(temp[1]) ^ temp[2] 		^ temp[3];
			state[1][i] = temp[0] 		^ mul2(temp[1]) ^ mul3(temp[2]) ^ temp[3];
			state[2][i] = temp[0] 		^ temp[1] 		^ mul2(temp[2]) ^ mul3(temp[3]);
			state[3][i] = mul3(temp[0]) ^ temp[1] 		^ temp[2] 		^ mul2(temp[3]);
		}
	}
	void invMixColumns(){
		unsigned char temp[4];
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				temp[j] = state[j][i];
			}
			state[0][i] = Multiply(temp[0], 0x0e) ^ Multiply(temp[1], 0x0b) ^ Multiply(temp[2], 0x0d) ^ Multiply(temp[3], 0x09);
			state[1][i] = Multiply(temp[0], 0x09) ^ Multiply(temp[1], 0x0e) ^ Multiply(temp[2], 0x0b) ^ Multiply(temp[3], 0x0d);
			state[2][i] = Multiply(temp[0], 0x0d) ^ Multiply(temp[1], 0x09) ^ Multiply(temp[2], 0x0e) ^ Multiply(temp[3], 0x0b);
			state[3][i] = Multiply(temp[0], 0x0b) ^ Multiply(temp[1], 0x0d) ^ Multiply(temp[2], 0x09) ^ Multiply(temp[3], 0x0e);
		}
	}

	//retrieve round key by expanding main key
	void keyExpansion(){
		int i;
		unsigned char temp[4], tmp;
		for (i = 0; i < Nk; i++){
			for (int j = 0; j < 4; j++){
				roundKey[i*4+j] = key[i*4+j];
			}
		}
		i = Nk;
		while ( i < (Nb * (Nr + 1)) )
		{
			for (int j = 0; j < 4; j++){
				temp[j] = roundKey[(i - 1)*4+j];  
			}
			if (i % Nk == 0){
				//rotword
				tmp 	= temp[0];
				temp[0] = temp[1];
				temp[1] = temp[2];
				temp[2] = temp[3];
				temp[3] = tmp;
				//subword
				temp[0] = sBox[temp[0]];
				temp[1] = sBox[temp[1]];
				temp[2] = sBox[temp[2]];
				temp[3] = sBox[temp[3]];
				//xor dengan rcon
				temp[0] ^= rCon[i/Nk]; // hanya 4byte pertama yang berpengaruh 
			} else if (Nk > 6 && (i % Nk == 4) ){
				//subword
				temp[0] = sBox[temp[0]];
				temp[1] = sBox[temp[1]];
				temp[2] = sBox[temp[2]];
				temp[3] = sBox[temp[3]];
			}
			for (int j = 0; j < 4; j++){
				roundKey[i*4+j] = roundKey[(i - Nk)*4+j] ^ temp[j];  
			}
			i++;
		}
	}

	//menambahkan (xor) round key ke state
	void addRoundKey(int round){
		for (int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				state[j][i] ^= roundKey[round * Nb * 4 + i * Nb + j];
			}
		}
	}

	//cipher and decipher main function
	unsigned char * cipher(unsigned char pt[32]){
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				state[j][i] = pt[i*4 + j];
			}
		}

		addRoundKey(0);

		for (int i = 1; i < Nr; i++){
			subBytes();
			shiftRows();
			mixColumns();
			addRoundKey(i);
		}

		subBytes();
		shiftRows();
		addRoundKey(Nr);


		static unsigned char temp[32];
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				temp[i*4+j]=state[j][i];
			}
		}
		return temp;
	}
	unsigned char * decipher(unsigned char pt[32]){
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				state[j][i] = pt[i*4 + j];
			}
		}

		addRoundKey(Nr);

		for (int i = Nr-1; i > 0; i--){
			invShiftRows();
			invSubBytes();
			addRoundKey(i);
			invMixColumns();
		}

		invShiftRows();
		invSubBytes();
		addRoundKey(0);


		static unsigned char temp[32];
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				temp[i*4+j]=state[j][i];
			}
		}
		return temp;
	}
};

int main(){

	//plaintext
	unsigned char temp[] = {0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f};
	//key
	unsigned char temp2[]= {0x00  ,0x11  ,0x22  ,0x33  ,0x44  ,0x55  ,0x66  ,0x77  ,0x88  ,0x99  ,0xaa  ,0xbb  ,0xcc  ,0xdd  ,0xee  ,0xff};
	
	AES test(128,temp);
	unsigned char * ot;
	
	//test cipher
	ot = test.cipher(temp2);
	printf("\nPlaintext:\n");
	for(int i = 0; i < 4*4; i++)
	{
		printf("%02x ",temp2[i]);
	}
	printf("\n\n");
	printf("\nText after encryption:\n");
	for(int i = 0; i < 4*4; i++)
	{
		printf("%02x ",ot[i]);
	}
	printf("\n\n");
	
	//test decipher
	ot = test.decipher(ot);
	printf("\nText after decryption:\n");
	for(int i = 0; i < 4*4; i++)
	{
		printf("%02x ",ot[i]);
	}
	printf("\n\n");

}
