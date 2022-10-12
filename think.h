#pragma once


#include <fstream>
#include <filesystem>
#include <sstream>
#include <random>
#include <iostream>

#include "brain.h"
#include "Graph.h"

extern Graph graph1;

#define programm 2			// 1 -training	// 2 -batch training  // 3 -check sample	

#define dataCount 100


#define eTreashold 0.055

#define showShort true


#define bdrawGraph true


#define batchSize 100
#define batchCount 1


#define quickSaveCount 1000


#define TrainDataDir "C:\\Users\\themr\\Desktop\\New folder 2\\VS\\PopkBrok\\traindata\\"

#define stokID "HP\\"



#define CheckDataDir "C:\\Users\\themr\\Desktop\\New folder 2\\VS\\PopkBrok\\CheckData\\"


#define BrainsDir "C:\\Users\\themr\\Desktop\\New folder 2\\VS\\PopkBrok\\brains\\"

#define NNFileNum "0"

#define ChDataNum 3

class think
{
public:

	ifstream inFileNN;

	

	


	

	brain nn;
	struct data
	{
		double input[inpuCount+1];
		double results[outputCount+1];
		double output[outputCount+1];
	};

	data data1;
	double 
		e = 100,
		eMax=100,
		ei[outputCount],
		dEMax=dataCount*outputCount;

	int maxj = 0;

	bool brunning = true;

	double loadedFileDataInP[inpuCount][batchSize],
		loadedFileDataOutp[outputCount][batchSize];


	void run() {

		if (programm == 1) {
			nn.initialize();
			run_1();
		}
		if (programm == 2) {
			nn.initialize();
			run_2();
		}

		if (programm == 3) {
			//nn.initialize();
			nn.loadFromFile((string) BrainsDir+stokID+NNFileNum+".txt");
			run_3();
		}

	}



	void run_1() {

		int itterCount = 0;
		
		if (bdrawGraph) {
			graph1.lines[0].x = 0;
		
		}

		while (abs(eMax) > eTreashold & brunning) {

			e = 0;
			eMax = -1;
			for (int i = 0; i < dataCount; i++) {
				loadData(i, (string)TrainDataDir + stokID);
				nn.thinK(data1.input, data1.output);
				for (int j = 0; j < outputCount; j++) {
					ei[j] = data1.output[j] - data1.results[j];
					e += ei[j] * ei[j];
				}
				if (abs(e) > eMax)
					eMax = abs(e);
				if (!showShort)
					showTr(i);
				nn.backProp(data1.results);
			}
			if (showShort)
				showSh();
			if (bdrawGraph) {
				graph1.drawline(eMax, 0);
				graph1.lines[0].x += 0.5f;
				graph1.lines[0].text.setString(std::to_string(eMax));
				graph1.lines[0].bdraw = true;
			}

			if (itterCount >= quickSaveCount) {
				itterCount = 0;
				nn.quickSave();
			}
			itterCount++;
		}
		nn.saveToFile((string) BrainsDir+stokID );
	
	}

