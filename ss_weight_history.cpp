#include <iostream>
#include<cmath>
#include<math.h>
#include<cstdlib>
#include<time.h>
#include <vector>
#include<fstream>

using namespace std;

int greedystable(vector<int> currentstate, int n, vector<vector<int> > &problemconfigurations);
double expectedstep(vector<int> config, int site, int n);
double J(int n, vector<int> config);
vector<int> Plug(int n, vector<int> config, int unstablesite, int zerosite);
void WriteMathematicaFile (vector<vector<int> > Function, int n, string titlepiece);
void paralleltoppling(int n, vector<int> config);
vector<int> backwardshexiaryexpansion(int numberofproblemsites, int l);
void singlesitetoppler(int n, vector<int> &currentstate, int site, int move);
double probability(vector<int> topplingvector);
void configprint(int n, vector<int> config);
	

int main (){
	//AT EACH TIMESTEP, TOPPLE A SITE WHICH CAUSES A DECREASE IN EXPECTATION OF SQRT EXPECTED IDLA TIME. IF NO SUCH SITE EXISTS, PRINT AN ERROR MESSAGE

	srand(time(NULL));
	int n = 20;
	int i,j,k;

	vector<int> currentstate(n);
	vector<vector<int> > s = {};
	vector<vector<int> > problemconfigurations = {};
	int unstablesite;
	bool singletoppling = true;


	for(i = 0; i < n; i++){
		currentstate[i] = 0;
	}

	currentstate[n/2] = n;
	unstablesite = n/2;
	s.push_back(currentstate);

	if(singletoppling == true){
		do{
			if(rand() > 0.5*RAND_MAX){
				if(rand() < 0.5*RAND_MAX){
					currentstate[unstablesite]--;
					currentstate[(unstablesite+1)%n]++;
				}else{
					currentstate[unstablesite]--;
					currentstate[(unstablesite+n-1)%n]++;
				}
			}
			if(rand() > 0.5*RAND_MAX){
				if(rand() < 0.5*RAND_MAX){
					currentstate[unstablesite]--;
					currentstate[(unstablesite+1)%n]++;
				}else{
					currentstate[unstablesite]--;
					currentstate[(unstablesite+n-1)%n]++;
				}
			}
			
			unstablesite = greedystable(currentstate, n, problemconfigurations);
			s.push_back(currentstate);
	
		}while(unstablesite < n);
	}
	
	WriteMathematicaFile (s,n,"s");
	WriteMathematicaFile (problemconfigurations,n,"problems");

	for(i = 0; i < (int) problemconfigurations.size(); i++){
		paralleltoppling(n,problemconfigurations[i]);
	}

	return 0;
}	

int greedystable(vector<int> currentstate, int n, vector<vector<int> > &problemconfigurations){
	int result;
	vector<int> unstablesites = {};
	int i;
	double runningmin = 0.0;
	int runningsitetotopple = n;
	double temp;

	for(i = 0; i < n; i++){
		if(currentstate[i] > 1){
			unstablesites.push_back(i);
		}
	}

	if((int) unstablesites.size() == 0){
		return n;
	}else{
		for(i = 0; i < (int) unstablesites.size(); i++){
			temp = expectedstep(currentstate, unstablesites[i], n);
			if(temp < runningmin){
				runningmin = temp;
				runningsitetotopple = unstablesites[i];
			}
		}
		if(runningsitetotopple < n){
			return runningsitetotopple;
		}else{
			problemconfigurations.push_back(currentstate);
			cout << "PROBLEM: NO SITE TO TOPPLE WHICH DECREASES EXPECTATION" << endl;
			return unstablesites[0];
		}
	}
	return result;
}

double expectedstep(vector<int> config, int site, int n){
	int i,j;
	vector<int> currentstate;
	double result = 0.0;
	for(i = -1; i <= 1; i++){
		for(j = -1; j <= 1; j++){
			currentstate = config;
			currentstate[site] = config[site] - 2;
			currentstate[(i+n+site)%n] = currentstate[(i+n+site)%n] + 1;  
			currentstate[(j+n+site)%n] = currentstate[(j+n+site)%n] + 1;
			result += 1.0/(pow(2, i*i + j*j + 2))*sqrt(J(n, currentstate));	
		}
	}		
	result = result - sqrt(J(n,config));
	return result;
}

double J(int n, vector<int> config){
	
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

		result = ((unstablesite - Lendpoint + n)%n)*((Rendpoint - unstablesite + n)%n) + ((double) ((unstablesite - Lendpoint + n)%n))/((Rendpoint - Lendpoint + n - 1)%n + 1)*J(n,Plug(n,config,unstablesite,Rendpoint)) + ((double) ((Rendpoint - unstablesite + n)%n))/((Rendpoint - Lendpoint + n - 1)%n + 1)*J(n,Plug(n,config,unstablesite,Lendpoint));
		
		
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

void WriteMathematicaFile (vector<vector<int> > Function, int n, string titlepiece){
	int i, j;

	ofstream outfile;
	string title;
	title = titlepiece + to_string(n);
	outfile.open(title + ".nb", ios::out);
	

	outfile << title + "=";

	for (j = 0; j < (int) Function.size(); j++) {
		if (j == 0) {
			outfile << "{{";
		} else {
			outfile << "{";
		}

		for (i = 0; i < n; i++) {

			outfile << Function[j][i];
			if (i != n-1){
				outfile << ",";
			} else if (j != (int) Function.size() - 1){
				outfile << "},";
			} else if (j ==  (int) Function.size() - 1){
				outfile << "}};";
			}
		}
	}

	outfile << endl;

	outfile<< "MatrixForm[" << title << "]" << endl;

	outfile.close();
}

void paralleltoppling(int n, vector<int> config){
	double result = 0.0;
	int x,i,l;
	vector<int> problemsites = {};
	vector<int> topplingvector = {};
	vector<int> currentstate;
	for(i = 0; i < n; i++){
		if(config[i] >= 2){
			problemsites.push_back(i);
		}
	}
	
	if((int) problemsites.size() > 0){
		x = pow(6,(int) problemsites.size());
		for(l = 0; l < x; l++){
			currentstate = config;
			topplingvector = backwardshexiaryexpansion((int) problemsites.size(),l);
			for(i = 0; i < (int) problemsites.size(); i++){
				//singlesitetoppler UPDATES THE SANDPILE BY PERFORMING THE MOVE INDEXED BY topplingvector[i] AT SITE problemsites[i]
				singlesitetoppler(n, currentstate, problemsites[i], topplingvector[i]);			
			}	
			
			
			result += probability(topplingvector)*sqrt(J(n, currentstate));	
			
			//configprint(n,currentstate);
		}
		
		result = result - sqrt(J(n, config));	
		//cout << result << endl;
			
		
		//cout << "result: " << result << endl;
		if(result >= 0){
			cout << "PROBLEM: PARALLEL TOPPLING DOES NOT DECREASE IN EXPECTATION" << endl;
			configprint(n,config);
		}
	}		
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

void configprint(int n, vector<int> config){
	int i;
	for(i = 0; i < n; i++){
		cout << config[i] << "  ";
	}
	cout << endl;
}
