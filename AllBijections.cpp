#include<iostream>
#include<cmath>
#include<math.h>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<fstream>

using namespace std;

bool NoDuplicates(int n, vector<int> map);
void BijectionGenerator(vector<vector<int> > &AllBijections, int n, vector<int> vectorsofar);
void configprint(vector<int> config);
void matrixprint(vector<vector<int> > matrix);
int distance(int n, int siteone, int sitetwo);
void locationmapgenerator(int n, vector<int> config, vector<int> &locationmap);

int main(){
	
	int n = 9;
	int trials = 1;
	int i,j,k;

	double result = 0.0;
	double subresult = 0.0;
	double superresult = 0.0;
	double temp = ((double) n);
	temp = temp*temp;
	int maxheight;
	int maxsite;
	int particletally;
	int tempsite;
	
	vector<vector<int> > AllBijections = {};
	vector<int> config(n,0);
	//config = {1,1,1,1,1,1,1,1};
	vector<int> locationmap(n,n);
	BijectionGenerator(AllBijections, n, {});
	//locationmapgenerator(n, config, locationmap);
	//cout << "here" << endl;

for(k = 0; k < trials; k++){
		maxsite = 0;	
		maxheight = 1;
		for(i = 0; i < n; i++){
			config[i] = 0;
		}

		particletally = 0;
		do{
			tempsite = rand()%n;
			config[tempsite]++;
			particletally++;
		}while(particletally < n);

		//CUSTOM CONFIG
		for(i = 0; i < n; i++){
			config[i] = 1;
		}
		config[0]=2;
		config[1]=0;
		//config = {1,0,2,1};
	
		

		for(i = 0; i < n; i++){
			if(config[i] > maxheight){
				maxheight = config[i];
				maxsite = i;
			}	
		}
	configprint(config);
	for(i = 0; i < n; i++){
		locationmap[i] = n;
	}
	locationmapgenerator(n,config,locationmap);
	superresult = 0.0;
	result = 0.0;
	for(i = 0; i < AllBijections.size(); i++){
		subresult = 0.0;
		for(j = 0; j < n; j++){
			subresult += pow(distance(n,AllBijections[i][j],locationmap[j]),1);
		}
		result += exp(-1.0/temp * (subresult));
	}
	superresult = log(result);
	cout <<(-1)*log(result) << endl;

	config[maxsite]--;
	config[(maxsite+1)%n]++;
	result = 0.0;
	for(i = 0; i < n; i++){
		locationmap[i] = n;
	}
	locationmapgenerator(n,config,locationmap);
	for(i = 0; i < AllBijections.size(); i++){
		subresult = 0.0;
		for(j = 0; j < n; j++){
			subresult += pow(distance(n,AllBijections[i][j],locationmap[j]),1);
		}
		result += exp(-1.0/temp * (subresult));
	}
	superresult -= 0.5*log(result);
	cout << (-1)*log(result) << endl;

	config[(maxsite+1)%n]--;
	config[(maxsite+n-1)%n]++;
	result = 0.0;
	for(i = 0; i < n; i++){
		locationmap[i] = n;
	}
	locationmapgenerator(n,config,locationmap);
	for(i = 0; i < AllBijections.size(); i++){
		subresult = 0.0;
		for(j = 0; j < n; j++){
			subresult += pow(distance(n,AllBijections[i][j],locationmap[j]),1);
		}
		result += exp(-1.0/temp * (subresult));
	}
	superresult -= 0.5*log(result);
	cout <<(-1)*log(result) << endl;
	cout << superresult << endl << endl;
}	
	//cout << "here" << endl;

	return 0;
}

bool NoDuplicates(int n, vector<int> map){
	vector<int> alreadyseen(n,0);
	int i;
	for(i = 0; i < n; i++){
		alreadyseen[map[i]]++;
	}
	for(i = 0; i < n; i++){
		if(alreadyseen[i] != 1){
			return false;
		}
	}
	return true;
}

void BijectionGenerator(vector<vector<int> > &AllBijections, int n, vector<int> vectorsofar){
	int i;
	if(vectorsofar.size() == n){
		if(NoDuplicates(n,vectorsofar) == true){
			AllBijections.push_back(vectorsofar);
		}
	}else{
		for(i = 0; i < n; i++){
			vectorsofar.push_back(i);
			BijectionGenerator(AllBijections,n,vectorsofar);
			vectorsofar.pop_back();
		}
	}
}

void configprint(vector<int> config){
	int i;
	for(i = 0; i < (int) config.size(); i++){
		cout << config[i] << "  ";
	}
	cout << endl;
}

void matrixprint(vector<vector<int> > matrix){
		
		int matxpnt1;
		int matxpnt2;
		for(matxpnt1 = 0; matxpnt1 < (int) matrix.size(); matxpnt1++){
			for(matxpnt2=0; matxpnt2 < ((int) matrix[matxpnt1].size()) - 1; matxpnt2++){
				cout << matrix[matxpnt1][matxpnt2] << "   ";
			}
			cout << matrix[matxpnt1][((int) matrix[matxpnt1].size()) - 1] <<endl;
		}
}

int distance(int n, int siteone, int sitetwo){
	return min(n-abs(siteone-sitetwo),abs(siteone-sitetwo));
}

void locationmapgenerator(int n, vector<int> config, vector<int> &locationmap){
	int i,j;
	j = 0;
	for(i = 0; i < n; i++){
		do{
			if(config[j] > 0){
				locationmap[i] = j;
				config[j]--;
			}else{
				j++;
			}
		}while(locationmap[i] == n); 
	}
}
