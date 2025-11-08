#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct binaryInt{
    unsigned num;
    unsigned dashes;

    binaryInt(unsigned n, unsigned d) : num(n), dashes(d) {}
};

int numberOfVariables;
string minterms;
string dontCares;
vector<binaryInt> Minterms;
vector<binaryInt> DontCares;

binaryInt toBinary(string num) {
    
    binaryInt res(0, 0);
    for(int i = 0; i < num.length(); ++i) {
        if(num[i] == '1') res.num += (1<<(num.length() - i - 1));
        if(num[i] == '-') res.dashes += (1<<(num.length() - i - 1));
    }
    return res;
}

void takeInput() {
    cin >> numberOfVariables;
    getline(cin, minterms);
    getline(cin, dontCares);

    if(minterms.empty() || dontCares.empty() || (minterms[0] != 'm' && minterms[0] != 'M') 
                        || dontCares[0] != 'd') throw runtime_error("Invalid Input");

    if(minterms[0] == 'm') {

    }
    else {
        
    }

    
}

int main() {

}