#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "time.h"
#include <cstdlib>
//note if python is acting up uncomment ms_no_coredell
//#define MS_NO_COREDLL

#include <python.h>

using namespace std;

void buildVector(ifstream & input,vector<int> &temp);//takes the input from a file and puts it in a vector;
void task2a(vector<int> tol, vector<int> val, vector<int> wei);//uses greedy to find the best set 
void task2b(vector<int> tol, vector<int> val, vector<int> wei);//uses the max heap to find best greedy set
void heapInsert(vector< pair <float,int>> &arry, float val,int index);//insert into heap vector
void heapDelMax(vector<pair <float,int>> &arry);//sorts the array useing the delete max algo
void task2Plot();//plots all files

int main(int argc, char* argv[]) {
	int choice;
	vector<int> total;
	vector<int> value;
	vector<int> weight;
	vector<string> new_argv;
	int noInputTripper = 0;//checks if no input (for VS)
	if (argc != 3) {
		cout << "Incorrect input. Correct format: ./<exectuable.out> <total.txt> <value.txt> <weight.txt> \n";
		//fix if possible
		
		new_argv.push_back("");//for const sake
		cout << "please enter (c) or total sack size: ";
		new_argv.push_back("");//for const sake
		cin >> new_argv[1];//c
		cout << endl << "please enter (v) or values: ";
		new_argv.push_back("");//for const sake
		cin >> new_argv[2];//v
		cout << endl << "please enter (w) or weight: ";
		new_argv.push_back("");//for const sake
		cin >> new_argv[3];//w
		cout << endl;
		noInputTripper = 1;
	}

	if (noInputTripper == 1)//user input vs command line
	{
		ifstream inputX("p07_c.txt");//reading file one getting the total sack
		if (inputX.is_open()) {
			buildVector(inputX, total);//build the total vector
			inputX.close();
		}
		ifstream inputY("p07_v.txt");//reading the second file for values
		if (inputY.is_open()) {
			buildVector(inputY, value);//build the values vector
			inputY.close();
		}
		ifstream inputZ("p07_w.txt");//this file has the weight values
		if (inputZ.is_open()) {
			buildVector(inputZ, weight);//build the weight vector
			inputZ.close();
		}
	}
	else
	{
		ifstream input(argv[1]);//reading file one getting the total sack
		if (input.is_open()) {
			buildVector(input, total);//build the total vector
			input.close();
		}
		ifstream input2(argv[2]);//reading the second file for values
		if (input2.is_open()) {
			buildVector(input2, value);//build the values vector
			input2.close();
		}
		ifstream input3(argv[3]);//this file has the weight values
		if (input3.is_open()) {
			buildVector(input3, weight);//build the weight vector
			input3.close();
		}

	}

		
			while (1) {
				choice = 0;
				cout << "Options: (1) display vectors, (2) quit, (3)????, (4) task 2a, (5)task 2b, (6)task 3 plots\n";
				cin >> choice;

				//Print vectors for testing
				if (choice == 1)
				{
					cout << "total vector: ";
					for (int x = 0; x < total.size(); x++)
					{
						cout << total[x] << " ";
					}
					cout << endl <<"value vector: ";
					for (int x = 0; x < value.size(); x++)
					{
						cout << value[x] << " ";
					}
					cout << endl << "weight vector: ";
					for (int x = 0; x < weight.size(); x++)
					{
						cout << weight[x] << " ";
					}
					cout << endl;
				}
				//Quit	
				else if (choice ==2)//quit
					break;
				else if (choice == 4)//part 2a
				{
					task2a(total, value, weight);
				}
				else if (choice == 5)//part 2b
				{
					task2b(total, value, weight);
				}
				else if (choice == 6)//need to plot 
				{
					//plot task 2 for all cases
					task2Plot();

				}

			}
	return 0;
}

void buildVector(ifstream & input, vector<int> &temp)
{
	int line = 1, numWords = 0;
	stringstream tempWord;
	//double totalTime, finishTime, startTime = clock();
	while (!input.eof()) {
		string tempLine, tempWord;

		//Read a whole line of text from the file
		getline(input, tempLine);
		for (int i = 0; i < tempLine.length(); i++) {
			//Insert valid chars into tempWord until a delimiter( newline or space) is found
			while (tempLine[i] != ' '&& tempLine[i] != '\n' && i < tempLine.length()) {
				tempWord.insert(tempWord.end(), tempLine[i]);
				i++;
			}

			//Trim any punctuation off end of word. Will leave things like apostrophes
			//and decimal points
			while (tempWord.length() > 0 && !isalnum(tempWord[tempWord.length() - 1]))
				tempWord.resize(tempWord.size() - 1);

			if (tempWord.length() > 0)
			{
				//Once word is formatted,call insert with the word, the line of the input
				//file it came from, the root of our tree, and the distinct word counter 
				temp.push_back(stoi(tempWord));
				//Clear out tempWord so we can use it again
				tempWord.clear();
			}
		}
	
	}
	
}

