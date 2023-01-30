#include<iostream>
#include<cmath>
#include<math.h>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<fstream>

using namespace std;

bool altsitechecker(int n, vector<int> config, double temperature);
double FE(int n, vector<int> config, int maxsite, double temperature);
vector<vector<int> > admissibleflows(int n, vector<int> config, int maxsite);
vector<vector<int> > manyflows(int n, vector<int> config, int maxsite);
vector<int> flowcalculator(int n, vector<int> config, int maxsite, int flownumber);
void configprint(vector<int> config);
void matrixprint(vector<vector<int> > matrix);
int numberofflows(int n, vector<int> config, int maxsite);
void numberofflowsprint(int n, vector<int> config, int maxsite);
double J(int n, vector<int> config);
vector<int> backwardshexiaryexpansion(int numberofproblemsites, int l);
void singlesitetoppler(int n, vector<int> &currentstate, int site, int move);
double probability(vector<int> topplingvector);



int main (){
	//CHOICES: TEMPERATURE; DISC/CONT FLOW; EXPONENTIAL FUNCTION;
	//OTHER WAYS OF CAPTURING FREE ENERGY?


	srand(time(NULL));

	int trials = 100;
	int n = 20;
	double temperature;
	temperature = pow(((double) n),2);
	//temperature = 1.0/temperature;
	


	double result = 0.0;
	vector<int> config(n);
	vector<int> currentstate(n);
	int k, i, j, l;
	int maxsite, maxheight;
	bool singletoppling = true;
	char function = 'x';
	int tally1 = 0;
	int tally2 = 0;
	vector<vector<int> > problemconfigurations;
	vector<int> problemsites;
	vector<int> topplingvector;
	int x;
	int tempsite;
	int particletally = 0;
	

	for(k = 0; k < trials; k++){
		
		result = 0.0;	
		maxsite = 0;	
		maxheight = 1;
		for(i = 0; i < n; i++){
			config[i] = 0;
		}

		particletally = 0;
		do{
			tempsite = rand()%n;
			if(config[tempsite] < 3){
				config[tempsite]++;
				particletally++;
			}
		}while(particletally < n);

		//CUSTOM CONFIG
		/*for(i = 0; i < n; i++){
			config[i] = 1;
		}
		config[0] = 2;
		config[1] = 0;
		config[45] = 2;
		config[46] = 0;*/
		//config = {1,0,2,1};
	
		

		for(i = 0; i < n; i++){
			//if(config[i] > maxheight){
				maxheight = config[i];
				maxsite = i;
			//}	
		}
		//numberofflowsprint(n,config, maxsite);
			
		configprint(config);
		//cout << numberofflows(n, config, maxsite) << endl;
		
		if(singletoppling == true){
			//SINGLE TOPPLING
			if(maxheight != 1){
				for(i = -1; i <= 1; i++){
					for(j = -1; j <= 1; j++){
						currentstate = config;
						currentstate[maxsite] = config[maxsite] - 2;
						currentstate[(i+n+maxsite)%n] = currentstate[(i+n+maxsite)%n] + 1;  
						currentstate[(j+n+maxsite)%n] = currentstate[(j+n+maxsite)%n] + 1;
						result += 1.0/(pow(2, i*i + j*j + 2))*J(n, currentstate);
						//configprint(currentstate);
						//cout << numberofflows(n, currentstate, maxsite) << endl << endl;
						//cout << FE(n, currentstate, maxsite, temperature) << endl << endl;							
					}
				}	
				//cout << FE(n,config,maxsite,temperature)*n*n*n << endl;	
				result = result - J(n,config);
				//cout << result << endl;
				if(result >= 0){
					//configprint(config);
					problemconfigurations.push_back(config);
					tally1++;
					//configprint(flowcalculator(n, config, maxsite, 0));
				}
			}
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
					
					/*if(function == 's'){
						result += probability(topplingvector)*sqrt(routine(n, currentstate));	
					}else if(function =='l'){
						result += probability(topplingvector)*log(routine(n, currentstate));	
					}else if(function =='x'){*/
						result += probability(topplingvector)*J(n, currentstate);	
					//}
					//configprint(n,currentstate);
				}
				/*if(function == 's'){
					result = result - sqrt(routine(n, config));	
					//cout << result << endl;
					
				}else if(function =='l'){
					result = result - log(routine(n, config));	
				}else if(function =='x'){*/
					result = result - J(n, config);	
					//cout << k << endl;
				//}
				//cout << "result: " << result << endl;
				if(result >= 0){
					tally1++;
					//configprint(n,config);
				}
			}	
		}
	}
	
	cout << endl << endl << "below are problem configs: " << endl;
	for(i = 0; i < (int) problemconfigurations.size(); i++){
		configprint(problemconfigurations[i]);
		if(altsitechecker(n,problemconfigurations[i], temperature) == false){
			tally2++;
			//configprint(problemconfigurations[i]);
			//cout << numberofflows(n, problemconfigurations[i], 0) << endl;
			cout.flush();
		}
	}
	
	cout << "tally1: " << tally1 << endl;
	cout << "tally2: " << tally2 << endl;
	

	return 0;
}

