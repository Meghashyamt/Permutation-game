

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int getMex(unordered_set<int> gValSet)
{
	int mex = 0;
	while(true)
	{
		if(gValSet.find(mex) == gValSet.end())
			return mex;
		else
			++mex;
	}
}

typedef vector<int>::const_iterator iter;
bool sorted(iter begin, iter end)
{
	for(iter cur = begin+1, prev = begin;
		cur != end; ++cur, ++prev)
		if(*cur < *prev)
			return false;
	return true;
}

void printVec(vector<int> v)
{
	cout << "( ";
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, ","));
	cout << " )" << endl;
}

int getGrundyIndex(int currentIndex, int removeNum)
{
	return currentIndex | (1 << (removeNum-1));
}

int spragueGrundy(
	const vector<int> &permutation, 
	int grundyFunctionIndex, // sum of the permutation
	unordered_map<int, int> &grundyVals
	)
{
	if(permutation.size() == 2)
	{
		if(permutation[0] < permutation[1])
		{
			grundyVals[grundyFunctionIndex] = 0;
			return 0;
		}
		else
		{
			grundyVals[grundyFunctionIndex] = 1;
			return 1;
		}
	}

	if(permutation.size() == 1 || 
	   sorted(permutation.begin(), permutation.end()))
	{
		grundyVals[grundyFunctionIndex]  = 0;
		return 0;
	}
//	printVec (permutation);
//	cout << grundyFunctionIndex << endl;

	int gVal = -1;
	int index = 0;
	unordered_set<int> myGrundyVals;
	for(auto num : permutation)
	{
		int newGrundyIndex = getGrundyIndex(grundyFunctionIndex, num);
		if(grundyVals.find(newGrundyIndex) == grundyVals.end() )
		{
			vector<int> nextPerm;
			int sum = grundyFunctionIndex - num;
			copy(permutation.begin(), permutation.begin() + index, 
				 back_inserter(nextPerm));
			copy(permutation.begin()+index+1, permutation.end(), 
				 back_inserter(nextPerm));
			spragueGrundy(nextPerm, newGrundyIndex, grundyVals);
		}
		myGrundyVals.insert(grundyVals[newGrundyIndex]);
		index++;	
	}
	grundyVals[grundyFunctionIndex] = gVal = getMex(myGrundyVals);
	return gVal;
}

int permutationGame(istream &instream)
{
//	ifstream instream;
//	instream.open(argv[1], ifstream::in);

	int nTests;
	instream >> nTests;

	for(int test = 0; test < nTests; ++test)
	{
		int N;
		instream >> N;
		vector<int> permutation(N);

		for(int index = 0; index < N; ++index)
		{
			int val;
			instream >> val;
			permutation[index] = val;
		}
		// output the winner

		unordered_map<int,int> grundyVals;
		if(spragueGrundy(permutation, 1<<N, grundyVals) == 0)
			cout << "Bob" << endl;
		else
			cout << "Alice" << endl;

		permutation.clear();
		grundyVals.clear();
	}
	return 0;
}

int main(int argc, char *argv[])
{
	return permutationGame(cin);
}