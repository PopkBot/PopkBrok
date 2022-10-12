#pragma once
#include<thread>
#include <stdio.h>    
#include <stdlib.h> 


#include<iostream>
#include <chrono>
#include<time.h>

#include <iomanip>
#include <fstream>
#include <filesystem>


#include<math.h>

using namespace std;
using std::filesystem::directory_iterator;
namespace fs = std::filesystem;

#define xfac 2.0f
#define mutationRate 0.05		//<1

#define outputCount 6
#define inpuCount 50
//#define h1NodesCount 18
//#define h2NodesCount 18
#define hiddenLayersCount 3
#define hiddenNodesCount 12
#define nodeCount outputCount+inpuCount+hiddenLayersCount*hiddenNodesCount+1

#define layerCount 2+hiddenLayersCount


//#define LoadFNumber 0

#define bpCoeff0 0.25


class brain
{
public:

	double bpCoeff = bpCoeff0;
	
	struct node
	{
		bool n[nodeCount];
		double w[nodeCount];
		double bias;
		double z;
		double outVal;
		bool nEnable;
		int nNum;
	};


	struct layer
	{
		node nodes[nodeCount];
		int nCount;

	};

	long double score;

	

	layer l[layerCount + 1];

	node nodes[nodeCount + 1];

	double da[60][layerCount];
	double dw[60][60][layerCount];
	double db[60][layerCount];

	double dCw[60][60][layerCount];
	double dCb[60][layerCount];

	double dCw_prev[60][60][layerCount];
	double dCb_prev[60][layerCount];

	int backPropCount = 0;
	int batchCount = 0;

	

