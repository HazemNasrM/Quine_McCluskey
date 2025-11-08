#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<cmath>
#include<set>
using namespace std;

struct binaryInt{
    unsigned num;
    unsigned dashes;

    binaryInt(unsigned n, unsigned d) : num(n), dashes(d) {}
};

int numberOfVariables;
vector<binaryInt> Minterms;
vector<binaryInt> DontCares;

binaryInt toBinary(string num) {
    binaryInt res(0, 0);
    for(int i = 0; i < num.length(); ++i) {
        if(num[i] <= '9' && num[i] >= '0') res.num += (num[i] - '0') * pow(10, num.length() - i - 1);
    }
    return res;
}

void takeInput() {
    cin >> numberOfVariables;
    string temp;
    getline(cin, temp);
    string minterms;
    string dontCares;
    getline(cin, minterms);
    getline(cin, dontCares);
    if(minterms.empty() || dontCares.empty() || (minterms[0] != 'm' && minterms[0] != 'M') 
                        || dontCares[0] != 'd') throw runtime_error("Invalid Input");

    set<unsigned> MintermSet;
    set<unsigned> DontCareSet;
    stringstream ss(minterms);
    string minterm;
    while(getline(ss, minterm, ',')) {
        MintermSet.insert(toBinary(minterm).num);
    }

    stringstream SS(dontCares);
    string dontCare;
    while(getline(SS, dontCare, ',')) {
        binaryInt b = toBinary(dontCare);
        DontCares.push_back(b);
        DontCareSet.insert(b.num);
    }

    if(minterms[0] == 'm') {
        for(unsigned i : MintermSet) {
            Minterms.push_back(binaryInt(i,0));
        }
    }
    else {
        for(unsigned i = 0; i < (1<<(numberOfVariables-1)); ++i) {
            if(!MintermSet.count(i) && !DontCareSet.count(i)) {
                Minterms.push_back(binaryInt(i,0));
            }
        }
    }
}

int main() {
    
}