#include <iostream>
#include<cmath>
#include<math.h>
#include<cstdlib>
#include<time.h>
#include <vector>
#include<fstream>
#include <iomanip>

using namespace std;

int routine(int n, double p_lazy);
int stable(vector<int> s, int n, bool ref);
void matrixprint(vector<vector<int> > matrix, int R, int C);
double J(int n, vector<int> currentstate, int i);
void WriteMathematicaFile (vector<double> Function, vector<int> Function2);
void WriteMathematicaFile1 (vector<vector<int> > Function, int n);
int unstablevector(int n, vector<bool> &topplingvector, vector<int> s);
void configprint(int n, vector<int> config);
double r(int n, vector<int> config);
vector<int> Plug(int n, vector<int> config, int unstablesite, int zerosite);


int main (){
	
	cout.flush();
	srand(time(NULL));
	int trials = 200;
	int numberofnvaluesprobed = 15;
	double p_lazy;
	int i,j;
	vector<double> results(numberofnvaluesprobed,0.0);
	int totalsteps;
	vector<int> nvalues = {};
	for(i = 0; i < numberofnvaluesprobed; i++){
		nvalues.push_back(10 + 15*i);
	}
	cout << "(n, result):" << endl;
	for(i = 0; i < numberofnvaluesprobed; i++){
		totalsteps = 0;
		p_lazy = 0.25*log(nvalues[i])/(1.0 + 0.25*log(nvalues[i]));
		for(j = 0; j < trials; j++){
			totalsteps += routine(nvalues[i],p_lazy);
		}
		results[i] = ((double) totalsteps)/((double) trials);
		cout << "(" << nvalues[i] << ", " << setprecision(8) << results[i] << ")" << endl; 
	}

	WriteMathematicaFile(results, nvalues);
	return 0;
}

int routine(int n, double p_lazy){
	vector<vector<int> > s;
	vector<int> currentstate(n);
	vector<double> Jvector;
	int unstablesite;
	int i;
	vector<bool> topplingvector(n);
	bool singletoppling = true;
	int time = 0;
	int result = 0;

	/*currentstate[n-2] = 2;
	currentstate[n-1] = 0;	*/
	for(i = 0; i < n; i++){
		currentstate[i] = 0;
	}

	currentstate[0] = n;

	//currentstate[n/2-1] = n;
	//currentstate = {2,0,2,0,2,0};
	
	s.push_back(currentstate);
	unstablesite = 0;
	bool ref;
	//result = (-1)*r(n,currentstate);
	//cout << result;

	bool atleastoneroundcomplete = false;
	
	

	if(singletoppling == true){
		do{
			if(rand() > p_lazy*RAND_MAX){
				if(rand() < 0.5*RAND_MAX){
					currentstate[unstablesite]--;
					currentstate[(unstablesite+1)%n]++;
				}else{
					currentstate[unstablesite]--;
					currentstate[(unstablesite+n-1)%n]++;
					/*currentstate[unstablesite]--;
					currentstate[(unstablesite+1)%n]++;*/
				}
			}
			if(rand() > p_lazy*RAND_MAX){
				if(rand() < 0.5*RAND_MAX){
					currentstate[unstablesite]--;
					currentstate[(unstablesite+1)%n]++;
				}else{
					currentstate[unstablesite]--;
					currentstate[(unstablesite+n-1)%n]++;
					/*currentstate[unstablesite]--;
					currentstate[(unstablesite+1)%n]++;*/
				}
			}

			s.push_back(currentstate);
			
			unstablesite = stable(currentstate, n, ref);	
			/*if(unstablesite == n/2 - 1 && atleastoneroundcomplete == true){
				cout << "halfway around, time is " << time << endl;
				atleastoneroundcomplete = false;
			}*/
		
			

			/*if(unstablesite == n-1){
				atleastoneroundcomplete = true;
				ref = true;
			}else{
				ref = false;
			}*/
			time++;
			//cout << time << endl;
			
			
		}while(unstablesite < n);
		if(unstablesite < n){
			cout << "unstable" << endl;
		}

		//matrixprint(s, (int) s.size(), n);
		/*for(i = 0; i < (int) s.size(); i++){
			Jvector.push_back(J(n,s[i], i));
		}*/
	}else{
		
		while(unstablevector(n, topplingvector, currentstate) > 0){
		
			for(i = 0; i < n; i++){
				if(topplingvector[i] == true){
					unstablesite = i;
					if(rand() > p_lazy*RAND_MAX){
						if(rand() < 0.5*RAND_MAX){
							currentstate[unstablesite]--;
							currentstate[(unstablesite+1)%n]++;
							if(unstablesite == 0){
								result++;
							}
						}else{
							currentstate[unstablesite]--;
							currentstate[(unstablesite+n-1)%n]++;
							if(unstablesite == 1){
								result--;
							}
						}
					}
					if(rand() > p_lazy*RAND_MAX){
						if(rand() < 0.5*RAND_MAX){
							currentstate[unstablesite]--;
							currentstate[(unstablesite+1)%n]++;
							if(unstablesite == 0){
								result++;
							}
						}else{
							currentstate[unstablesite]--;
							currentstate[(unstablesite+n-1)%n]++;
							if(unstablesite == 1){
								result--;
							}
						}
					}
				}
			}
			s.push_back(currentstate);
			//matrixprint(s, (int) s.size(), n);
		};
		for(i = 0; i < (int) s.size(); i++){
			Jvector.push_back(r(n,s[i]));
		}
	}

	//cout << "here";
	//configprint(n,currentstate);
	//cout << Jvector[((int) s.size())-1];
	//WriteMathematicaFile (Jvector, n);
	//WriteMathematicaFile1(s,n);
	//cout << time << endl;
	return time;
	/*if(currentstate[n-1] == 1){
		//cout << "here";
		return 1;
	}else{
		return 0;
	}*/
	//configprint(n,currentstate);
	//result += r(n, currentstate);
	//return result;
	
}

