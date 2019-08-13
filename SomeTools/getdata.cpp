#include<cstdio>
#include<iostream>
#include<algorithm>
#include<queue>
#include<map>
#include<vector>
#include<set>
#include<string>
#include<cmath>
#include<cstring>
#include<ctime>
#define ll long long

using namespace std;


	

map<string,float> m;


int main(int argc, char const *argv[]){
	
	FILE *fp;
	char line[1000];
	fp=fopen("Data/most20.txt","r");
	if(fp==NULL){
		printf("can not load file!");
		return 1;
	}
	int u = 0;
	while(!feof(fp)){
		char a[32];
		float c;
		fscanf (fp, "%s %f",a,&c );
		fgets(line,1000,fp);
		string s1 = string(a);
		m[s1] = c;
		cout << a << " " << c << endl;
	}
	
	fclose(fp);

	return 0;
}