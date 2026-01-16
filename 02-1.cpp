#include<iostream>
#include<cstdint>
#include<cstdio> 
#include<fcntl.h>
#include<wmmintrin.h>
#include<immintrin.h>
#include<chrono>
#include<ctime>
using namespace std;

FILE *fp=fopen("input.bin","rb");
FILE *p=fopen("output.bin","wb");

/*
#define fp stdin
#define p stdout
*/
struct gf{
	uint64_t num[6]={0};
};
int gf_deg(gf &c){
	for(int i=2;i>=0;--i){
		for(int k=63;k>=0;--k){
			if((c.num[i]&(1ull<<k))!=0){
				return i*64+k;
			}
		}
	}
	return 0;
}
inline void gf_print(const gf c){
	for(int i=0;i<3;++i) fwrite(&c.num[i],sizeof(uint64_t),1,p);
}
inline void gf_add(gf &c,const gf a,const gf b){
	for(int i=0;i<3;++i) c.num[i]=a.num[i]^b.num[i];
}
inline void gf_mul(gf &c,const gf a,const gf b){
	__m128i tmp[6];
    uint64_t t[9];
    t[0]=a.num[1]^a.num[2];
    t[1]=b.num[1]^b.num[2];
    t[2]=a.num[0]^a.num[2];
    t[3]=b.num[0]^b.num[2];
    t[4]=a.num[0]^a.num[1];
    t[5]=b.num[0]^b.num[1];
    __m128i a0=_mm_set_epi64x(0,a.num[0]);
    __m128i a1=_mm_set_epi64x(0,a.num[1]);
    __m128i a2=_mm_set_epi64x(0,a.num[2]);
    __m128i b0=_mm_set_epi64x(0,b.num[0]);
    __m128i b1=_mm_set_epi64x(0,b.num[1]);
    __m128i b2=_mm_set_epi64x(0,b.num[2]);
    __m128i t0=_mm_set_epi64x(0,t[0]);
    __m128i t1=_mm_set_epi64x(0,t[1]);
    __m128i t2=_mm_set_epi64x(0,t[2]);
    __m128i t3=_mm_set_epi64x(0,t[3]);
    __m128i t4=_mm_set_epi64x(0,t[4]);
    __m128i t5=_mm_set_epi64x(0,t[5]);
    tmp[0]=_mm_clmulepi64_si128(t0,t1,0x00);
    tmp[1]=_mm_clmulepi64_si128(t2,t3,0x00);
    tmp[2]=_mm_clmulepi64_si128(t4,t5,0x00);
    tmp[3]=_mm_clmulepi64_si128(a0,b0,0x00);
    tmp[4]=_mm_clmulepi64_si128(a1,b1,0x00);
    tmp[5]=_mm_clmulepi64_si128(a2,b2,0x00);
    t[3]=_mm_extract_epi64(tmp[3],0);
    t[5]=_mm_extract_epi64(tmp[5],0);
    //cout<<hex<<_mm_extract_epi64(tmp[2]^tmp[3]^tmp[4],0)<<" "<<hex<<_mm_extract_epi64(tmp[3],1)<<endl;
    t[6]=_mm_extract_epi64(tmp[0]^tmp[4]^tmp[5],0)^_mm_extract_epi64(tmp[1]^tmp[3]^tmp[4]^tmp[5],1);
    t[7]=_mm_extract_epi64(tmp[2]^tmp[3]^tmp[4],0)^_mm_extract_epi64(tmp[3],1);
    //cout<<hex<<t[7]<<endl;
    t[8]=_mm_extract_epi64(tmp[1]^tmp[3]^tmp[4]^tmp[5],0)^_mm_extract_epi64(tmp[2]^tmp[3]^tmp[4],1);
    c.num[0]=t[3]; 
    c.num[1]=t[7]; 
    c.num[2]=t[8]; 
    c.num[3]=t[6]; 
    c.num[4]=t[5]^_mm_extract_epi64(tmp[0]^tmp[4]^tmp[5],1); 
}
inline void gf_mod(gf &c){
	gf b;
	uint64_t tmp;
	for(int i=4;i>=3;i--){
		tmp=c.num[i];
		c.num[i-3]=c.num[i-3]^(tmp<<61)^(tmp<<62)^(tmp<<63);
		c.num[i-2]=c.num[i-2]^(tmp<<10)^(tmp>>1)^(tmp>>2)^(tmp>>3);
		c.num[i-1]=c.num[i-1]^(tmp>>54);
	}
	tmp=(c.num[2]&0xFFFFFFFFFFFFFFF8);//not 0xFFFFFFFFFFFFFFFC!!!
	c.num[0]=c.num[0]^(tmp<<10)^(tmp>>1)^(tmp>>2)^(tmp>>3);
	c.num[1]=c.num[1]^(tmp>>54);
	c.num[2]=c.num[2]&0x7;
	c.num[3]=c.num[4]=0x0;
}
inline void gf_pow2(gf &c,const gf a){
	__m128i tmp0,tmp1,tmp2;
	__m128i a0=_mm_set_epi64x(0,a.num[0]);
    __m128i a1=_mm_set_epi64x(0,a.num[1]);
    __m128i a2=_mm_set_epi64x(0,a.num[2]);
    tmp0=_mm_clmulepi64_si128(a0,a0,0x00);
    tmp1=_mm_clmulepi64_si128(a1,a1,0x00);
    tmp2=_mm_clmulepi64_si128(a2,a2,0x00);
    c.num[0]=_mm_extract_epi64(tmp0,0);
    c.num[1]=_mm_extract_epi64(tmp0,1);
    c.num[2]=_mm_extract_epi64(tmp1,0);
    c.num[3]=_mm_extract_epi64(tmp1,1);
    c.num[4]=_mm_extract_epi64(tmp2,0);
}
inline void gf_inv(gf &c,const gf a){
	gf tmp;
	for(int i=0;i<3;++i) tmp.num[i]=a.num[i];
	for(int i=1;i<130;++i){
		gf_pow2(tmp,tmp);
		gf_mod(tmp);
		gf_mul(tmp,tmp,a);
		gf_mod(tmp);
	}
	gf_pow2(c,tmp);
	gf_mod(c);
}
inline void gf_inv_it(gf &c,const gf a){
	gf x1,x2,x3,x4,x5,x6,x7,tmp;
	gf_pow2(x1,a);
	gf_mod(x1);
	gf_mul(x1,x1,a);
	gf_mod(x1);
	for(int i=0;i<3;++i) tmp.num[i]=x1.num[i];
	for(int i=1;i<=2;++i){
		gf_pow2(tmp,tmp);
		gf_mod(tmp);
	}
	gf_mul(x2,tmp,x1);
	gf_mod(x2);
	for(int i=0;i<3;++i) tmp.num[i]=x2.num[i];
	for(int i=1;i<=4;++i){
		gf_pow2(tmp,tmp);
		gf_mod(tmp);
	}
	gf_mul(x3,tmp,x2);
	gf_mod(x3);
	for(int i=0;i<3;++i) tmp.num[i]=x3.num[i];
	for(int i=1;i<=8;++i){
		gf_pow2(tmp,tmp);
		gf_mod(tmp);
	}
	gf_mul(x4,tmp,x3);
	gf_mod(x4);
	for(int i=0;i<3;++i) tmp.num[i]=x4.num[i];
	for(int i=1;i<=16;++i){
		gf_pow2(tmp,tmp);
		gf_mod(tmp);
	}
	gf_mul(x5,tmp,x4);
	gf_mod(x5);
	for(int i=0;i<3;++i) tmp.num[i]=x5.num[i];
	for(int i=1;i<=32;++i){
		gf_pow2(tmp,tmp);
		gf_mod(tmp);
	}
	gf_mul(tmp,tmp,x5);
	gf_mod(tmp);
	gf_pow2(tmp,tmp);
	gf_mod(tmp);
	gf_mul(x6,tmp,a);
	gf_mod(x6);
	for(int i=0;i<3;++i) tmp.num[i]=x6.num[i];
	for(int i=1;i<=65;++i){
		gf_pow2(tmp,tmp);
		gf_mod(tmp);
	}
	gf_mul(x7,tmp,x6);
	gf_mod(x7);
	gf_pow2(c,x7);
	gf_mod(c);
}
/*
void gf_pow(gf &c,const gf a,int power){
	if(power==0){
		c.num[0]=0x1;
		for(int i=1;i<5;++i) c.num[i]=0;
		return;
	}
	if(power==1){
		for(int i=0;i<5;++i) c.num[i]=a.num[i];
		return;
	}
	else{
		gf _a;
		for(int i=0;i<5;++i){
			_a.num[i]=a.num[i];
			c.num[i]=0;
		}
		c.num[0]=0x1;
		while(power){
        	if(power&1){
        		gf_mul(c,c,_a);
        		gf_mod(c);
        	}
        	gf_pow2(_a,_a);
        	gf_mod(_a);
        	power>>=1;
    	}
    	gf_mod(c);
	}
}
*/
void gf_inv_gcd(gf &c,const gf a){
	gf u,v,g1,g2,z;
	int j,degu=0,degv=0;
	for(int i=0;i<3;++i) u.num[i]=a.num[i];
	v.num[0]=0x2007;
	v.num[2]=0x8;
	g1.num[0]=0x1;
	while(1){
		if(u.num[0]==0x1 && u.num[1]==0 && u.num[2]==0){
			for(int i=0;i<3;++i) c.num[i]=g1.num[i];
			return;
		}
		degu=gf_deg(u);
		degv=gf_deg(v);
		j=degu-degv;
		if(j<0){
			j=-j;
			gf tmp2;
			for(int i=0;i<3;++i){
				tmp2.num[i]=u.num[i];
				u.num[i]=v.num[i];
				v.num[i]=tmp2.num[i];
				tmp2.num[i]=g1.num[i];
				g1.num[i]=g2.num[i];
				g2.num[i]=tmp2.num[i];
			}
		}
		gf tmp,tmp1,tmp2;
		tmp.num[j/64]=1ull<<(j%64);
		gf_mul(tmp1,tmp,v);
		gf_mod(tmp1);
		gf_add(u,u,tmp1);
		gf_mul(tmp2,tmp,g2);
		gf_mod(tmp2);
		gf_add(g1,g1,tmp2);
		gf_mod(u);
        gf_mod(g1);
        gf_mod(g2);
	}
}
int main(){
	#ifdef _WIN32
	setmode(fileno(stdin), O_BINARY);
	setmode(fileno(stdout), O_BINARY);
	#endif
	uint32_t n,dat_n;
	uint8_t type,dat_type;
	uint64_t dat[8];
	gf a,b,c;
	fread(&dat_n,sizeof(uint32_t),1,fp);
	clock_t start = clock();
	n=dat_n;
	while(n--){
		fread(&dat_type,sizeof(uint8_t),1,fp);
		type=dat_type;
		fread(&dat,sizeof(uint64_t),6,fp);
		for(int i=0;i<3;++i){
			a.num[i]=dat[i];
			b.num[i]=dat[i+3];
		}
		if(type==0) gf_add(c,a,b);
		else if(type==1){
			gf_mul(c,a,b);
			//gf_print(c);
			gf_mod(c);
		}
		else if(type==2){
			gf_pow2(c,a);
			//gf_pow(c,a,2);
			//gf_mul(c,a,a);
			gf_mod(c);
		}
		else{
			//gf_inv(c,a);
			gf_inv_it(c,a);
			//gf_inv_gcd(c,a);
		}
		gf_print(c);
	}
}
