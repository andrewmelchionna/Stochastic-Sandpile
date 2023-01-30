#include <iostream>
#include<cmath>
#include<math.h>
#include<cstdlib>
#include<time.h>
#include <vector>
#include<fstream>

using namespace std;

double J(int n, vector<int> currentstate);
double routine(int n, vector<int> config);
double altroutine(int n, vector<int> config);
vector<int> Plug(int n, vector<int> config, int unstablesite, int zerosite);
void configprint(int n, vector<int> config);
bool altsitechecker(int n, vector<int> config, char function);
vector<int> backwardshexiaryexpansion(int numberofproblemsites, int l);
void singlesitetoppler(int n, vector<int> &currentstate, int site, int move);
double probability(vector<int> topplingvector);

int main(){

	srand(time(NULL));
	char function = 's';
	int trials = 1;
	int n = 20;
	double result = 0.0;
	vector<int> config(n);
	vector<int> currentstate(n);
	int k, i, j, l;
	int maxsite, maxheight;
	int tally1 = 0;
	int tally2 = 0;
	vector<vector<int> > problemconfigurations;
	vector<int> problemsites;
	vector<int> topplingvector;
	int x;
	bool singletoppling = false;

	for(k = 0; k < trials; k++){
		
		result = 0.0;		
		maxheight = 1;
		for(i = 0; i < n; i++){
			config[i] = 0;
		}

		
		for(i = 0; i < n; i++){
			config[rand()%n]++;
		}

		//CUSTOM CONFIG
		/*for(i = 0; i < n; i++){
			config[i] = 1;
		}
		config[0] = 2;
		config[2] = 0;*/
		config = {2,0,2,1,0,1,1,2,0,2,0,2,0,1,1,1,1,1,2,0};
		//cout << "J is: " << sqrt(routine(n,config)) << endl;
		

		for(i = 0; i < n; i++){
			if(config[i] > maxheight){
				maxheight = config[i];
				maxsite = i;
			}	
		}
	
		//maxsite = 9;
		if(singletoppling == true){
			//SINGLE TOPPLING
			if(maxheight != 1){
				for(i = -1; i <= 1; i++){
					for(j = -1; j <= 1; j++){
						currentstate = config;
						currentstate[maxsite] = config[maxsite] - 2;
						currentstate[(i+n+maxsite)%n] = currentstate[(i+n+maxsite)%n] + 1;  
						currentstate[(j+n+maxsite)%n] = currentstate[(j+n+maxsite)%n] + 1;
						if(function == 's'){
							result += 1.0/(pow(2, i*i + j*j + 2))*sqrt(routine(n, currentstate));	
						}else if(function =='l'){
							result += 1.0/(pow(2, i*i + j*j + 2))*log(routine(n, currentstate));	
						}else if(function =='x'){
							result += 1.0/(pow(2, i*i + j*j + 2))*pow(J(n, currentstate),1);	
						}
					}
				}		
				if(function == 's'){
					result = result - sqrt(routine(n,config));
					cout << "result: " << result << endl;
					//cout << k << endl;
					cout.flush();
				}else if(function =='l'){
					result = result - log(routine(n,config));
					cout << k << endl;
					cout.flush();
				}else if(function =='x'){
					result = result - pow(J(n,config),1);
					//cout << k << endl;
					cout.flush();
				}
				if(result >= 0){
					problemconfigurations.push_back(config);
					tally1++;
				}
			}
			//cout << (int) problemconfigurations.size() << endl;
		}else{
			//PARALLEL TOPPLING
			//configprint(n,config);
			problemsites = {};
			topplingvector = {};
			for(i = 0; i < n; i++){
				if(config[i] >= 2){
					problemsites.push_back(i);
				}
			}
			
			//configprint((int) problemsites.size() ,problemsites);
			if((int) problemsites.size() > 0){
				x = pow(6,(int) problemsites.size());
				for(l = 0; l < x; l++){
					currentstate = config;
					topplingvector = backwardshexiaryexpansion((int) problemsites.size(),l);
					for(i = 0; i < (int) problemsites.size(); i++){
						//singlesitetoppler UPDATES THE SANDPILE BY PERFORMING THE MOVE INDEXED BY topplingvector[i] AT SITE problemsites[i]
						singlesitetoppler(n, currentstate, problemsites[i], topplingvector[i]);			
					}	
					
					if(function == 's'){
						result += probability(topplingvector)*sqrt(routine(n, currentstate));	
					}else if(function =='l'){
						result += probability(topplingvector)*log(routine(n, currentstate));	
					}else if(function =='x'){
						result += probability(topplingvector)*routine(n, currentstate);	
					}
					//configprint(n,currentstate);
				}
				if(function == 's'){
					result = result - sqrt(routine(n, config));	
					cout << result << endl;
					
				}else if(function =='l'){
					result = result - log(routine(n, config));	
				}else if(function =='x'){
					result = result - routine(n, config);	
					//cout << k << endl;
				}
				//cout << "result: " << result << endl;
				if(result >= 0){
					tally1++;
					//configprint(n,config);
				}
			}		
		}
	}
	
	for(i = 0; i < (int) problemconfigurations.size(); i++){
		if(altsitechecker(n,problemconfigurations[i], function) == false){
			tally2++;
			configprint(n,problemconfigurations[i]);
		}
	}
	
	cout << "tally1: " << tally1 << endl;
	cout << "tally2: " << tally2 << endl;
	//cout << altroutine(5,{3,0,0,1,1});	

	return 0;
}

double J(int n, vector<int> currentstate){
	int i,j;
	double result = 0.0;
	double dummy = 0.0;
	for(i = 0; i < n; i++){
		dummy = 0.0;
		for(j = 1; j < n; j++){
			dummy += currentstate[j]*min(abs(i-j),(n - abs(i-j)));	
		}
		result += sqrt(dummy);
	}
	return (-1)*(result);
}

