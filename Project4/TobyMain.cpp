#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "gnuplot_i.hpp"
#include <algorithm>
#include <conio.h>
#include <windows.h> //for Sleep()
#define SLEEP_LGTH 2  // sleep time in seconds
#define NPOINTS    1000 // length of array
#include "time.h"
#include <cstdlib>
#include <cmath>
#include <list>

using namespace std;

class HashTable;
void wait_for_key();
void buildVector(ifstream & input,vector<int> &temp);//takes the input from a file and puts it in a vector;
void task1(vector<int> total, vector<int> value, vector<int> weight);
void task2a(vector<int> tol, vector<int> val, vector<int> wei);//uses greedy to find the best set
void task2b(vector<int> tol, vector<int> val, vector<int> wei);//uses the max heap to find best greedy set
void heapInsert(vector< pair <float,int>> &arry, float val,int index);//insert into heap vector
void heapDelMax(vector<pair <float,int>> &arry);//sorts the array useing the delete max algo
int memFunction(int i, int j, vector<int> value, vector<int> weight, HashTable & H, int n, int w);
int computeKey(int i, int j, int n, int w);
void task3();//displays the graphs real time
//globle vars
vector<pair<float, int>> timeTotalsA;//task 2a
vector<pair<float,int>> timeTotalsB;//task 2b

vector<pair<float, int>> timeTotalsC;//task 1a
vector<pair<float, int>> timeTotalsD;//task 2b





//am going to use node struct as the type of list to store i, j, value and hashed value
class HashTable {
private:
    struct node {
      int unique;
      int value;
      node(){
        unique = -1;
        value =-1;
      }
      node(int l, int val){
        unique = l;
        value = val;
      }


    };
public:
  list<node> *table;
  int k; //# of slots in hashTable

  HashTable(int slots){
    this->k = slots;
    this->table = new list<node>[this->k];
  }

  int hashKey(int key){
    return key % this->k;
  }

  bool getKeyInTable(int key, int & keyValue){
      int index = hashKey(key);

      // find the key in (i)th list
      list <node>::iterator i;
      for (i = table[index].begin(); i != table[index].end(); i++) {
        if (i->unique == key){
					// cout << "node instance found :"<< i->value << endl;
          keyValue = i->value;
          return true;
        }
      }
      return false;
  }

  void insertNode(int key, int val){
    int slot = hashKey(key);
    node n;
    n.unique = key;
    n.value = val;
		// cout << "node:" << n.value << "  " << n.unique << endl;
    table[slot].push_back(n);
  }


};