	void initialize() {
		layer input, output,h[hiddenLayersCount];
		//srand(time(0));

		for (int i = 0; i <= nodeCount; i++) {
			nodes[i].nNum = i;
			for (int j = 0; j <= nodeCount; j++)
			{
				//nodes[i].n[j] = false;
				nodes[i].w[j] = 0;// rand() % 2000 / 100.0f - 10.0f;
				nodes[i].bias = 0;
				nodes[i].z = 0;
				nodes[i].outVal = 0;
			}
		}

		input.nCount = inpuCount;


		output.nCount = outputCount;


		//h1.nCount = h1NodesCount;


	//	h2.nCount = h2NodesCount;


		int nodeN = 1;
		for (int i = 1; i <= inpuCount; i++) {
			input.nodes[i] = nodes[nodeN];
			nodeN++;
		}
		for (int i = 1; i <= outputCount; i++) {
			output.nodes[i] = nodes[nodeN];
			nodeN++;
		}
		for (int j = 0; j < hiddenLayersCount; j++) {
			for (int i = 1; i <= hiddenNodesCount; i++) {
				h[j].nCount = hiddenNodesCount;
				h[j].nodes[i] = nodes[nodeN];
				nodeN++;
			}
		}
		//for (int i = 1; i <= h2NodesCount; i++) {
		//	h2.nodes[i] = nodes[nodeN];
		//	nodeN++;
		//}

		l[1] = input;
		//l[2] = h[0];
		//l[3] = h[1];
		l[layerCount] = output;

		for (int i = 2; i < layerCount ; i++) {
			l[i] = h[i - 2];
		}


		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				dCb[i][lnum] = 0;
				dCb_prev[i][lnum] = 0;
				//printf("\ndb[%d][%d] %10f",i,lnum,  db[i][lnum]);
				for (int k = 1; k <= l[lnum - 1].nCount; k++) {
					dCw[i][k][lnum] =0;
					dCw_prev[i][k][lnum] = 0;
					//printf("\ndw[%d][%d][%d] %10f",i,k,lnum,  dw[i][k][lnum]);
				}
			}
		}

	
		
		printf("\n\n\_____initializing  complete_______\n\n");
	}




	 double sigmoid( double val) {
		return(1 / (1 + 1 / exp(val)));
	}
	 double sigmDeriv(double val) {

		 return(sigmoid(val) * (1 - sigmoid(val)));
	 }


	void thinK(double inp[], double outp[]) {
		
		for (int i = 1; i <= l[1].nCount; i++) {
			l[1].nodes[i].outVal = inp[i - 1];
		}

		layersCalc();

		for (int i = 1; i <= l[layerCount].nCount; i++) {

			outp[i - 1] = l[layerCount].nodes[i].outVal;
		}
	}
	
	void layersCalc() {
		double val = 0;
		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				val = 0;
				
				for (int k = 1; k <= l[lnum - 1].nCount; k++) {
					val += l[lnum - 1].nodes[k].outVal * l[lnum].nodes[i].w[l[lnum - 1].nodes[k].nNum];
				}
				
				val += l[lnum].nodes[i].bias;
				l[lnum].nodes[i].outVal = sigmoid(val);
				l[lnum].nodes[i].z = val;
			}
		}


	}




	void backProp(double y[]) {
		//printf("\n\n\_____backprop _______\n\n");
		


		for (int i = 1; i <= l[layerCount].nCount; i++) {
			da[i][layerCount] = 2 * (l[layerCount].nodes[i].outVal - y[i - 1]);
			//printf("\nda[%d][4]\t%5f", i, da[i][4]);
		}
		for (int j = 1; j < layerCount; j++) {
			for (int k = 1; k <= l[layerCount - j].nCount; k++) {
				da[k][layerCount - j] = 0;
				for (int i = 1; i <=l[layerCount - j + 1].nCount; i++) {
					da[k][layerCount - j] += 
						da[i][layerCount - j + 1] * 
						sigmDeriv(l[layerCount - j + 1].nodes[i].z) * 
						l[layerCount - j + 1].nodes[i].w[l[layerCount - j].nodes[k].nNum];
				}
			}
		}
		
		for (int j = 0; j < layerCount; j++) {
			for (int i = 1; i <= l[layerCount - j].nCount; i++) {
				for (int k = 1; k <= l[layerCount - j - 1].nCount; k++) {
					dw[i][k][layerCount - j] =
						l[layerCount - j - 1].nodes[k].outVal *
						sigmDeriv(l[layerCount - j].nodes[i].z)*
						da[i][layerCount - j];
				}
			}
		}
		for (int j = 0; j < layerCount; j++) {
			for (int i = 1; i <= l[layerCount - j].nCount; i++) {
					db[i][layerCount - j] =
						sigmDeriv(l[layerCount - j].nodes[i].z)*
						da[i][layerCount];
						//printf("\nFLAG---%d", l[layerCount - j].nCount);
				
			}
		}

		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				l[lnum].nodes[i].bias -= bpCoeff * db[i][lnum];
				//printf("\ndb[%d][%d] %10f",i,lnum,  db[i][lnum]);
				for (int k = 1; k <= l[lnum - 1].nCount; k++) {
					l[lnum].nodes[i].w[l[lnum - 1].nodes[k].nNum] -= bpCoeff * dw[i][k][lnum];
					//printf("\ndw[%d][%d][%d] %10f",i,k,lnum,  dw[i][k][lnum]);
				}
			}
		}


	}

	void batchBackProp(double y[]) {


	



		for (int i = 1; i <= l[layerCount].nCount; i++) {
			da[i][layerCount] = 2 * (l[layerCount].nodes[i].outVal - y[i - 1]);
			//printf("\nda[%d][4]\t%5f", i, da[i][4]);
		}
		for (int j = 1; j < layerCount; j++) {
			for (int k = 1; k <= l[layerCount - j].nCount; k++) {
				da[k][layerCount - j] = 0;
				for (int i = 1; i <= l[layerCount - j + 1].nCount; i++) {
					da[k][layerCount - j] +=
						da[i][layerCount - j + 1] *
						sigmDeriv(l[layerCount - j + 1].nodes[i].z) *
						l[layerCount - j + 1].nodes[i].w[l[layerCount - j].nodes[k].nNum];
				}
			}
		}

		for (int j = 0; j < layerCount; j++) {
			for (int i = 1; i <= l[layerCount - j].nCount; i++) {
				for (int k = 1; k <= l[layerCount - j - 1].nCount; k++) {
					dw[i][k][layerCount - j] =
						l[layerCount - j - 1].nodes[k].outVal *
						sigmDeriv(l[layerCount - j].nodes[i].z) *
						da[i][layerCount - j];
				}
			}
		}
		for (int j = 0; j < layerCount; j++) {
			for (int i = 1; i <= l[layerCount - j].nCount; i++) {
				db[i][layerCount - j] =
					sigmDeriv(l[layerCount - j].nodes[i].z) *
					da[i][layerCount];
				//printf("\nFLAG---%d", l[layerCount - j].nCount);

			}
		}

		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				dCb[i][lnum] -=  db[i][lnum];
				//printf("\ndb[%d][%d] %10f",i,lnum,  db[i][lnum]);
				for (int k = 1; k <= l[lnum - 1].nCount; k++) {



					dCw[i][k][lnum] -=  dw[i][k][lnum];
					//printf("\ndw[%d][%d][%d] %10f",i,k,lnum,  dw[i][k][lnum]);
				}
			}
		}
	

	}

	void batchBackProp_apply(int batchSize) {
		double coeffA;
		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				coeffA = 1;
				if (dCb[i][lnum] * dCb_prev[i][lnum] < 0) {
				//	printf("\n===================b========\n");
					//coeffA = 0.05;
				}
				else {
					//coeffA = 2;
					
				}
				dCb_prev[i][lnum] = dCb[i][lnum];
				l[lnum].nodes[i].bias += (bpCoeff * dCb[i][lnum]/ batchSize);
				//printf("\ndb[%d][%d] %10f",i,lnum,  db[i][lnum]);
				for (int k = 1; k <= l[lnum - 1].nCount; k++) {

					coeffA = 1;
					if (dCw[i][k][lnum] * dCw_prev[i][k][lnum] < 0) {
					//	printf("\n===================w========\n");
						//coeffA = 0.05;
					}
					else {
						//coeffA = 2;
						
					}
					
					dCw_prev[i][k][lnum] = dCw[i][k][lnum];
					l[lnum].nodes[i].w[l[lnum - 1].nodes[k].nNum] += (bpCoeff * dCw[i][k][lnum]/ batchSize);
					//printf("\ndw[%d][%d][%d] %10f",i,k,lnum,  dw[i][k][lnum]);
				}
			}
		}

		for (int lnum = 2; lnum <= layerCount; lnum++) {
			for (int i = 1; i <= l[lnum].nCount; i++) {
				dCb[i][lnum] =0;
				//printf("\ndb[%d][%d] %10f",i,lnum,  db[i][lnum]);
				for (int k = 1; k <= l[lnum - 1].nCount; k++) {
					dCw[i][k][lnum] =0;
					//printf("\ndw[%d][%d][%d] %10f",i,k,lnum,  dw[i][k][lnum]);
				}
			}
		}
	}

	void saveToFile(string path) {

		
		ofstream outFile;
		
		int lastF = lastFileInt(path);
		outFile.open(path +to_string(lastF)+".txt");
		



		for (int n = 2; n <= layerCount ; n++) {
			for (int j = 1; j <= l[n].nCount; j++) {
				for (int i = 1; i <= l[n-1].nCount; i++) {

					outFile << l[n].nodes[j].w[l[n-1].nodes[i].nNum];
					printf("\nw[%d][%d][%d]=%5f", i, j, n, l[n].nodes[j].w[l[n - 1].nodes[i].nNum]);
					outFile << "\n";
				}
				outFile << l[n].nodes[j].bias;
				printf("\nb[%d][%d]=%5f", j, n, l[n].nodes[j].bias);
				outFile << "\n";
			}
		}

		outFile.close();
	}

	void quickSave() {
		ofstream outFile;

		
		outFile.open("quickSave.txt");




		for (int n = 2; n <= layerCount; n++) {
			for (int j = 1; j <= l[n].nCount; j++) {
				for (int i = 1; i <= l[n - 1].nCount; i++) {

					outFile << l[n].nodes[j].w[l[n - 1].nodes[i].nNum];
					//printf("\nw[%d][%d][%d]=%5f", i, j, n, l[n].nodes[j].w[l[n - 1].nodes[i].nNum]);
					outFile << "\n";
				}
				outFile << l[n].nodes[j].bias;
				//printf("\nb[%d][%d]=%5f", j, n, l[n].nodes[j].bias);
				outFile << "\n";
			}
		}
		printf("\n quick saved\n");
		outFile.close();
	}

	void loadFromFile(string path) {

		ifstream inFile;
		inFile.open(path);
		if (!inFile) {
			cout << "Unable to open file";
			initialize();
		}
		else {
			initialize();
			for (int n = 2; n <= layerCount; n++) {
				for (int j = 1; j <= l[n].nCount; j++) {
					for (int i = 1; i <= l[n - 1].nCount; i++) {

						inFile >> l[n].nodes[j].w[l[n - 1].nodes[i].nNum];
						printf("\nw[%d][%d][%d]=%5f", i, j, n, l[n].nodes[j].w[l[n - 1].nodes[i].nNum]);
					}
					inFile >> l[n].nodes[j].bias;
					printf("\nb[%d][%d]=%5f", j, n, l[n].nodes[j].bias);
				}
			}

			
			

		}
		inFile.close();


	}

	int lastFileInt(string path) {

		
		int i = 0;
		directory_iterator end_itr;
		for (directory_iterator itr(path); itr != end_itr; ++itr)
		{
			fs::path path1{ itr->path() };
			std::string path_string{ path1.string() };
			i++;
		}



		return(i);
	}


	
};


