#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<cmath>
#include<set>
#include<map>
#include<stdexcept>
#include<cstdio>
#include<bitset>
#define endl "\n" // to save time on flushing the output stream
using namespace std;

int numberOfVariables;
bool display = false;
//this struct is meant to represent integers with dashes
struct binaryInt{
    unsigned num;       //the ones in the represented number match the ones in `num`
    unsigned dashes;    //the dashes in the represented number match the ones in `dashes`

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
    bool covers(binaryInt b) const{
        for(int i=0; i < numberOfVariables; ++i){
            if((num>>i)&1 && !((b.num>>i)&1)) return false;
            if(!((num>>i)&1) && !((dashes>>i)&1) && ((b.num>>i)&1)) return false;
        }
        return true;
    }
};

/* ============================== parsing and storing data ============================== */
//main functions
void takeInput(vector<binaryInt> &Minterms, vector<binaryInt> &DontCares);

//helpers
binaryInt toBinaryInt(const string &token);

/* =============================== creating the PI chart =============================== */
//main functions
set<binaryInt> getPrimeImplicants(const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares);
map<binaryInt, string> createPrimeImplicantChart(const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares);

//helpers
bool are1BitOff(const binaryInt &a, const binaryInt &b);
binaryInt combine(const binaryInt &a, const binaryInt &b);
bool nextColumn(vector<set<binaryInt>> &groups,set<binaryInt> &implicants);

/* ================================ solving the PI chart ================================ */
//main functions
set<binaryInt> getEssentialPrimeImplicants(const map<binaryInt, string> &primeImplicantChart, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares);
string generateExpression(vector<binaryInt> &minterms,vector<binaryInt> &dontCares);

//helpers
int popcount(string s);
void deleteCol(map<binaryInt, string> &m, int i, vector<binaryInt> &minterms);

/* ====================================== display ====================================== */
//main functions
void displayPrimeImplicantChart(const map<binaryInt,string> &chart, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares);
void displayEssentialPrimeImplicants(const set<binaryInt> &EPIs, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares);

//helpers
void hline(int width);
string vspace(int width);
string toBooleanExpression(const binaryInt &b);
string toString(const binaryInt &token);

