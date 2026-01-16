#include<iostream>
#include<algorithm>
#include<vector>
#include<cmath>
#include<map>
using namespace std;
double fre[26]={0.082,0.015,0.028,0.043,0.127,
				0.022,0.020,0.061,0.070,0.002,
				0.008,0.040,0.024,0.067,0.075,
				0.019,0.001,0.060,0.063,0.091,
				0.028,0.010,0.023,0.001,0.020,
				0.001};//frequency in normal English sentences
string str,all_big="";
map<string,vector<int>> a;
int guess_cycle(){
	int len[501]={0},cnt=0,fl=0;
	for(auto it=a.begin();it!=a.end();it++){
		if(fl==1) break;
		if(it->second.size()>=2){
			int k=it->second.size();
			for(int j=1;j<k;++j){
				len[cnt++]=(it->second[j])-(it->second[j-1]);
				if(cnt>=500) fl=1;
			}
		}
	}
	int tmp_gcd=len[0];
	for(int i=1;i<cnt;++i){
		tmp_gcd=__gcd(tmp_gcd,len[i]);
	}
	//cout<<tmp_gcd<<endl;
	return tmp_gcd;
}
string key="";
void decrypt(int cycle){
	string substr[cycle];
	int n=all_big.length();
	int subcnt[cycle][26]={0};//count of each substrings
	int subtot[cycle]={0};
	double M[cycle][26]={0};
	for(int i=0;i<cycle;++i) substr[i]="";
	for(int i=0;i<n;++i){
		substr[i%cycle]+=all_big[i];
		subcnt[i%cycle][all_big[i]-'A']++;
		subtot[i%cycle]++;
	}
	for(int i=0;i<cycle;++i){
		int sublen=substr[i].length();
		int mov_k=0;
		double eps=0.5;
		for(int k=0;k<26;++k){
			for(int j=0;j<26;++j){
				M[i][k]+=fre[j]*subcnt[i][(j+k)%26]/sublen;
			}
			//cout<<M[i][k]<<" ";
			double d_dis=M[i][k]-0.065;
			if(d_dis<0) d_dis=-d_dis;
			if(d_dis<eps){
				//cout<<d_dis<<endl;
				eps=d_dis;
				mov_k=k;
			}
		}
		key+='A'+mov_k;
	}
	cout<<key<<endl;
}
string line_str[500001];
int main(){
	int l_tot=0;//total num of letters
	int line_cnt=0;
	while(getline(cin,str)){
		int n=str.length(),flag=1;
		line_str[line_cnt++]=str;
		string tmp="";//to save into the map,all capital
		for(int i=0;i<n;++i){
			if(str[i]>='A' && str[i]<='Z'){
				flag=1;
				tmp+=str[i];
				all_big+=str[i];
				l_tot++;
			}
			else if(str[i]>='a' && str[i]<='z'){
				flag=1;
				tmp+='A'+str[i]-'a';
				all_big+='A'+str[i]-'a';
				l_tot++;
			}
			else{
				if(flag){//first time not letter
					a[tmp].push_back(l_tot);
					flag=0;
				}
				tmp="";
			}
		}
	}
	int cycle=guess_cycle();
	decrypt(cycle);
	l_tot=0;
	for(int i=0;i<line_cnt;++i){
		int str_len=line_str[i].length();
		for(int j=0;j<str_len;++j){
			if(line_str[i][j]>='a' && line_str[i][j]<='z'){
				if(line_str[i][j]-'a'-(key[l_tot%cycle]-'A')>=0){
					cout<<char(line_str[i][j]-key[l_tot%cycle]+'A');
				}
				else cout<<char(26+line_str[i][j]-key[l_tot%cycle]+'A');
				l_tot++;
			}
			else if(line_str[i][j]>='A' && line_str[i][j]<='Z'){
				if(line_str[i][j]-key[l_tot%cycle]>=0){
					cout<<char(line_str[i][j]-key[l_tot%cycle]+'A');
				}
				else cout<<char('A'+26+line_str[i][j]-key[l_tot%cycle]);
				l_tot++;
			}
			else cout<<line_str[i][j];
		}
		cout<<endl;
	}
}
