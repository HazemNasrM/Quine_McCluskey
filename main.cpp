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

    binaryInt(unsigned n=0, unsigned d=0) : num(n), dashes(d) {}
    bool operator<(const binaryInt&other)const{
        if(num!=other.num){
            return num<other.num;
        }
        return dashes<other.dashes;
    }
};

bool are1BitOff(binaryInt a,binaryInt b){
    if(a.dashes!=b.dashes) return false;
    return __builtin_popcount(a.num^b.num)==1;
}
binaryInt combine(binaryInt a,binaryInt b){
    if(!are1BitOff(a,b)) throw runtime_error("Not 1 bit off");
    binaryInt c;
    c.dashes=a.dashes|(a.num^b.num);
    c.num=a.num&b.num;
    return c;
}
bool nextColumn(vector<set<binaryInt>> &groups,set<binaryInt> &implicants){
    bool modified=false;
    vector<set<binaryInt>> newGroups(20);
    for(int i=0; i<19; ++i){
        for(auto&a:groups[i]){
            for(auto&b:groups[i+1]){
                if(are1BitOff(a,b)){
                    binaryInt c=combine(a,b);
                    newGroups[i].insert(c);
                    if(implicants.find(a)!=implicants.end()) implicants.erase(a);
                    if(implicants.find(b)!=implicants.end()) implicants.erase(b);
                    implicants.insert(c);
                    modified=true;
                }
            }
        }
    }
    groups=newGroups;
    return modified;
}
set<binaryInt> getPrimeImplicants(vector<binaryInt> minTerms,vector<binaryInt> dontCares){
    set<binaryInt>  implicants;
    vector<set<binaryInt>> groups(20);
    for(auto term:minTerms){
        implicants.insert(term);
        groups[__builtin_popcount(term.num)].insert(term);
    }
    for(auto&term:dontCares){
        implicants.insert(term);
        groups[__builtin_popcount(term.num)].insert(term);
    }
    bool b=nextColumn(groups,implicants);
    while(b){
        b=nextColumn(groups,implicants);
    }
    return implicants;
} 
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