bool altsitechecker(int n, vector<int> config, double temperature){
	int i,j,k, maxsite;
	bool altsiteexists = false;
	double result = 0.0;
	vector<int> currentstate = config;
	for(k = 0; k < n; k++){
		if(config[k] > 1){
			maxsite = k;
			result = 0.0;
			for(i = -1; i <= 1; i++){
				for(j = -1; j <= 1; j++){
					currentstate = config;
					currentstate[maxsite] = config[maxsite] - 2;
					currentstate[(i+n+maxsite)%n] = currentstate[(i+n+maxsite)%n] + 1;  
					currentstate[(j+n+maxsite)%n] = currentstate[(j+n+maxsite)%n] + 1;
					
					result += 1.0/(pow(2, i*i + j*j + 2))*J(n, currentstate);	
				}
			}
			
			result = result - J(n,config);
			
			
			if(result < 0){
				altsiteexists = true;
				break;
			}
		}
	}	
	if(altsiteexists == false){
		//configprint(config);
		//cout << result << endl << endl;
	}
	return altsiteexists;
		
}

double J(int n, vector<int> config){
	double result = 0.0;
	int i,j,k;
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			for(k = 0; k < n; k++){
				result += 0.5*config[i]*config[j]*config[k]*abs(sin((i-j)*2*M_PI/((double) n)) + sin((j-k)*2*M_PI/((double) n)) + sin((k-i)*2*M_PI/((double) n)));
			}
		}
	}
	return result;
}


double FE(int n, vector<int> config, int maxsite, double temperature){
	vector<vector<int> > allflows;
	allflows = manyflows(n,config,maxsite);
	double partitionfunction = 0.0;
	int i,j;
	int elltwosum, ellonesum;
	for(i = 0; i < (int) allflows.size(); i++){
		elltwosum = 0;
		ellonesum = 0;
		for(j = 0; j < n; j++){
			elltwosum += allflows[i][j]*allflows[i][j];
			ellonesum += abs(allflows[i][j]);
		}
		partitionfunction += exp((-1)*(((double) ellonesum)/temperature));
		//partitionfunction += 1.0/pow(((double) ellonesum),2);
	}
	/*configprint(config);*/
	cout << (-1)*(partitionfunction) << endl << endl;
	return (-1)*log(partitionfunction);
	//return 1.0/((double) partitionfunction);
}

vector<vector<int> > admissibleflows(int n, vector<int> config, int maxsite){
	int i;
	vector<vector<int> > result = {};
	//result WILL GIVE THE ADMISSIBLE FLOWS. POSITIVE INTEGERS REPRESENT MOTION TO THE RIGHT FROM MAXSITE,
	//AND NEGATIVE INTEGERS REPRESENT MOTION TO THE LEFT INTO MAXSITE
	bool topcapped = false;
	bool bottomcapped = false;
	vector<int> tempflow;
	result.push_back(flowcalculator(n,config,maxsite,0));
	int flownumber = 1;
	int topcap = 0;
	int bottomcap = 0;
	do{
		if(flownumber > 0){
			if(topcapped == false){
				tempflow = flowcalculator(n,config,maxsite,flownumber);
				for(i = 0; i < n; i++){
					if(tempflow[i] <= 0){
						result.push_back(tempflow);
						break;
					}else if(i == n-1){
						topcapped = true;
						topcap = flownumber - 1;
					}
				}
			}
			flownumber = (-1)*flownumber;
		}else{
			if(bottomcapped == false){
				tempflow = flowcalculator(n,config,maxsite,flownumber);
				for(i = 0; i < n; i++){
					if(tempflow[i] >= 0){
						result.push_back(tempflow);
						break;
					}else if(i == n-1){
						bottomcapped = true;
						bottomcap = flownumber + 1;
					}
				}
			}
			flownumber = (-1)*flownumber + 1;
		}
	}while(topcapped == false || bottomcapped == false);


	return result;
}