double routine(int n, vector<int> config){
	
	double result;
	int unstablesite = n;
	int i;
	int Lendpoint, Rendpoint;
	
	for(i = 0; i < n; i++){
		if(config[i] > 1){
			unstablesite = i;
			break;
		}
	}
	if(unstablesite == n){
		result = 0.0;
	}else{
		Lendpoint = unstablesite;
		Rendpoint = unstablesite;
		do{
			Lendpoint = (Lendpoint + n - 1)%n;
		}while(config[Lendpoint] != 0);
		do{
			Rendpoint = (Rendpoint + 1)%n;
		}while(config[Rendpoint] != 0);

		/*cout << "unstablesite is: " << unstablesite << endl;
		cout << "Lendpoint is: " << Lendpoint << endl;
		cout << "Rendpoint is: " << Rendpoint << endl;*/

		result = ((unstablesite - Lendpoint + n)%n)*((Rendpoint - unstablesite + n)%n) + ((double) ((unstablesite - Lendpoint + n)%n))/((Rendpoint - Lendpoint + n - 1)%n + 1)*routine(n,Plug(n,config,unstablesite,Rendpoint)) + ((double) ((Rendpoint - unstablesite + n)%n))/((Rendpoint - Lendpoint + n - 1)%n + 1)*routine(n,Plug(n,config,unstablesite,Lendpoint));
		
		
	}
	
	return result;
}

double altroutine(int n, vector<int> config){
	
	double result = 0.0;
	int i,j;
	vector<int> zeroes;
	zeroes = {};
	for(i = 0; i < n; i++){
		if(config[i] == 0){
			zeroes.push_back(i);
		}
	}

	for(i = 0; i < n; i++){
		if(config[i] > 1){
			for(j = 0; j < (int) zeroes.size(); j++){
				result += (config[i]-1)*abs(i - zeroes[j])*(n-abs(i - zeroes[j]));
			}
		}
	}
	
	return result;
}

vector<int> Plug(int n, vector<int> config, int unstablesite, int zerosite){
	vector<int> result(n);
	result = config;
	result[unstablesite]--;
	result[zerosite]++;
	return result;

}

void configprint(int n, vector<int> config){
	int i;
	for(i = 0; i < n; i++){
		cout << config[i] << "  ";
	}
	cout << endl;
}

bool altsitechecker(int n, vector<int> config, char function){
	int i,j,k, maxsite;
	bool altsiteexists = false;
	vector<int> currentstate = config;
	for(k = 0; k < n; k++){
		if(config[k] > 1){
			maxsite = k;
			double result = 0.0;
			for(i = -1; i <= 1; i++){
				for(j = -1; j <= 1; j++){
					currentstate = config;
					currentstate[maxsite] = config[maxsite] - 2;
					currentstate[(i+n+maxsite)%n] = currentstate[(i+n+maxsite)%n] + 1;  
					currentstate[(j+n+maxsite)%n] = currentstate[(j+n+maxsite)%n] + 1;
					if(function == 's'){
						result += 1.0/(pow(2, i*i + j*j + 2))*sqrt(routine(n, currentstate));	
					}else if(function == 'l'){
						result += 1.0/(pow(2, i*i + j*j + 2))*log(routine(n, currentstate));	
					}else if(function == 'x'){
						result += 1.0/(pow(2, i*i + j*j + 2))*pow(J(n, currentstate),1);	
					}
				}
			}
			if(function == 's'){	
				result = result - sqrt(routine(n,config));
			}else if(function == 'l'){	
				result = result - log(routine(n,config));
			}else if(function == 'x'){	
				result = result - pow(J(n,config),1);
			}
			if(result < 0){
				altsiteexists = true;
				break;
			}
		}
	}	
	//configprint(n, config);
	//cout << maxsite;
	//cout << endl;
	return altsiteexists;
		
}

vector<int> backwardshexiaryexpansion(int numberofproblemsites, int l){
	vector<int> result = {};
	do{
		result.push_back(l%6);
		l = l/6;
	}while(l!=0);

	while((int) result.size() < numberofproblemsites){
		result.push_back(0);
	}
	return result;
}

void singlesitetoppler(int n, vector<int> &currentstate, int site, int move){
	if(move == 1){
		currentstate[site] = currentstate[site] - 1;
		currentstate[(site + 1)%n]++;
	}else if(move == 2){
		currentstate[site] = currentstate[site] - 1;
		currentstate[(site + n - 1)%n]++;
	}else if(move == 3){
		currentstate[site] = currentstate[site] - 2;
		currentstate[(site + 1)%n]++;
		currentstate[(site + n - 1)%n]++;
	}else if(move == 4){
		currentstate[site] = currentstate[site] - 2;
		currentstate[(site + 1)%n] = currentstate[(site + 1)%n] + 2;
	}else if(move == 5){
		currentstate[site] = currentstate[site] - 2;
		currentstate[(site + n - 1)%n] = currentstate[(site + n - 1)%n] + 2;
	}

}

double probability(vector<int> topplingvector){
	int i;
	double prob = 1.0;
	for(i = 0; i < (int) topplingvector.size(); i++){
		if(topplingvector[i] == 0){
			prob = prob*0.25;
		}else if(topplingvector[i] == 1){		
			prob = prob*0.25;
		}else if(topplingvector[i] == 2){
			prob = prob*0.25;
		}else if(topplingvector[i] == 3){
			prob = prob*0.125;
		}else if(topplingvector[i] == 4){
			prob = prob*0.0625;
		}else if(topplingvector[i] == 5){
			prob = prob*0.0625;
		}
	}
	return prob;
}