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
#include <fstream> // Added for file writing
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
void displayPrimeImplicantChart(const map<binaryInt,string> &chart, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares,bool showDontcares);
void displayEssentialPrimeImplicants(const set<binaryInt> &EPIs, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares);

//helpers
void hline(int width);
string vspace(int width);
string toBooleanExpression(const binaryInt &b);
string toString(const binaryInt &token);

// =================================== NEW VERILOG GENERATION ===================================

/**
  Generates a complete .v Verilog file using Verilog primitives.
  finalTerms A vector of the binaryInts in the final solution.
  numVars The total number of variables.
  booleanExpression The human-readable string (e.g., "A'B + C").
  verilogFilename The name of the file to create (e.g., "logic_circuit.v").
 */
void generateVerilogFile(const vector<binaryInt> &finalTerms, int numVars, const string& booleanExpression, const string& verilogFilename) {
    ofstream outFile(verilogFilename);
    if (!outFile) {
        cerr << "Error: Could not open Verilog file " << verilogFilename << " for writing." << endl;
        return;
    }
    // --- Write Header & Inputs/Outputs ---
    outFile << "// Function: F = " << booleanExpression << endl;
    outFile << "// Generated using Verilog primitives." << endl;
    outFile << "\nmodule logic_function (" << endl;

    // Write input wire list
    for (int i = 0; i < numVars; ++i) {
        outFile << "input  " << (char)('A' + i);
        if (i < numVars - 1) {
            outFile << ",";
        }
        outFile << endl;
    }

    outFile << ",\noutput Final" << endl;
    outFile << ");" << endl;

    // --- Handle edge case: F = 0 ---
    if (finalTerms.empty()) {
        outFile << "\n// Function is always 0" << endl;
        outFile << "assign F = 1'b0;" << endl;
        outFile << "\nendmodule" << endl;
        outFile.close();
        cerr << "Verilog file generation complete (using primitives)." << endl;
        return;
    }

    // --- Handle edge case: F = 1 ---
    // Check if the only term is '1' (all dashes)
   bool all_dashes_term = false;
    if (finalTerms.size() == 1) {
        bool all_dashes = true;
        for(int i = 0; i < numVars; ++i) {
            // Check based on toBooleanExpression logic (LSB to MSB)
            if (!((finalTerms[0].dashes >> i) & 1u)) {
                all_dashes = false;
                break;
            }
        }
        all_dashes_term = all_dashes;
    }

    if (all_dashes_term) {
         outFile << "\n// Function is always 1" << endl;
         outFile << "assign F = 1'b1;" << endl;
         outFile << "\nendmodule" << endl;
         outFile.close();
         cerr << "Verilog file generation complete (using primitives)." << endl;
         return;
    }

    // --- 1. Find and declare all needed inverters ---
    set<char> inverted_vars;
    for (const auto& term : finalTerms) {
        for (int i = 0; i < numVars; ++i) {
            // If bit is '0' (not dash and not '1')
            if (!(term.dashes & (1u << i)) && !(term.num & (1u << i))) {
                inverted_vars.insert('A' + (numVars - 1 - i));
            }
        }
    }

    if (!inverted_vars.empty()) {
        outFile << "// Wires for inverted inputs" << endl;
        for (char var : inverted_vars) {
            outFile << "wire not_" << var << ";" << endl;
        }
    }

    // Wires for AND gate outputs (one for each product term)
    if (!finalTerms.empty()) {
        outFile << "// Wires for product terms (AND gate outputs)" << endl;
        for (size_t i = 0; i < finalTerms.size(); ++i) {
            outFile << "wire term_" << i << ";" << endl;
        }
    }

    // --- 2. Instantiate Inverters ---
    if (!inverted_vars.empty()) {
        outFile << "\n// --- Input Inverters ---" << endl;
        for (char var : inverted_vars) {
            outFile << "not inv_" << var << " (not_" << var << ", " << var << ");" << endl;
        }
    }

    // --- 3. Instantiate AND gates (Product Terms) ---
    outFile << "\n// --- Product Terms (AND gates) ---" << endl;
    for (size_t i = 0; i < finalTerms.size(); ++i) {
        const auto& term = finalTerms[i];
        vector<string> and_inputs;

        // Iterate from LSB (bit 0) to MSB
        for (int j = 0; j < numVars; ++j) {
            // Map bit position to var name (bit 0 -> 'D', bit 3 -> 'A' for 4 vars)
            char varName = 'A' + (numVars - 1 - j);

            // If bit is not a dash
            if (!(term.dashes & (1u << j))) {
                if (term.num & (1u << j)) { // If it's '1'
                    and_inputs.push_back(string(1, varName)); // e.g., "A"
                } else { // If it's '0'
                    and_inputs.push_back("not_" + string(1, varName)); // e.g., "not_A"
                }
            }
        }

        // Now create the gate
        outFile << "// Term " << i << ": " << toBooleanExpression(term) << endl;
        if (and_inputs.empty() && numVars > 0) {
            // Should be caught by F=1 case, but good to have
            outFile << "assign term_" << i << " = 1'b1;" << endl;
        } else if (and_inputs.size() == 1) {
            // Only one literal in term, just assign it (no gate needed)
            outFile << "assign term_" << i << " = " << and_inputs[0] << ";" << endl;
        } else if (!and_inputs.empty()) { // Only build gate if there are inputs
            // A proper AND gate
            outFile << "and and_" << i << " (term_" << i << ", ";
            for (size_t k = 0; k < and_inputs.size(); ++k) {
                outFile << and_inputs[k] << (k < and_inputs.size() - 1 ? ", " : "");
            }
            outFile << ");" << endl;
        } else {
            // Case for F=0 (finalTerms is empty), already handled.
            // This case might be F=1 if numVars is 0, also handled.
        }
    }

    // --- 4. Instantiate OR gate (Sum Term) ---
    outFile << "\n// --- Sum Term (OR gate) ---" << endl;
    if (finalTerms.size() == 1) {
        // Only one term, no OR gate needed. Assign directly.
        outFile << "assign Final = term_0;" << endl;
    } else {
        outFile << "or or_final (Final, ";
        for (size_t i = 0; i < finalTerms.size(); ++i) {
            outFile << "term_" << i << (i < finalTerms.size() - 1 ? ", " : "");
        }
        outFile << ");" << endl;
    }

    outFile << "\nendmodule" << endl;
    outFile.close();
    cerr << "Verilog file generation complete (using Verilog primitives)." << endl;
}

