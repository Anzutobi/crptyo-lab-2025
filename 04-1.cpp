#include<iostream>
#include<cstring>
#include<cstdint>
#include<fstream>
#include<fcntl.h>
using namespace std;

FILE *fp=fopen("dump2.bin","rb");
FILE *p=fopen("output.bin","wb");

/*
#define fp stdin
#define p stdout
*/
const uint32_t h[9]={0,0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
					   0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
const uint32_t k[65]={0,0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
					    0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
						0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
						0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,	
						0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
						0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
						0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
						0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
						0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
						0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
						0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
						0xd192e819,0xd6990624,0xf40e3585,0x106aa070,	
						0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
						0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
						0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
						0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};

uint32_t A=h[1],B=h[2],C=h[3],D=h[4],E=h[5],F=h[6],G=h[7],H=h[8],W[65],T1,T2;
uint32_t _hash[10]={0};
uint32_t S(uint32_t x,uint8_t l){
	return (x>>l)|(x<<(32-l));
}
uint32_t Ch(uint32_t x,uint32_t y,uint32_t z){
	return (x&y)^(~x&z);
}
uint32_t Maj(uint32_t x,uint32_t y,uint32_t z){
	return (x&y)^(x&z)^(y&z);
}
uint32_t Sigma0(uint32_t x){
	return S(x,2)^S(x,13)^S(x,22);
}
uint32_t Sigma1(uint32_t x){
	return S(x,6)^S(x,11)^S(x,25);
}
uint32_t sigma0(uint32_t x){
	return S(x,7)^S(x,18)^(x>>3);
}
uint32_t sigma1(uint32_t x){
	return S(x,17)^S(x,19)^(x>>10);
}
void sha_256(uint8_t tmp[],uint32_t _hash[],int start){
	if(start){
		A=_hash[0];
		B=_hash[1];
		C=_hash[2];
		D=_hash[3];
		E=_hash[4];
		F=_hash[5];
		G=_hash[6];
		H=_hash[7];
	}
	for(int i=0;i<=15;++i){
		W[i]=((uint32_t)tmp[i*4]<<24)|((uint32_t)tmp[i*4+1]<<16)|((uint32_t)tmp[i*4+2]<<8)|((uint32_t)tmp[i*4+3]);
	}
	for(int i=16;i<=63;++i){
		W[i]=sigma1(W[i-2])+W[i-7]+sigma0(W[i-15])+W[i-16];
	}
	//cout<<hex<<(int)A<<' '<<(int)B<<' '<<(int)C<<' '<<(int)D<<' '<<(int)E<<' '<<(int)F<<' '<<(int)G<<' '<<(int)H<<endl;
	for(int i=0;i<=63;++i){
		T1=H+Sigma1(E)+Ch(E,F,G)+k[i+1]+W[i];
		/*
		cout<<"Ch:"<<hex<<(int)Ch(E,F,G)<<endl;
		cout<<"Sigma1:"<<hex<<(int)Sigma1(E)<<endl;
		cout<<"W[i]:"<<hex<<W[i]<<endl;
		cout<<"T1:"<<hex<<(int)T1<<endl;
		*/
		T2=Sigma0(A)+Maj(A,B,C);
		H=G;
		G=F;
		F=E;
		E=D+T1;
		D=C;
		C=B;
		B=A;
		A=T1+T2;
		//cout<<hex<<(int)A<<' '<<(int)B<<' '<<(int)C<<' '<<(int)D<<' '<<(int)E<<' '<<(int)F<<' '<<(int)G<<' '<<(int)H<<endl;
	}
	_hash[0]+=A;
	_hash[1]+=B;
	_hash[2]+=C;
	_hash[3]+=D;
	_hash[4]+=E;
	_hash[5]+=F;
	_hash[6]+=G;
	_hash[7]+=H;
}

int main(){
	#ifdef _WIN32
	setmode(fileno(stdin), O_BINARY);
	setmode(fileno(stdout), O_BINARY);
	#endif
	uint8_t c,tmp[70],k;
	uint64_t cnt=0;
	int start=0;
	for(int i=0;i<8;++i) _hash[i]=h[i+1];
	while(!feof(fp)){
		fread(&c,sizeof(uint8_t),1,fp);
		if(feof(fp)){
			tmp[cnt%64]=0x80;
			if(cnt%64<=55){
				k=55-cnt%64;
				for(int i=1;i<=k;++i) tmp[cnt%64+i]=0x00;//padding
				cnt<<=3;
				for(int i=0;i<8;++i){
					tmp[63-i]=cnt&0xFF;
					cnt>>=8;
				}
				sha_256(tmp,_hash,start);
			}
			else{
				for(int i=cnt%64+1;i<=63;++i) tmp[i]=0x00;
				sha_256(tmp,_hash,start);
				//for(int i=0;i<=7;++i) cout<<hex<<(int)_hash[i]<<' ';
				//cout<<endl;
				cnt<<=3;
				for(int i=0;i<8;++i){
					tmp[63-i]=cnt&0xFF;
					cnt>>=8;
				}
				for(int i=0;i<56;++i) tmp[i]=0x00;
				sha_256(tmp,_hash,1);
			}
			for(int i=0;i<=7;++i){
				//cout<<hex<<(int)_hash[i]<<' ';
				uint32_t ttmp=_hash[i];
				uint8_t tttmp[4];
				for(int j=0;j<=3;++j){
					tttmp[3-j]=ttmp;
					ttmp>>=8;	
				}
				fwrite(&tttmp,sizeof(uint8_t),4,p);
			}
			break;
		}
		tmp[cnt%64]=c;
		//cout<<cnt<<endl;
		if(cnt%64==63){
			sha_256(tmp,_hash,start);
			/*
			for(int i=0;i<=7;++i){
				cout<<hex<<(int)_hash[i]<<' ';
			}
			*/
		}
		cnt++;
		if(cnt>64) start=1;
	}
}
