#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<cmath>
#include<set>
#include<map>
using namespace std;

int numberOfVariables;
struct binaryInt{
    unsigned num;
    unsigned dashes;

    binaryInt(unsigned n=0, unsigned d=0) : num(n), dashes(d) {}
    bool operator<(const binaryInt &b ) const {
        if(num!=b.num){
            return num<b.num;
        }
        return dashes<b.dashes;
    }
    bool operator==(const binaryInt &b) const{
        return num==b.num&&dashes==b.dashes;
    }
    bool covers(binaryInt b){
        for(int i=0; i<20; ++i){
            if((num>>i)&1 && !((b.num>>i)&1)) return false;
            if(!((num>>i)&1) && !((dashes>>i)&1) && ((b.num>>i)&1)) return false;
        }
        return true;
    }
};

binaryInt toBinary(string num) {
    binaryInt res(0, 0);
    for(int i = 0; i < num.length(); ++i) {
        if(num[i] <= '9' && num[i] >= '0') res.num += (num[i] - '0') * pow(10, num.length() - i - 1);
    }
    return res;
}

void takeInput(vector<binaryInt> &Minterms, vector<binaryInt> &DontCares) {
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

set<binaryInt> getPrimeImplicants(vector<binaryInt> minterms, vector<binaryInt> dontCares){
    set<binaryInt>  implicants;
    vector<set<binaryInt>> groups(20);
    for(auto term:minterms){
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

map<binaryInt,string> createPrimeImplicantChart(vector<binaryInt> &minterms,vector<binaryInt> &dontCares){
    map<binaryInt,string> primeImplicantChart;
    set<binaryInt> implicants=getPrimeImplicants(minterms,dontCares);
    for(auto implicant:implicants){
        for(auto term:minterms){
            if(implicant.covers(term))  primeImplicantChart[implicant]+='1';
            else primeImplicantChart[implicant]+='0';
        }
    }
    return primeImplicantChart;
}

set<binaryInt> getEssentialPrimeImplicants(vector<binaryInt> minterms,vector<binaryInt> dontCares){
    map<binaryInt,string> primeImplicantChart=createPrimeImplicantChart(minterms,dontCares);
    set<binaryInt> essentialPrimeImplicants;
    for(int i=0; i<minterms.size(); ++i){
        binaryInt term=minterms[i];
        int cnt=0;
        binaryInt tmp;
        for(auto&[implicant,coverage]:primeImplicantChart){
            if(coverage[i]=='1'){
                cnt++;
                tmp=implicant;
            }
        }
        if(cnt==0) throw runtime_error("Minterm is not covered by any implicant.");
        else if(cnt==1) essentialPrimeImplicants.insert(tmp);
    }
    return essentialPrimeImplicants;
}



int popcount(string s) {
    int res = 0;
    for(auto i : s) if(i == '1') ++res;
    return res;
}

// struct comp {
//     bool operator()(const pair<binaryInt, string> &a, const pair<binaryInt, string> &b) {
//         if(b.second != a.second) return popcount(a.second) > popcount(b.second);
//         else return b.first < a.first;
//     }
// };

void deleteCol(map<binaryInt, string> &m, int i) {
    for(auto &[implicant, s] : m) {
        s.erase(i, 1);
    }
}

string toBooleanExpression(const binaryInt &b) {
    string res = "";
    int len1 = 32 - __builtin_clz(b.num);
    int len2 = 32 - __builtin_clz(b.dashes);
    int len = max(len1, len2);

    for(int i = 0; i < len; ++i) {
        if(b.dashes && (1<<i)) continue;
        res += 'A' + i;
        if(!(b.num & (1<<i))) res += '\'';
    }
    return res;
}

string generateExpression(vector<binaryInt> minterms,vector<binaryInt> dontCares) {
    map<binaryInt,string> m=createPrimeImplicantChart(minterms,dontCares);
    set<binaryInt> essentials=getEssentialPrimeImplicants(minterms,dontCares);
    vector<binaryInt> essential(essentials.begin(),essentials.end());
    vector<binaryInt> final = essential;
    for(auto i : essential) {
        string s = m[i];
        int cnt = 0;
        for(int j = 0; j < s.length(); ++j) {
            if(s[j] == '1') {
                deleteCol(m, j - cnt); ++cnt;
            }
        }
        m.erase(i);
    }

    while(!m.empty() && !m.begin()->second.empty()) {
        string s = m.begin()->second;
        int cnt = 0;
        for(int i = 0; i < s.length(); ++i) {
            if(s[i] == '1') {
                deleteCol(m, i - cnt); ++cnt;
            }
        }
        final.push_back(m.begin()->first);
        m.erase(m.begin()->first);
    }

    string res = "";
    for(auto i : final) {
        res += toBooleanExpression(i);
    }
    return res;
}

int main() {
    // freopen("test.txt", "r", stdin);
    vector<binaryInt> minTerms, dontCares;
    takeInput(minTerms, dontCares);
    cout << generateExpression(minTerms,dontCares) << endl;
}