int stable(vector<int> currentstate, int n, bool ref){
	
	int result = n;
	int max = 1;
	int i;
	i = 1;

	
/*	if(ref == true && currentstate[n-1] > 1){
		result = n-1;
	}else{

		do{
			if(currentstate[((int) (n/2-1+(i/2)*pow(-1,i)+n))%n] > 1 && ((int) (n/2-1+(i/2)*pow(-1,i)+n))%n != n-1){
				result = ((int) (n/2-1+(i/2)*pow(-1,i)+n))%n;
			}
			//cout << i << endl;
			i++;
		}while(result == n && i < n + 1);	

		i = 1;
		while(result == n && i < n + 1){
			if(currentstate[((int) (n/2-1+(i/2)*pow(-1,i)+n))%n] > 1 && ((int) (n/2-1+(i/2)*pow(-1,i)+n))%n != n-1){
				result = ((int) (n/2-1+(i/2)*pow(-1,i)+n))%n;
			}
			//cout << i << endl;
			i++;
		}

		if(result == n && currentstate[n-1] > 1){
			configprint(n,currentstate);
			result = n-1;
		}
	}*/

ref = false;	
vector<int> maxsites(0);
if(ref == true && currentstate[n-1] > 1){
	maxsites.push_back(n-1);
	max = currentstate[n-1];
}else{
	for(i = 0; i < n; i++){
		if(currentstate[i] >= max && currentstate[i] > 1){
			max = currentstate[i];
			maxsites.push_back(i);
			
			break;	
		}
	}
}
	if(max == 2){
		
		for(i = 0; i < (int) maxsites.size(); i++){
			//cout << i << endl;
			if(currentstate[(maxsites[i] +1)%n] == 0 || currentstate[(maxsites[i]+n-1)%n] == 0){
				return maxsites[i];
			}else if(i == (int) maxsites.size()-1){
				return maxsites[0];
			}
		} 
		return maxsites[0];
	}else if(max != 1){
		return maxsites[(int) maxsites.size() - 1];
	}else{
		return n;
	}
	if(max == 1){
		return n;
	}else{
		
		return maxsites[0];
		
	}

	return result;
	
}


