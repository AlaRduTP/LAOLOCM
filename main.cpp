#include <vector>
#include <string>
#include <iostream>

#include "fitness.h"

using namespace std;

int main() {
	vector<string> genes;
	genes.push_back("(#1+((#3*#5)+3))");
	genes.push_back("(#2+((#4*#6)+1))");
	genes.push_back("(#9+((#8*#7)+5))");
	cout << fitness(genes) << endl;
}