vector<vector<int> > manyflows(int n, vector<int> config, int maxsite){
	int i;
	vector<vector<int> > result = {};
	

	vector<int> tempflow;
	result.push_back(flowcalculator(n,config,maxsite,0));
	int flownumber = 1;
	do{
		if(flownumber > 0){
			tempflow = flowcalculator(n,config,maxsite,flownumber);	
			result.push_back(tempflow);		
			flownumber = (-1)*flownumber;
		}else{
			tempflow = flowcalculator(n,config,maxsite,flownumber);
			result.push_back(tempflow);
			flownumber = (-1)*flownumber + 1;
		}
	}while(flownumber < 1000);

	return result;
}

int numberofflows(int n, vector<int> config, int maxsite){
	int i;
	vector<vector<int> > result = {};
	//result WILL GIVE THE ADMISSIBLE FLOWS. POSITIVE INTEGERS REPRESENT MOTION TO THE RIGHT FROM MAXSITE,
	//AND NEGATIVE INTEGERS REPRESENT MOTION TO THE LEFT INTO MAXSITE
	bool topcapped = false;
	bool bottomcapped = false;
	vector<int> tempflow;
	result.push_back(flowcalculator(n,config,maxsite,0));
	int flownumber = 1;
	int topcap = 0;
	int bottomcap = 0;
	
	do{
		if(flownumber > 0){
			if(topcapped == false){
				tempflow = flowcalculator(n,config,maxsite,flownumber);
				for(i = 0; i < n; i++){
					if(tempflow[i] <= 0){
						result.push_back(tempflow);
						break;
					}else if(i == n-1){
						topcapped = true;
						topcap = flownumber - 1;
					}
				}
			}
			flownumber = (-1)*flownumber;
			
		}else{
			if(bottomcapped == false){
				tempflow = flowcalculator(n,config,maxsite,flownumber);
				for(i = 0; i < n; i++){
					if(tempflow[i] >= 0){
						result.push_back(tempflow);
						break;
					}else if(i == n-1){
						bottomcapped = true;
						bottomcap = flownumber + 1;
					}
				}
			}
			flownumber = (-1)*flownumber + 1;
		}
	}while(topcapped == false || bottomcapped == false);
	
	return 1 + topcap - bottomcap;

}

vector<int> flowcalculator(int n, vector<int> config, int maxsite, int flownumber){
	//THE FIRST ENTRY IS THE FLOW GOING AWAY (TO THE RIGHT) OF MAXSITE
	int i;
	vector<int> flow = {};
	flow.push_back(flownumber);
	config[maxsite] = config[maxsite] - flownumber;
	config[(maxsite+1)%n] = config[(maxsite+1)%n] + flownumber;
	for(i = 1; i < n; i++){
		flow.push_back(config[(maxsite+i)%n]-1);
		config[(maxsite+i)%n] = 1;
		config[(maxsite+i+1)%n] += flow[i];
	}
	return flow;
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

void numberofflowsprint(int n, vector<int> config, int maxsite){
	int i,j,k;
	vector<int> currentstate;
	vector<vector<int> > ssmoves;
	ssmoves = {{0,0},{-1,0},{1,0},{-1,1},{-1,-1},{1,1}};
	for(k = 0; k < 6; k++){
		i = ssmoves[k][0];
		j = ssmoves[k][1];
		currentstate = config;
		currentstate[maxsite] = config[maxsite] - 2;
		currentstate[(i+n+maxsite)%n] = currentstate[(i+n+maxsite)%n] + 1;  
		currentstate[(j+n+maxsite)%n] = currentstate[(j+n+maxsite)%n] + 1;	
		cout << numberofflows(n,currentstate,0) << "  ";
		if(k == 0 || k == 2){
			cout << "|  ";
			
		}
	}
	cout << endl;
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