void task2a(vector<int> tol, vector<int> val, vector<int> wei)
{
	//sort arrays then print out
	vector<pair <float,int>> bestVal;//hold the ratio for best weight to value first(val),second(wei) by subset
	int newTotal = 0;
	double totalTime, finishTime, startTime = clock();//timers
	

	for (int i = 0; i < val.size(); i++)
	{
		bestVal.push_back(make_pair (float (val[i]) / wei[i], i + 1));
	}

	//sort the array
	sort(bestVal.begin(), bestVal.end());
	int counter = bestVal.size() - 1;
	//display
	
	//
	int totalValue = 0;
	vector<int> displayArry;
	while (1)//carefull
	{
		newTotal = newTotal + wei[bestVal[counter].second - 1];//add to total counter
		
		if (tol[0] < newTotal)//break clause
		{
			break;
		}
		totalValue = totalValue + val[bestVal[counter].second - 1];
		//cout << bestVal[counter].second << " ";//display 
		displayArry.push_back(bestVal[counter].second);
		counter--;
		
	}
	sort(displayArry.begin(), displayArry.end());
	cout << "Greedy Approach Optimal subset: { ";
	for (int i = 0; i < displayArry.size(); i++)
	{
		cout << displayArry[i]<<" ";
	}
	//newTotal = newTotal - wei[bestVal[counter].second - 1];//add to total counter
	cout << "}" << endl;
	cout << "Greedy Approach Optimal value: " << totalValue << endl;
	//timers
	finishTime = clock();
	totalTime = (double)(finishTime - startTime) / CLOCKS_PER_SEC;
	cout << "Greedy Approach Time Taken: " << totalTime;
	cout << endl << endl;

}

void task2b(vector<int> tol, vector<int> val, vector<int> wei)
{
	//sort arrays then print out
	vector<pair <float, int>> bestVal;//hold the ratio for best weight to value first(val),second(wei) by subset
	int newTotal = 0;
	double totalTime, finishTime, startTime = clock();//timers


	for (int i = 0; i < val.size(); i++)// holds the heap needs to heapify
	{
		//bestVal.push_back(make_pair(val[i] / wei[i], i + 1));
		heapInsert(bestVal, float(val[i]) / wei[i], i + 1);
	}
	//now use delete to sort the array
	heapDelMax(bestVal);
	//arry now sorted backwards

	int counter = bestVal.size()-1;
	//display
	cout << "Heap based Greedy Approach Optimal subset: ";
	//
	int totalValue = 0;
	vector<int> displayArry1;
	while (1)//carefull
	{
		newTotal = newTotal + wei[bestVal[counter].second - 1];//add to total counter
		if (tol[0] < newTotal)//break clause
		{
			break;
		}
		else//
		{
			totalValue = totalValue + val[bestVal[counter].second - 1];
			//cout << bestVal[counter].second << " ";//display 
			displayArry1.push_back(bestVal[counter].second);
			counter--;
		}
	}
	//newTotal = newTotal - wei[bestVal[counter].second - 1];//add to total counter
	cout << "{ ";
	sort(displayArry1.begin(), displayArry1.end());
	for (int i = 0; i < displayArry1.size(); i++)
	{
		cout << displayArry1[i] << " ";
	}
	cout << " }" << endl;
	cout << "Heap based Greedy Approach Optimal value: " << totalValue << endl;
	//timers
	finishTime = clock();
	totalTime = (double)(finishTime - startTime) / CLOCKS_PER_SEC;
	cout << "Heap based Greedy Approach Time Taken: " << totalTime;
	cout << endl << endl;


}

void heapInsert(vector<pair <float,int>>& arry, float val,int index)
{
	arry.push_back(make_pair(val, index));
	for (int m = 0; m < arry.size(); m++)
	{
		if (arry.size() > 2 * m + 1)
		{
			if (arry[0].first < (arry[2 * m + 1].first))
			{
				swap(arry[0], arry[2 * m + 1]);//left
			}
		}
		if (arry.size() > 2 * m + 2)
		{
			if ((arry[0].first < (arry[2 * m + 2].first)))
			{
				swap(arry[0], arry[2 * m + 2]);//right
			}
		}
		else//nothing
		{

		}
	}
}

void heapDelMax(vector<pair <float,int>>& arry)
{
	
	int arrySize = arry.size()-1;//store old size
	while (arrySize != 0)
	{
		swap(arry[0], arry[arrySize]);//swap to last in array

		arrySize--;//dec the array
		//now re sort/heap
		if (arrySize == 0)
			break;
		for (int m = 0; m < arrySize; m++)
		{
			if (arrySize > 2 * m + 1)
			{
				if (arry[0].first < (arry[2 * m + 1].first))
				{
					swap(arry[0], arry[2 * m + 1]);//left
				}
			}
			if (arrySize > 2 * m + 2)
			{
				if ((arry[0].first < (arry[2 * m + 2].first)))
				{
					swap(arry[0], arry[2 * m + 2]);//right
				}
			}
			else//nothing
			{

			}
		}
	}

}

void task2Plot()//plots all files
{
	PyObject* graph;
	//Py_Initialize();//start

	//FILE *fd = fopen("pythonTest.py", "r");
	//PyRun_SimpleFileEx(fd, "pythonTest.py", 1);
	//Py
	//Py_Finalize();//finish
	//BMP graph1;//create image 
	//graph1.ReadFromFile("graph1.txt");
	//graph1.SetSize(640, 480);//set size
	//graph1.SetBitDepth(8);//sets bit depth
	//graph1.WriteToFile("graph1.txt");
}
