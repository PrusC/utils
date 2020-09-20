#include <iostream>
#include "src/arithmetic.h"

using namespace std;
using namespace arithmetic;


int main()
{

    string s = "5 + 8/2";

    auto node = Parse(s.begin(), s.end(), 0);


    cout << node->Evaluate() << endl;

    return 0;
}