// ================================= END NEW VERILOG ==================================


int main() {
    cout << "Initializing The Program...\n"
         << "Input File: ";
    string in, out;
    if(!(cin >> in)) return 1;

    cout << "Output File: ";
    if(!(cin >> out)) return 1;

    if(in.length() < 4 || in.substr(in.length() - 4, 4) != ".txt") in += ".txt";
    if(out.length() < 4 || out.substr(out.length() - 4, 4) != ".txt") out += ".txt";

    // *** Prompt user before redirecting file streams ***
    cout << "Warning: Dispalying the PI chart can take several gegabytes of memory for relatively large input!\n"
         << "Do you want to display intermediate steps? (y/n) ";
    char d; cin >> d;

    // *** Redirect files after user interaction ***
    freopen(in.c_str(), "r", stdin);
    freopen(out.c_str(), "w", stdout);


    if(d == 'y' || d == 'Y') display = true;

    vector<binaryInt> minTerms, dontCares;
    try {
        takeInput(minTerms, dontCares);
        string s = generateExpression(minTerms, dontCares); // This will also generate the .v file
        cout << "Final Expression: " << s << endl;
    } catch(const std::exception &e) {
        // Send error to cerr, which is not redirected
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

    if(dontCares.length() != 1) {
        stringstream SS(dontCares);
        string dontCare;
        while(getline(SS, dontCare, ',')) { // fills dontcares array
            binaryInt b = toBinaryInt(dontCare);
            DontCares.push_back(b);
            DontCareSet.insert(b.num);
        }
    }

    if(minterms.length() != 1) {
        stringstream ss(minterms);
        string minterm;
        while(getline(ss, minterm, ',')) {
            MintermSet.insert(toBinaryInt(minterm).num);
        }
        // fills minterms array
        if(minterms[0] == 'm') {
            for(unsigned i : MintermSet) {
                Minterms.push_back(binaryInt(i,0));
            }
        }
        else { // converts maxterms to minterms
            for(unsigned i = 0; i < (1u << numberOfVariables); ++i) {
                if(!MintermSet.count(i) && !DontCareSet.count(i)) {
                    Minterms.push_back(binaryInt(i,0));
                }
            }
        }
    }
    else {
        if(minterms[0] == 'M') {
            for(int i = 0; i < (1<<numberOfVariables); ++i) Minterms.push_back(binaryInt(i, 0));
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
    if(display) displayPrimeImplicantChart(primeImplicantChart, minterms, dontCares,true);
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
void displayPrimeImplicantChart(const map<binaryInt,string> &chart, const vector<binaryInt> &minterms, const vector<binaryInt> &dontCares,bool showDontcares) {
    cout << vspace(12) << "Prime Implicant" << vspace(12) << " | Binary Representation | ";
    vector<int> width;
    width.push_back(40); width.push_back(23);

    for(auto minterm : minterms) {
        cout << "m" << minterm.num << " | ";
        int len = (minterm.num) ? (1 + floor(log10(minterm.num))) : 1;
        width.push_back(3 + len);
    }
    if(showDontcares) {
        for(auto dontCare : dontCares) {
            cout << "d" << dontCare.num << " | ";
            int len = (dontCare.num) ? (1 + floor(log10(dontCare.num))) : 1;
            width.push_back(3 + len);
        }
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
        if(showDontcares){
            for(int i = 0; i < dontCares.size(); ++i) {
                int w = width[i + minterm.length() + 2] - 1;
                cout << vspace(w/2) << (pi.covers(dontCares[i])) ? "1" : "0";
                cout << vspace(w - w/2) << "|";
            }
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
    for(int i=0; i<EPIs.size(); ++i) {
        cout << toBooleanExpression(*next(EPIs.begin(),i)) << (i!=EPIs.size()-1 ? ", ":"");
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
    bool all_dashes = true;
    for(int i=0; i<numberOfVariables; ++i){
        if(b.num&(1u<<i)) {
            out+=('A'+numberOfVariables-i-1);
            all_dashes = false;
        }
        else if(!(b.dashes&(1u<<i))){
            out+='\'';
            out+='A'-i+numberOfVariables-1;
            all_dashes = false;
        }
    }
    reverse(out.begin(),out.end());

    // Handle the case for a constant '1' (all dashes)
    if (all_dashes && numberOfVariables > 0) return "1";

    return out;
}
// generates the final expression of the function
string generateExpression(vector<binaryInt> &minterms,vector<binaryInt> &dontCares) {
    map<binaryInt,string> m = createPrimeImplicantChart(minterms,dontCares);
    set<binaryInt> essentials = getEssentialPrimeImplicants(m,minterms,dontCares);
    vector<binaryInt> essential(essentials.begin(),essentials.end());
    vector<binaryInt> final = essential;
    
    if(display) cout << "\nRemoving EPIs...\n\n";
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

    if(display) cout << "Remaining Minterms after removing EPIs: \n\n";
    if(display) displayPrimeImplicantChart(m, minterms, dontCares,false);

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
        // Add check to avoid adding empty/default target
        if (!target_minterm.empty()) {
            final.push_back(target);
            m.erase(target);
        }

        //remove the unnecassary PIs
        for(auto pi : unnecassaryPIs) m.erase(pi);

        // Add break condition if no target was found
        if (target_minterm.empty()) break;
    }

    string res = "";
    // Handle F=0 and F=1 edge cases ***
    if (final.empty()) {
        res = "0"; // Function is always 0
    } else {
        for(int i = 0; i < final.size(); ++i) {
            string term_str = toBooleanExpression(final[i]);
            if (term_str == "1") {
                res = "1"; // If any term is '1', the whole expression is '1'
                final = {final[i]}; // Clear other terms for Verilog gen
                break;
            }
            res += term_str;
            if(i != final.size() - 1) res += " + ";
        }
    }

    // We pass 'final' (the list of terms) and 'res' (the string "A'B + C")
    generateVerilogFile(final, numberOfVariables, res, "logic_circuit.v");

    return res;
}