	void run_2() {
	
		int itterCount = 0;
		int nc = 0;
		int itterCount_2 = 0;
		
		int n[dataCount];

		for (int i = 0; i < dataCount; i++) {
			n[i] = i;
		}
		
		/*
		for (int i = 0; i < dataCount - 1; i++) {
			int r = i + (rand() % (dataCount - i));
			int buf = n[i];
			n[i] = n[r];
			n[r] = buf;
		}
		*/

		for (int i = 0; i < dataCount; i++) {
			cout << n[i] << endl;
		}
		printf("\nflag1 ");
	
		if (bdrawGraph) {
			graph1.lines[0].x = 0;
		}
	
	
		while (abs(eMax) > eTreashold*dataCount & brunning) {
		
			e = 0;
			eMax = -1;
			nc = 0;
			
			
			for (int i = 0; i < batchCount; i++) {

				

				for (int k = 0; k< batchSize; k++) {
				
				
				
					loadData(n[nc],(string) TrainDataDir+stokID);
				
					
					
					nn.thinK(data1.input, data1.output);
				//	nn.backProp(data1.results);
				//	nn.thinK(data1.input, data1.output);

					//e = 0;
					for (int j = 0; j < outputCount; j++) {
						ei[j] = data1.output[j] - data1.results[j];
						e += ei[j] * ei[j];
						
					}

					if (abs(e) > eMax) {
						eMax = abs(e);
						maxj = n[nc];
					}

					
					
					nn.batchBackProp(data1.results);
				//	printf("\n\n n[nc]= %d", n[nc]);
					if (!showShort)
						showTr(n[nc]);
					nc++;
				}



				//if (eMax < 0.5* batchSize) {
					//	nn.bpCoeff= bpCoeff0/5;
				//}
				//else {
					//	nn.bpCoeff = bpCoeff0;
				//}

				
				//nn.bpCoeff = bpCoeff0 * (0.99 / (dataCount * (outputCount - eTreashold)) * eMax + (0.01 - 0.99 / (dataCount * (outputCount - eTreashold))));
				//if ((eMax - dEMax != 0) & (dEMax<dataCount*outputCount)) {
				//	nn.bpCoeff = bpCoeff0 * (0.0001 + 1 /pow((abs(eMax - dEMax) + 1),5));
				//	nn.bpCoeff = nn.bpCoeff * (0.99 / (dataCount * (outputCount * 0.25 - eTreashold)) * eMax + (0.01 - 0.99 / (dataCount * (outputCount * 0.25 - eTreashold))));
				//}
				//dEMax = eMax;


				//if (eMax < 35) {
				//		nn.bpCoeff= bpCoeff0*0.0001;
				//}


				nn.batchBackProp_apply(batchSize);

			}
			

			if (showShort)
				showSh();
			if (bdrawGraph) {
				graph1.drawline(eMax, 0);
				graph1.lines[0].x += 0.5f;
				graph1.lines[0].text.setString(std::to_string(eMax));
				graph1.lines[0].bdraw = true;
			}

			if (itterCount >= quickSaveCount) {
				itterCount = 0;
				nn.quickSave();
			}
			itterCount++;
			itterCount_2++;
		}
		nn.saveToFile((string)BrainsDir + stokID);
		printf("\nitterations %d\n", itterCount_2);
	
	
	}

	void run_3() {

		loadData(ChDataNum, CheckDataDir);

		nn.thinK(data1.input, data1.output);

		showTr(0);

	}

	void showSh() {
		printf("\ne = %10f\tj = %f",eMax, nn.bpCoeff);
	}

	void showTr(int num) {

		printf("\n\n%d\n\trise\t\thold\t\tdecline\te = %5f\tmaxj=  %d",num,eMax, nn.bpCoeff);
		
		for (int i = 0; i < outputCount; i+=3) {
			printf("\n%d\t%4f\t%4f\t%4f",i/3+1,data1.output[i], data1.output[i+1], data1.output[i+2]);
			printf("\n%d\t%4f\t%4f\t%4f", i / 3 + 1, data1.results[i], data1.results[i + 1], data1.results[i + 2]);
		}
		
	}

	void loadData(int loadN,string dir) {

		
			//string path = dirPath() + "\\traindata\\";
			//std::stringstream ss;
			//ss << path << loadN << ".txt";
			//std::string str = ss.str();
			//ss.str("");

			inFileNN.open(dir+to_string(loadN)+".txt");

			float xxx;

			for (int i = 0; i < inpuCount; i++) {
				inFileNN >> xxx;
				data1.input[i] = (double)xxx;
			}
			for (int i = 0; i < outputCount; i++) {
				inFileNN >> xxx;
				data1.results[i] = (double)xxx;
				//data1.output[i] = -1;
			}

			inFileNN.close();
		//	printf("nooooooooooooo\n");
		



	}

	string dirPath() {
		/*
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find(L'x64');
		//wstring pos1 = std::wstring(buffer).append(L"\\florist\\trainSamples\\");
		wstring wstr = std::wstring(buffer).substr(0, pos);
		string str(wstr.begin(), wstr.end());;
		*/
		return ("C:\\Users\\themr\\Desktop\\New folder 2\\VS\\PopkBrok");
	}




};