int main() {
    cout << "Initializing The Program...\n"
         << "Input File: ";
    string in, out;
    if(!(cin >> in)) return 1;
    
    cout << "Output File: ";
    if(!(cin >> out)) return 1;

    if(in.length() < 4 || in.substr(in.length() - 4, 4) != ".txt") in += ".txt";
    if(out.length() < 4 || out.substr(out.length() - 4, 4) != ".txt") out += ".txt";
    cout << "Warning: Dispalying the PI chart can take several gegabytes of memory for relatively large input!\n"
         << "Do you want to display intermediate steps? (y/n) ";
         char d; cin >> d;

    freopen(in.c_str(), "r", stdin);
    freopen(out.c_str(), "w", stdout);


    if(d == 'y' || d == 'Y') display = true;

    vector<binaryInt> minTerms, dontCares;
    try {
        takeInput(minTerms, dontCares);
        string s = generateExpression(minTerms, dontCares);
        cout << "Final Expression: " << s << endl;
    } catch(const std::exception &e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
// converts string decimals to binaryInt
binaryInt toBinaryInt(const string &token) {
    unsigned v = 0;
    for(char c : token) if(c >= '0' && c <= '9') v = v*10 + (c - '0');
    return binaryInt(v, 0);
}
// converts binaryInt to binary string with dashes
string toString(const binaryInt &token) {
    string s = "";
    for(int i = 0; i < numberOfVariables; ++i) {
        if(token.dashes & (1u << (numberOfVariables - i - 1))) s += "-";
        else if(token.num & (1u << (numberOfVariables - i - 1))) s += "1";
        else s += "0";
    }
    return s;
}
// reads the input from the input file and initializes the minterms and dontcare arrays
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
        MintermSet.insert(toBinaryInt(minterm).num);
    }

    stringstream SS(dontCares);
    string dontCare; 
    while(getline(SS, dontCare, ',')) { // fills dontcares array
        binaryInt b = toBinaryInt(dontCare);
        DontCares.push_back(b);
        DontCareSet.insert(b.num);
    }
    // fills minterms array
    if(minterms[0] == 'm') { 
        for(unsigned i : MintermSet) {
            Minterms.push_back(binaryInt(i,0));
        }
    }
    else { // converts maxterms to minterms 
        for(unsigned i = 0; i < (1u<<(numberOfVariables-1)); ++i) {
            if(!MintermSet.count(i) && !DontCareSet.count(i)) {
                Minterms.push_back(binaryInt(i,0));
            }
        }
    }
}
// checks wheter two binaryInt s differy by one bit only
bool are1BitOff(const binaryInt &a, const binaryInt &b){
    if(a.dashes!=b.dashes) return false;
    return __builtin_popcount(a.num^b.num)==1;
}
// combines two binaryInts if they differy by one bit 
binaryInt combine(const binaryInt &a, const binaryInt &b){
    if(!are1BitOff(a,b)) throw runtime_error("Not 1 bit off");
    binaryInt c;
    c.dashes=a.dashes|(a.num^b.num);
    c.num=a.num&b.num;
    return c;
}
// generates the next column of the implicant table given the previous column
bool nextColumn(vector<set<binaryInt>> &groups,set<binaryInt> &implicants){
    bool modified=false;
    vector<set<binaryInt>> newGroups(numberOfVariables+1);
    for(int i=0; i<numberOfVariables; ++i){
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
// finds all prime implicants by generating the prime implicant table 
set<binaryInt> getPrimeImplicants(const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares){
    set<binaryInt> implicants;
    vector<set<binaryInt>> groups(numberOfVariables+1);
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
// creates the prime implicant chart given the minterms and dont cares 
map<binaryInt,string> createPrimeImplicantChart(const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares){
    map<binaryInt,string> primeImplicantChart;
    set<binaryInt> implicants=getPrimeImplicants(minterms,dontCares);
    for(auto implicant:implicants){
        for(auto term:minterms){
            if(implicant.covers(term))  primeImplicantChart[implicant]+='1';
            else primeImplicantChart[implicant]+='0';
        }
    }
    if(display) displayPrimeImplicantChart(primeImplicantChart, minterms, dontCares);
    return primeImplicantChart;
}
// writes a horizontal line of the specified width for formatting purposes 
void hline(int width) {
    for(int i = 0; i < width; ++i) cout << '-';
    cout << endl;
}
// adds a space of the specified width for formatting purposes 
string vspace(int width) {
    string s = "";
    for(int i = 0; i < width; ++i) s += " ";
    return s;
}
// displays the prime implicant chart in the output file 
void displayPrimeImplicantChart(const map<binaryInt,string> &chart, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares) {
    cout << vspace(12) << "Prime Implicant" << vspace(12) << " | Binary Representation | ";
    vector<int> width;
    width.push_back(40); width.push_back(23);

    for(auto minterm : minterms) {
        cout << "m" << minterm.num << " | ";
        int len = (minterm.num) ? (1 + floor(log10(minterm.num))) : 1;
        width.push_back(3 + len);
    }
    for(auto dontCare : dontCares) {
        cout << "d" << dontCare.num << " | ";
        int len = (dontCare.num) ? (1 + floor(log10(dontCare.num))) : 1;
        width.push_back(3 + len);
    }
    cout << endl;

    int total_width = 0;
    for(auto i : width) total_width += i + 1;
    hline(total_width);
    for(auto &[pi, minterm] : chart) {
        string booleanExpression = toBooleanExpression(pi);
        string PI = toString(pi);
        int w0 = width[0] - booleanExpression.length();
        int w1 = width[1] - PI.length();
        cout << vspace(w0/2) << booleanExpression << vspace(w0 - w0/2) << "|" << vspace(w1/2) << PI << vspace(w1 - w1/2) << "|";
        for(int i = 0; i < minterm.length(); ++i) {
            int w = width[i + 2] - 1;
            cout << vspace(w/2) << minterm[i] << vspace(w - w/2) << "|";
        }
        for(int i = 0; i < dontCares.size(); ++i) {
            int w = width[i + minterm.length() + 2] - 1;
            cout << vspace(w/2) << (pi.covers(dontCares[i])) ? "1" : "0";
            cout << vspace(w - w/2) << "|";
        }
        cout << endl;
        hline(total_width);
    }
}
// generates all essential prime implicants given the prime impicant chart
set<binaryInt> getEssentialPrimeImplicants(const map<binaryInt,string> &primeImplicantChart, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares){
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
    if(display) displayEssentialPrimeImplicants(essentialPrimeImplicants, minterms, dontCares);
    return essentialPrimeImplicants;
}
// displays the essential prime implicants in the output file 
void displayEssentialPrimeImplicants(const set<binaryInt> &EPIs, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares) {
    cout << "\nEssential Prime Implicants: ";
    for(auto i : EPIs) {
        cout << toBooleanExpression(i) << " | ";
    }
    cout << endl;
}
// returns the number of ones in a binary string 
int popcount(string s) {
    int res = 0;
    for(auto i : s) if(i == '1') ++res;
    return res;
}
// deletes a specific column from the prime implicant chart 
void deleteCol(map<binaryInt, string> &m, int i, vector<binaryInt> &minterms) {
    for(auto &[implicant, s] : m) {
        s.erase(i, 1);
    }
    minterms.erase(minterms.begin() + i);
}
// converts a binaryInt to a bolean algebra expression
string toBooleanExpression(const binaryInt &b) {
    string out;
    for(int i=0; i<numberOfVariables; ++i){
        if(b.num&(1<<i)) out+=('A'+numberOfVariables-i-1);
        else if(!(b.dashes&(1<<i))){
            out+='\'';
            out+='A'-i+numberOfVariables-1;
            
        }
    }
    reverse(out.begin(),out.end());
    return out;
}
// generates the final expression of the function
string generateExpression(vector<binaryInt> &minterms,vector<binaryInt> &dontCares) {
    map<binaryInt,string> m = createPrimeImplicantChart(minterms,dontCares);
    set<binaryInt> essentials = getEssentialPrimeImplicants(m,minterms,dontCares);
    vector<binaryInt> essential(essentials.begin(),essentials.end());
    vector<binaryInt> final = essential;
    
    //remove and record EPIs and their corresponding minterms
    for(auto i : essential) {
        string s = m[i];
        int cnt = 0;
        for(int j = 0; j < s.length(); ++j) {
            if(s[j] == '1') {
                deleteCol(m, j - cnt, minterms); ++cnt;
            }
        }
        m.erase(i);
    }

    if(display) cout << "\nRemoving EPIs...\n\n";
    if(display) cout << "Remaining Minterms after removing EPIs: \n";
    if(display) displayPrimeImplicantChart(m, minterms, dontCares);

    //iteratively remove and record PIs that cover the greatest number of minterms
    //also remove the PIs that cover no minterms
    while(!m.empty() && !m.begin()->second.empty()) {
        //find the target PI
        binaryInt target; string target_minterm;
        vector<binaryInt> unnecassaryPIs;
        for(auto& [pi, minterm] : m) {
            int p1 = popcount(minterm), p2 = popcount(target_minterm);
            if(p1 == 0) unnecassaryPIs.push_back(pi);    //collect the unnecessary PIs
            else if(p1 > p2) {
                target = pi;
                target_minterm = minterm;
            }
        }
        
        //delete the corresponding minterms
        int cnt = 0;
        for(int i = 0; i < target_minterm.length(); ++i) {
            if(target_minterm[i] == '1') {
                deleteCol(m, i - cnt, minterms); ++cnt;
            }
        }

        //record the PI and remove it
        final.push_back(target);
        m.erase(target);

        //remove the unnecassary PIs
        for(auto pi : unnecassaryPIs) m.erase(pi);
    }

    string res = "";
    for(int i = 0; i < final.size(); ++i) {
        res += toBooleanExpression(final[i]);
        if(i != final.size() - 1) res += " + ";
    }
    return res;
}