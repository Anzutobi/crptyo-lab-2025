#include<iostream>
#include<cstdint>
#include<fstream>
#include<fcntl.h>
#include<wmmintrin.h>
#include<immintrin.h>
#include<emmintrin.h>
using namespace std;

FILE *fp=fopen("dump1.bin","rb");
FILE *p=fopen("output.bin","wb");

/*
#define fp stdin
#define p stdout
*/
//const uint8_t rcon[11]={0,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
__inline void key_expend(uint8_t key[],__m128i w[],__m128i dw[]){
	__m128i tmp=_mm_loadu_si128(reinterpret_cast<const __m128i*>(key)),t,t2;
	w[0]=tmp;
	t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x01);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[1]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x02);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[2]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x04);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[3]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x08);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[4]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x10);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[5]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x20);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[6]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x40);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[7]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x80);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[8]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x1b);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[9]=tmp;
    t=_mm_slli_si128(tmp,4);
    t^=tmp;
    t2=_mm_slli_si128(t,8);
    t^=t2;
    tmp=_mm_aeskeygenassist_si128(tmp,0x36);
    tmp=_mm_shuffle_epi32(tmp,0xFF);
    tmp^=t;
    w[10]=tmp;
    dw[0]=w[0];
	for(int i=1;i<=9;++i) dw[i]=_mm_aesimc_si128(w[i]);
    dw[10]=w[10];
}
__inline void en_AES(uint8_t text[],__m128i w[],uint8_t out[]){
	__m128i state=_mm_loadu_si128(reinterpret_cast<const __m128i*>(text));
	state^=w[0];
	for(int i=1;i<10;++i) state=_mm_aesenc_si128(state,w[i]);
	state=_mm_aesenclast_si128(state,w[10]);
	_mm_storeu_si128(reinterpret_cast<__m128i*>(out),state);
}
__inline void de_AES(uint8_t text[],__m128i dw[],uint8_t out[]){
	__m128i state=_mm_loadu_si128(reinterpret_cast<const __m128i*>(text));
	state^=dw[10];
	for(int i=9;i>=1;i--) state=_mm_aesdec_si128(state,dw[i]);
	state=_mm_aesdeclast_si128(state,dw[0]);
	_mm_storeu_si128(reinterpret_cast<__m128i*>(out),state);
}

int main(){
	#ifdef _WIN32
	setmode(fileno(stdin), O_BINARY);
	setmode(fileno(stdout), O_BINARY);
	#endif
	uint8_t mode,key[16],IV[16],tmp[50];
	uint32_t len;
	fread(&tmp,sizeof(uint8_t),33,fp);
	mode=tmp[0];
	for(int i=1;i<=16;++i){
		key[i-1]=tmp[i];
		IV[i-1]=tmp[i+16];
	}
	fread(&len,sizeof(uint32_t),1,fp);
	int pad_len=16-len%16;//need to paddle pad_len bytes
	//fwrite(&text,sizeof(uint8_t),l,p);
	__m128i w[12],dw[12];
	uint8_t text[20];
	key_expend(key,w,dw);
	if(mode==0x01){
		uint32_t l=len+pad_len;
		uint8_t block[16],out[16];
		for(int i=0;i<l/16-1;++i){
			fread(&text,sizeof(uint8_t),16,fp);
			for(int j=0;j<16;++j){
				block[j]=text[j]^((i==0)?IV[j]:out[j]);
			}
			en_AES(block,w,out);
			fwrite(&out,sizeof(uint8_t),16,p);
		}
		fread(&text,sizeof(uint8_t),16,fp);
		for(int j=0;j<pad_len;++j) text[len%16+j]=pad_len;
		for(int j=0;j<16;++j) block[j]=text[j]^out[j];
		en_AES(block,w,out);
		fwrite(&out,sizeof(uint8_t),16,p);
	}
	else if(mode==0x81){
		uint8_t block[16],out[16],tmp[16];
		for(int i=0;i<len/16-1;++i){
			fread(&text,sizeof(uint8_t),16,fp);
			for(int j=0;j<16;++j){
				block[j]=text[j];
			}
			de_AES(block,dw,out);
			for(int j=0;j<16;++j){
				out[j]^=((i==0)?IV[j]:tmp[j]);
				tmp[j]=block[j];
			}
			fwrite(&out,sizeof(uint8_t),16,p);
		}
		fread(&text,sizeof(uint8_t),16,fp);
		for(int j=0;j<16;++j) block[j]=text[j];
		de_AES(block,dw,out);
		for(int j=0;j<16;++j){
			out[j]^=tmp[j];
			tmp[j]=block[j];
		}
		for(int j=0;j<16;++j){
			if(out[j]==16-j){
				int fl=1;
				for(int k=j;k<16;++k){
					if(out[k]!=out[j]){
						fl=0;
						break;
					}
				}
				if(fl&&j) fwrite(&out,sizeof(uint8_t),j,p);
			}
		}
	}
}
