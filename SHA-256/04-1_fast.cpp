#include<iostream>
#include<cstring>
#include<cstdint>
#include<fstream>
#include<fcntl.h>
#include<wmmintrin.h>
#include<immintrin.h>
#include<emmintrin.h>
using namespace std;

FILE *fp=fopen("dump (9).bin","rb");
FILE *p=fopen("output.bin","wb");

/*
#define fp stdin
#define p stdout
*/
const uint32_t h[9]={0,0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
					   0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
uint32_t _hash[10]={0};
__m128i state0,state1,msg,msgtmp0,msgtmp1,msgtmp2,msgtmp3,msgtmp4,abef,cdgh;
__m128i shuf_mask=_mm_set_epi32(
    0x0c0d0e0f,
    0x08090a0b,
    0x04050607,
    0x00010203
);
void sha_256(uint8_t tmp[],uint32_t _hash[],int start){
	if(!start){
		state0=_mm_set_epi64x(0x6a09e667bb67ae85,0x510e527f9b05688c);
		state1=_mm_set_epi64x(0x3c6ef372a54ff53a,0x1f83d9ab5be0cd19);
	}
	abef=state0;
	cdgh=state1;
	//round 0~3 
	msg=_mm_loadu_si128((__m128i*)(tmp+0));
	msgtmp0=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0xe9b5dba5b5c0fbcf,0x71374491428a2f98));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
	//round 4~7
	msg=_mm_loadu_si128((__m128i*)(tmp+16));
	msgtmp1=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0xab1c5ed5923f82a4,0x59f111f13956c25b));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	msgtmp0=_mm_sha256msg1_epu32(msgtmp0,msgtmp1);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
	//round 8~11
	msg=_mm_loadu_si128((__m128i*)(tmp+32));
	msgtmp2=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0x550c7dc3243185be,0x12835b01d807aa98));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	msgtmp1=_mm_sha256msg1_epu32(msgtmp1,msgtmp2);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
	//round 12~15
	msg=_mm_loadu_si128((__m128i*)(tmp+48));
	msgtmp3=_mm_shuffle_epi8(msg,shuf_mask);
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0xc19bf1749bdc06a7,0x80deb1fe72be5d74));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp3,msgtmp2,4);
    msgtmp0=_mm_add_epi32(msgtmp0,msgtmp4);
    msgtmp0=_mm_sha256msg2_epu32(msgtmp0,msgtmp3);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp2=_mm_sha256msg1_epu32(msgtmp2,msgtmp3);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 16~19
    msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0x240ca1cc0fc19dc6,0xefbe4786e49b69c1));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp0,msgtmp3,4);
    msgtmp1=_mm_add_epi32(msgtmp1,msgtmp4);
    msgtmp1=_mm_sha256msg2_epu32(msgtmp1,msgtmp0);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp3=_mm_sha256msg1_epu32(msgtmp3,msgtmp0);
   	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 20~23
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0x76f988da5cb0a9dc,0x4a7484aa2de92c6f));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp1,msgtmp0,4);
    msgtmp2=_mm_add_epi32(msgtmp2,msgtmp4);
    msgtmp2=_mm_sha256msg2_epu32(msgtmp2,msgtmp1);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp0=_mm_sha256msg1_epu32(msgtmp0,msgtmp1);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 24~27
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0xbf597fc7b00327c8,0xa831c66d983e5152));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp2,msgtmp1,4);
    msgtmp3=_mm_add_epi32(msgtmp3,msgtmp4);
    msgtmp3=_mm_sha256msg2_epu32(msgtmp3,msgtmp2);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp1=_mm_sha256msg1_epu32(msgtmp1,msgtmp2);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 28~31
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0x1429296706ca6351,0xd5a79147c6e00bf3));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp3,msgtmp2,4);
    msgtmp0=_mm_add_epi32(msgtmp0,msgtmp4);
    msgtmp0=_mm_sha256msg2_epu32(msgtmp0,msgtmp3);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp2=_mm_sha256msg1_epu32(msgtmp2,msgtmp3);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 32~35
	msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0x53380d134d2c6dfc,0x2e1b213827b70a85));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp0,msgtmp3,4);
    msgtmp1=_mm_add_epi32(msgtmp1,msgtmp4);
    msgtmp1=_mm_sha256msg2_epu32(msgtmp1,msgtmp0);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp3=_mm_sha256msg1_epu32(msgtmp3,msgtmp0);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 36~39
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0x92722c8581c2c92e,0x766a0abb650a7354));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp1,msgtmp0,4);
    msgtmp2=_mm_add_epi32(msgtmp2,msgtmp4);
    msgtmp2=_mm_sha256msg2_epu32(msgtmp2,msgtmp1);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp0=_mm_sha256msg1_epu32(msgtmp0,msgtmp1);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 40~43
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0xc76c51a3c24b8b70,0xa81a664ba2bfe8a1));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp2,msgtmp1,4);
    msgtmp3=_mm_add_epi32(msgtmp3,msgtmp4);
    msgtmp3=_mm_sha256msg2_epu32(msgtmp3,msgtmp2);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp1=_mm_sha256msg1_epu32(msgtmp1,msgtmp2);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 44~47
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0x106aa070f40e3585,0xd6990624d192e819));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp3,msgtmp2,4);
    msgtmp0=_mm_add_epi32(msgtmp0,msgtmp4);
    msgtmp0=_mm_sha256msg2_epu32(msgtmp0,msgtmp3);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp2=_mm_sha256msg1_epu32(msgtmp2,msgtmp3);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 48~51
    msg=_mm_add_epi32(msgtmp0,_mm_set_epi64x(0x34b0bcb52748774c,0x1e376c0819a4c116));
    state1=_mm_sha256rnds2_epu32(state1,state0,msg);
    msgtmp4=_mm_alignr_epi8(msgtmp0,msgtmp3,4);
    msgtmp1=_mm_add_epi32(msgtmp1,msgtmp4);
    msgtmp1=_mm_sha256msg2_epu32(msgtmp1,msgtmp0);
    msg=_mm_shuffle_epi32(msg,0x0e);
    state0=_mm_sha256rnds2_epu32(state0,state1,msg);
    msgtmp3=_mm_sha256msg1_epu32(msgtmp3,msgtmp0);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    //round 52~55
	msg=_mm_add_epi32(msgtmp1,_mm_set_epi64x(0x682e6ff35b9cca4f,0x4ed8aa4a391c0cb3));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msgtmp4=_mm_alignr_epi8(msgtmp1,msgtmp0,4);
	msgtmp2=_mm_add_epi32(msgtmp2,msgtmp4);
	msgtmp2=_mm_sha256msg2_epu32(msgtmp2,msgtmp1);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
	//round 56~59
	msg=_mm_add_epi32(msgtmp2,_mm_set_epi64x(0x8cc7020884c87814,0x78a5636f748f82ee));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msgtmp4=_mm_alignr_epi8(msgtmp2,msgtmp1,4);
	msgtmp3=_mm_add_epi32(msgtmp3,msgtmp4);
	msgtmp3=_mm_sha256msg2_epu32(msgtmp3,msgtmp2);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
	//round 60~63
	msg=_mm_add_epi32(msgtmp3,_mm_set_epi64x(0xc67178f2bef9a3f7,0xa4506ceb90befffa));
	state1=_mm_sha256rnds2_epu32(state1,state0,msg);
	msg=_mm_shuffle_epi32(msg,0x0e);
	state0=_mm_sha256rnds2_epu32(state0,state1,msg);
	//cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
	state0=_mm_add_epi32(state0,abef);
    state1=_mm_add_epi32(state1,cdgh);
    //cout<<hex<<state0[0]<<" "<<state0[1]<<endl<<state1[0]<<" "<<state1[1]<<endl;
    _hash[0]=(uint32_t)(state0[1]>>32);
    _hash[1]=(uint32_t)(state0[1]);
    _hash[4]=(uint32_t)(state0[0]>>32);
    _hash[5]=(uint32_t)(state0[0]);
    _hash[2]=(uint32_t)(state1[1]>>32);
    _hash[3]=(uint32_t)(state1[1]);
    _hash[6]=(uint32_t)(state1[0]>>32);
    _hash[7]=(uint32_t)(state1[0]);
    //for(int i=0;i<=7;++i) cout<<hex<<(int)_hash[i]<<" ";
    //cout<<endl;
}
int main(){
	//ios::sync_with_stdio(false);
	#ifdef _WIN32
	setmode(fileno(stdin), O_BINARY);
	setmode(fileno(stdout), O_BINARY);
	#endif
	uint8_t tmp[70];
	uint64_t cnt=0;
	int start=0;
	for(int i=0;i<8;++i) _hash[i]=h[i+1];
	while(!feof(fp)){
		int b_cnt=fread(tmp,sizeof(uint8_t),64,fp);
		if(b_cnt==64){
			sha_256(tmp,_hash,start);
			cnt+=64;
			start=1;
		}
		else if(b_cnt){
			cnt+=b_cnt;
			uint64_t len=cnt<<3;
			if(b_cnt<=55){
				tmp[b_cnt]=0x80;
				for(int i=b_cnt+1;i<56;++i) tmp[i]=0x00;
				for(int i=0;i<8;++i) tmp[63-i]=(len>>(i*8))&0xFF;
				sha_256(tmp,_hash,start);
			}
			else{
				tmp[b_cnt]=0x80;
				for(int i=b_cnt+1;i<64;++i) tmp[i]=0x00;
				sha_256(tmp,_hash,start);
				for(int i=0;i<56;++i) tmp[i]=0x00;
				for(int i=0;i<8;++i) tmp[63-i]=(len>>(i*8))&0xFF;
				sha_256(tmp,_hash,1);
			}
			for(int i=0;i<=7;++i){
				uint32_t ttmp=_hash[i];
				uint8_t tttmp[4];
				for(int j=0;j<=3;++j){
					tttmp[3-j]=ttmp;
					ttmp>>=8;
				}
				fwrite(tttmp,sizeof(uint8_t),4,p);
			}
			break;
		}
		else if(feof(fp)){
			if(cnt==0){
				for(int i=0;i<64;++i) tmp[i]=(i==0)?0x80:0x00;
				sha_256(tmp,_hash,start);
				for(int i=0;i<=7;++i){
					uint32_t ttmp=_hash[i];
					uint8_t tttmp[4];
					for(int j=0;j<=3;++j){
						tttmp[3-j]=ttmp;
						ttmp>>=8;	
					}
					fwrite(tttmp,sizeof(uint8_t),4,p);
				}
			}
			break;
		}
	}
}
