#include<iostream>
#include<cmath>
using namespace std;
const int MAXM=30;
int y_diff[MAXM][MAXM],mat[MAXM][MAXM],adj[MAXM][MAXM],key[MAXM][MAXM]; 
int rev(int x){//mod 26,x^-1
	x%=26;
	for(int i=1;i<26;i+=2) if(x*i%26==1) return i;
	return -1;
}
int det(int mat[MAXM][MAXM],int m){
    int ans=0;
    if(m==1) return mat[0][0];
	else{
        for(int i=0;i<m;++i){
            int tmp[MAXM][MAXM];
            for(int j=0;j<m;++j){
                for(int k=0;k<m;++k){
                    if(k==i) continue;
                    int col=k<i?k:k-1;
                    tmp[j-1][col]=mat[j][k];
                }
            }
            ans+=((i%2==1)?-1:1)*mat[0][i]*det(tmp,m-1);
        }
    }
    return ans;
}
void com_adj(int mat[MAXM][MAXM],int m){//compute the adjacency matrix
	for(int i=0;i<m;++i){
		for(int j=0;j<m;++j){
			int tmp[MAXM][MAXM];
			for(int k=0;k<m;++k){
				if(k==i) continue;
				int row=k<i?k:k-1;
				for(int l=0;l<m;++l){
					if(l==j) continue;
					int col=l<j?l:l-1;
					tmp[row][col]=mat[k][l];
				}
			}
			adj[j][i]=pow(-1,i+j)*det(tmp,m-1);
		}
	}
}

int main(){
	int m;
	string pl,ci;//plaintext & ciphertext 
	cin>>m;
	cin>>pl>>ci;
	for(int i=0;i<m*m;++i){
		mat[i/m][i%m]=pl[i+m]-pl[i];
		y_diff[i/m][i%m]=ci[i+m]-ci[i];
	}
	com_adj(mat,m);
	int _rev=rev(det(mat,m));
	for(int i=0;i<m;++i){
		for(int j=0;j<m;++j){
			adj[i][j]=((adj[i][j]*_rev)%26+26)%26;
			//cout<<adj[i][j]<<" ";
		}
		//cout<<endl;
	}
	
	int test[MAXM][MAXM]={0};
	for(int i=0;i<m;++i){
		for(int j=0;j<m;++j){
			for(int k=0;k<m;++k){
				test[i][j]+=adj[i][k]*mat[k][j];
			}
			test[i][j]=(test[i][j]%26+26)%26;
			cout<<test[i][j]<<" ";
		}
		cout<<endl;
	}
	
	for(int i=0;i<m;++i){
		for(int j=0;j<m;++j){
			for(int k=0;k<m;++k){
				key[i][j]+=adj[i][k]*y_diff[k][j];
			}
			key[i][j]=(key[i][j]%26+26)%26;
			cout<<key[i][j]<<" ";
		}
		cout<<endl;
	}
	int dif[MAXM]={0};
	for(int i=0;i<m;++i){
		for(int j=0;j<m;++j){
			dif[i]+=(pl[j]-'A')*key[j][i];
		}
		cout<<((ci[i]-'A'-dif[i])%26+26)%26<<" ";
	}
}