int main(int argc, char* argv[]) {
	int choice;
	vector<int> total;
	vector<int> value;
	vector<int> weight;
	vector<string> new_argv;
	int noInputTripper = 0;//checks if no input (for VS)
	if (argc != 4) {
		cout << "Incorrect input. Correct format: ./<exectuable.out> <total.txt> <value.txt> <weight.txt> \n";
		//fix if possible

		new_argv.push_back("");//for const sake
		cout << "please enter count file:";
		new_argv.push_back("");//for const sake
		cin >> new_argv[1];//c
		cout << "please enter value file: ";
		new_argv.push_back("");//for const sake
		cin >> new_argv[2];//v
		cout << "please enter weight file: ";
		new_argv.push_back("");//for const sake
		cin >> new_argv[3];//w
		cout << endl;
		noInputTripper = 1;
	}

	if (noInputTripper == 1)//user input vs command line
	{
		ifstream inputX(new_argv[1]);//reading file one getting the total sack
		if (inputX.is_open()) {
			buildVector(inputX, total);//build the total vector
			inputX.close();
		}
		ifstream inputY(new_argv[2]);//reading the second file for values
		if (inputY.is_open()) {
			buildVector(inputY, value);//build the values vector
			inputY.close();
		}
		ifstream inputZ(new_argv[3]);//this file has the weight values
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
				cout << "Options: (1) display vectors, (2) quit, (3)Run 1 and 2 (4) Run 3 :";
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
				else if (choice ==3)
					{
						cout << endl << "Knapsack capacity = " << total[0];
						cout << ". Total number of items = " << value.size() << endl << endl;
						task1(total, value, weight);
						task2a(total, value, weight);
						task2b(total, value, weight);
					}
					else if(choice ==4){
						cout << "dont have the task 3 done" << endl;
						task3();
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

void task1(vector<int> total, vector<int> value, vector<int> weight)
{/*
	const size_t totalWeight = total[0];
	const size_t valueSize = value.size();

	//Traditional Dynamic Programming
	double totalTime, finishTime, startTime = clock();//timers
	int currentValue= 0;

	//construct a 2d array of size (value.size() +1, totalWeight + 1)
	int table [valueSize + 1][totalWeight + 1];//my complier hates non const vars!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	for (int i=0; i< value.size() + 1; i++ ){
		for (int j=0; j < totalWeight + 1; j++){
					//F(i,j)= (if j−w i≥0: max{F(i−1,j),v[i]+F(i−1,j−w[i])},
			 				// {if j−w[i] i<0: F(i−1,j)})

				//base case: (i or j) = 0; => we assign 0 b/c its no items or 0 weight
				if(i ==0 || j == 0){
					table[i][j] = 0;
				}else if(weight[i-1] <= j){ //if the item we are looking at is small enough to add/replace in subset
					currentValue = table[i-1][j - weight[i-1]]+ value[i-1];
					if(currentValue > table[i-1][j]){
						table[i][j]= currentValue;
					}else{
						table[i][j] = table[i-1][j];
					}
				}else{ //if the new possible item is too heavy to add.
						table[i][j] = table[i-1][j];
				}
		}
	}

	// for (int i=0; i< value.size() + 1; i++ ){
	// 	for (int j=0; j < totalWeight + 1; j++){
	// 		cout << table[i][j] << "  ";
	// 	}
	// 	cout << endl;
	// }

	int maxValue = table[value.size()][totalWeight];
	//backtrace w/ while loop  and insert into optimalSet
	// int tempSize = value.size();
	int tempWeight = totalWeight;
	 //Starting with: F(tempSize, tempWeight);
	vector<int> optimalSetA;
	// w = W;
  for (int i = value.size(); i > 0 && maxValue > 0; i--) {
        if (maxValue == table[i - 1][tempWeight])
            continue;
        else {

            // This item is included -> add it & make adjustments!
						optimalSetA.push_back(i-1);
            maxValue -= value[i - 1];
            tempWeight -= weight[i - 1];
        }
    }

	finishTime = clock();
	totalTime = (double)(finishTime - startTime)/CLOCKS_PER_SEC;
	timeTotalsC.push_back(make_pair(totalTime,//please put k here//));//load globle---------------------------------!!!!!!!_----------------------
		//optimal subset value:
		cout << "Traditional Dynamic Programming Optimal value: " << table[value.size()][totalWeight] << endl;
		//optimal subset
		cout << "Traditional Dynamic Programming Optimal subset: " <<  "{";

		reverse(optimalSetA.begin(), optimalSetA.end());

		for (int l = 0; l < optimalSetA.size(); l++){
			cout << optimalSetA[l] +1;
			if(l != optimalSetA.size() - 1 ){
				cout << ", ";
			}
		}
		cout << "}" << endl;

		//time taken for computation
		cout << "Traditional Dynamic Programming Time Taken: " << totalTime << endl << endl;

	//Space-efficient Dynamic Programming
	int newTotalB = 0;
	startTime = clock();//timers
	//totalWeight

	int k = (int)ceil(sqrt((double)(value.size() *total[0]/2))); //nasty!
	// cout << "size of hash table: " << k <<  endl;
	HashTable h = HashTable(k);

	newTotalB = memFunction(value.size(), total[0], value, weight, h, value.size(), total[0]);

	int key = 0;
	int existsValue = 0;
	vector<int> optimalSetB;
	// w = W;
	maxValue = newTotalB;
	tempWeight = total[0];
  for (int i = value.size(); i > 0 && maxValue > 0; i--) {


				key = computeKey(i-1, tempWeight, value.size(), total[0]);
				h.getKeyInTable(key, existsValue);
				// cout << "maxValue found: " << maxValue << " tempWeight: " << tempWeight << endl;

				if(i == 1 && weight[i-1] <= tempWeight){ //weird edge case i couldn't cover for some reason
					// cout << "just found index: " << i-1 << endl << endl;
					optimalSetB.push_back(i-1);
					maxValue -= value[i - 1];
					tempWeight -= weight[i - 1];
				}else if (maxValue == existsValue){
            continue;
        }else {
            // This item is included -> add it & make adjustments!
						// cout << "just found index: " << i-1 << endl << endl;
						optimalSetB.push_back(i-1);
            maxValue -= value[i - 1];
            tempWeight -= weight[i - 1];
        }

				existsValue = -1;
    }

	finishTime = clock();
	totalTime = (double)(finishTime - startTime)/CLOCKS_PER_SEC;
	timeTotalsD.push_back(make_pair(totalTime,//please put k here//));//load globle---------------------------------!!!!!!!_----------------------


	cout << "Space-efficient Dynamic Programming optimal subset value: " << newTotalB << endl;
	cout << "Space-efficient Dynamic Programming optimal subset: {";


	reverse(optimalSetB.begin(), optimalSetB.end());

	for (int l = 0; l < optimalSetB.size(); l++){
		cout << optimalSetB[l] +1;
		if(l != optimalSetB.size() - 1 ){
			cout << ", ";
		}
	}
	cout << "}" << endl;


	cout << "Space-efficient Dynamic Programming time taken: " << totalTime << endl << endl;

	return;*/
}
int computeKey(int i, int j, int n, int w)
{
	// cout << "made it into computeKey; i:"  << i << " j:" << j << " n:" << n << " w:" << w << endl;
	//number of bits in n
	int Bn = (int)ceil(log2((double)(n+1)));
	//number of bits in w
	int Bw = (int)ceil(log2((double)(w+1)));
	// cout << "Bn: " << Bn << "  Bw: " << Bw << endl;
	//number of bits in i
	int Bi = (int)ceil(log2((double)(i+1)));
	//number of bits in j
	int Bj = (int)ceil(log2((double)(j+1)));
	// cout << "Bi: " << Bi << "  Bj: " << Bj << endl;


	int value = 1 <<  (Bn + Bw); //value: 1<size of N + size of W>
	int result = i << Bw; //result  <i><size of w>
	result = result|value; // result: <i><j>
	result = result|j; //result: 1<i><j>
	return result;

}

void task3()//print out graphs
{
	string filesNames[9][3] = {
	{"p00_c.txt", "p00_v.txt", "p00_w.txt"},
	{"p01_c.txt", "p01_v.txt", "p01_w.txt"},
	{"p02_c.txt", "p02_v.txt", "p02_w.txt"},
	{"p03_c.txt", "p03_v.txt", "p03_w.txt"},
	{"p04_c.txt", "p04_v.txt", "p04_w.txt"},
	{"p05_c.txt", "p05_v.txt", "p05_w.txt"},
	{"p06_c.txt", "p06_v.txt", "p06_w.txt"},
	{"p07_c.txt", "p07_v.txt", "p07_w.txt"},
	{"p08_c.txt", "p08_v.txt", "p08_w.txt"}
	};
	vector<int> value;
	vector<int> weight;
	vector<int> total;
	vector<float> x1;
	vector<int> y1;
	vector<float> x2;
	vector<int> y2;
	for (int i = 0; i < 9; i++)
	{
		ifstream input(filesNames[i][0]);//reading file one getting the total sack
		if (input.is_open()) {
			buildVector(input, total);//build the total vector
			input.close();
		}
		ifstream input2(filesNames[i][1]);//reading the second file for values
		if (input2.is_open()) {
			buildVector(input2, value);//build the values vector
			input2.close();
		}
		ifstream input3(filesNames[i][2]);//this file has the weight values
		if (input3.is_open()) {
			buildVector(input3, weight);//build the weight vector
			input3.close();
		}
		//run tasks and get in put task 2
		cout << endl << "Knapsack capacity = " << total[0];
		cout << ". Total number of items = " << value.size() << endl << endl;
		task1(total, value, weight);
		task2a(total, value, weight);
		task2b(total, value, weight);
		//clear values
		total.clear();
		value.clear();
		weight.clear();

	}
	Gnuplot g1a("task 2");
	Gnuplot g1b("task 1");
	for (int i = 0; i < timeTotalsA.size(); i++)
	{
		x1.push_back(timeTotalsA[i].first);
		y1.push_back(timeTotalsA[i].second);

	}
	for (int j = 0; j < timeTotalsB.size(); j++)
	{
		x2.push_back(timeTotalsB[j].first);
		y2.push_back(timeTotalsB[j].second);

	}
	g1a.set_style("lines").plot_xy(x1, y1, "task 2a").plot_xy(x2,y2,"task 2b");
	//system("pause");
	wait_for_key();
	x1.clear();
	x2.clear();
	y1.clear();
	y2.clear();
	//--------------------------------------------uncomment below once task 1 works-----------------------------
	/*for (int i = 0; i < timeTotalsC.size(); i++)
	{
		x1.push_back(timeTotalsC[i].first);
		y1.push_back(timeTotalsC[i].second);

	}
	for (int j = 0; j < timeTotalsD.size(); j++)
	{
		x2.push_back(timeTotalsD[j].first);
		y2.push_back(timeTotalsD[j].second);

	}
	g1a.set_style("lines").plot_xy(x1, y1, "task 1a").plot_xy(x2, y2, "task 1b");
	//system("pause");
	wait_for_key();*/
	//----------------------------------------^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^------------------------------------
}


int memFunction(int i, int j, vector<int> value, vector<int> weight, HashTable & H, int n, int w){
	// cout << "made it into memFunction with i :" << i << " j:" << j << endl;
	int upper, replace;
	int existsValue = -1;

	int key = computeKey(i, j, n,w);

	if(i==0 || j == 0){
		// cout << "hit base case, i or j == 0" << endl;
		return 0;
	}else if(H.getKeyInTable(key, existsValue)) {
		// cout << "key already existed." << endl;
		return existsValue;
	}else if(j >= weight[i-1]){//we know at this point we have a value that is 0 or doesn't exist in the hash table

		//if we go here then we can potentially grab a item i
		// upperKey = computeKey(i-1, j);
		// replaceKey = computeKey(i-1, j - weight[i-1]);
		upper = memFunction(i-1, j, value, weight, H, n, w);
		replace = value[i-1] + memFunction(i-1, j-weight[i-1], value, weight, H, n, w);


		if(upper < replace){
			H.insertNode(key, replace);
			return replace;
		}else{
			H.insertNode(key, upper);
			return upper;
		}

	}else{ //item i is too big to be in our sub set so we only look up one cell
				upper = memFunction(i-1,j, value, weight, H, n, w);
				//push upper's value into hash table
				H.insertNode(key, upper);
				return upper;
	}
}


void task2a(vector<int> tol, vector<int> val, vector<int> wei)
{
	//sort arrays then print out
	vector<pair <float, int>> bestVal;//hold the ratio for best weight to value first(val),second(wei) by subset
	int newTotal = 0;
	double totalTime, finishTime, startTime = clock();//timers


	for (int i = 0; i < val.size(); i++)
	{
		bestVal.push_back(make_pair(float(val[i]) / wei[i], i + 1));
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
		cout << displayArry[i] << " ";
	}
	//newTotal = newTotal - wei[bestVal[counter].second - 1];//add to total counter
	cout << "}" << endl;
	cout << "Greedy Approach Optimal value: " << totalValue << endl;
	//timers
	finishTime = clock();
	totalTime = (double)(finishTime - startTime) / CLOCKS_PER_SEC;
	timeTotalsA.push_back(make_pair(totalTime,timeTotalsA.size()));//load globle---------------------------------!!!!!!!_----------------------
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

	int counter = bestVal.size() - 1;
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
	timeTotalsB.push_back(make_pair(totalTime,timeTotalsB.size()));//load globle---------------------------------!!!!!!!_----------------------
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

void wait_for_key()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
	cout << endl << "Press any key to continue..." << endl;

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	_getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
	cout << endl << "Press ENTER to continue..." << endl;

	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
#endif
	return;
}