void matrixprint(vector<vector<int> > matrix, int R, int C){
		
		int matxpnt1;
		int matxpnt2;
		for(matxpnt1 = 0; matxpnt1 < R; matxpnt1++){
			for(matxpnt2=0; matxpnt2 < C-1; matxpnt2++){
				cout << matrix[matxpnt1][matxpnt2] << "   ";
			}
			cout << matrix[matxpnt1][C-1] <<endl;
		}
}

double J(int n, vector<int> currentstate, int t){
	double result = 0.0;
	int i,j;
	/*for(i = 0; i < n - 1; i++){
		for(j = i + 1; j < n; j++){
			//result = result + currentstate[i]*currentstate[j]*abs(i-j)*(n - abs(i-j));
			//result = result + currentstate[i]*currentstate[j]*sqrt(1-cos(2*3.14159*(i-j)/n));
			//result = result + currentstate[i]*currentstate[j]*pow((1-cos(2*3.14159*(i-j)/n)),1);
		}
	}*/

	//result = (-1)*pow(result,2); 

	double xcom = 0;
	double ycom = 0;
	double xsecond = 0;
	double ysecond = 0;
	for(i = 0; i < n; i++){
		xcom = xcom + currentstate[i]*cos(2*3.14159*i/n);
		ycom = ycom + currentstate[i]*sin(2*3.14159*i/n);
		xsecond = xsecond + currentstate[i]*(cos(2*3.14159*i/n)*cos(2*3.14159*i/n)-sin(2*3.14159*i/n)*sin(2*3.14159*i/n));
		ysecond = ysecond + currentstate[i]*2*cos(2*3.14159*i/n)*sin(2*3.14159*i/n);
	}
	xcom = xcom/n;
	ycom = ycom/n;
	xsecond = xsecond/n;
	ysecond = ysecond/n;
	double firstmoment = sqrt(xcom*xcom + ycom*ycom);
	double secondmoment = sqrt(xsecond*xsecond + ysecond*ysecond);
	result = n*firstmoment + secondmoment;
	return result;
}

void WriteMathematicaFile (vector<double> Function, vector<int> Function2){
	int i, j;

	ofstream outfile;
	string title;
	title = "results";
	outfile.open(title + ".nb", ios::out);
	

	outfile << title + "=";

	for (j = 0; j < (int) Function.size(); j++) {
		if (j == 0) {
			outfile << "{{";
		} else {
			outfile << "{";
		}

		outfile << Function2[j] << ", " << setprecision(8) << Function[j];
		if(j == ((int) Function.size()) - 1){
			outfile << "}};";
		}else{
			outfile << "}, ";
		}
	}

	outfile << endl;
	outfile << "ListPlot[" + title + "]"; 
	
	outfile.close();
}

void WriteMathematicaFile1 (vector<vector<int> > Function, int n){
	int i, j;

	ofstream outfile;
	string title;
	title = "s" + to_string(n);
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

	outfile<< "MatrixForm[s" << to_string(n) << "]" << endl;

	outfile.close();
}

int unstablevector(int n, vector<bool> &topplingvector, vector<int> s){
	int i;
	int result = 0;
	for(i = 0; i < n; i++){
		if(s[i] > 1){
			result++;
			topplingvector[i] = true;
		}else{
			topplingvector[i] = false;
		}
	}
	return result;
}


void configprint(int n, vector<int> config){
	int i;
	for(i = 0; i < n; i++){
		cout << config[i] << "  ";
	}
	cout << endl;
}

double r(int n, vector<int> config){
	
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

		result = ((unstablesite - Lendpoint + n)%n)*((Rendpoint - unstablesite + n)%n) + ((double) ((unstablesite - Lendpoint + n)%n))/((Rendpoint - Lendpoint + n - 1)%n + 1)*r(n,Plug(n,config,unstablesite,Rendpoint)) + ((double) ((Rendpoint - unstablesite + n)%n))/((Rendpoint - Lendpoint + n - 1)%n + 1)*r(n,Plug(n,config,unstablesite,Lendpoint));
		
		
	}
	
	return sqrt(result);
}


vector<int> Plug(int n, vector<int> config, int unstablesite, int zerosite){
	vector<int> result(n);
	result = config;
	result[unstablesite]--;
	result[zerosite]++;
	return result;
}