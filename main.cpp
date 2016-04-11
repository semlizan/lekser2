#include <fstream>
#include <iostream>
#include <string>
#include <ctype.h>
using namespace std;

struct Token {
	int col;
	int row;
	string lex;
	string tip;
};


ifstream ifs("input.txt");
ofstream fout("output.txt");
string lex;
char bukva;
char ops[] = { '+', '*', ' :', '-', '>', '^', '<', '@', '.', '=' };
char ops_second[] = { '+', '-' , '*', '/' , '<', '>', ':' };
string type;
Token tok;
bool prover_masiv(char s, char *masiv, int len){
	for (int i = 0; i < len; i++){
		if (s == masiv[i]) return true;
	}
	return false;
}
void error(string msg)
{
	cout << "error: " << msg << "\n";
	system("pause");
	exit(1);
}
int stl, st;
int stl2, st2;

char get_char()
{
	ifs >> bukva;
	if (bukva == '\n'){
		stl++;
		st = 0;
	}
	if (bukva == '\t'){
		st = st / 4 * 4 + 4;
	}
	else st++;
	if (ifs.eof()) {
		return 0;
	}
	return bukva;
}
void next_plus()
{
	if (bukva != '0'){
		lex += bukva;
		get_char();
	}
	if (ifs.eof()) bukva = '~';
}
bool chislo(char c)
{
	return (isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}
void chit_znach()
{
	if (type == "integer"){
		int a;
		a = atoi(lex.c_str());
		fout << tok.col << "\t" << tok.row << "\t" << tok.tip << "\t" << tok.lex << "\t" << a << endl;
	}
	else if (type == "real"){
		double a;
		a = atof(lex.c_str());
		char buf[11];
		sprintf(buf, "%.4E", a);
		buf[8] = buf[9]; 
		buf[9] = buf[10];
		buf[10] = 0;
		fout << tok.col << "\t" << tok.row << "\t" << tok.tip << "\t" << tok.lex << "\t" << buf << endl;
	}
	else if (type == "hex"){
		long int a;
		string s;
		s = lex.substr(1, lex.size());
		a = strtol(s.c_str(), NULL, 16);
	}
	else if (type == "char"){
		string s;
		int a;
		char d;
		type = "char";
		if (lex[1] == '$')
		{
			s = lex.substr(2, lex.size());
			a = strtol(s.c_str(), NULL, 16);
		}
		else
		{
			s = lex.substr(1, lex.size());
			a = atoi(s.c_str());
		}
		cout << a << endl;
		if (a > 127){
			error("BadCC");
		}
			d = (char)a;
			fout << tok.col << "\t" << tok.row << "\t" << tok.tip << "\t" << tok.lex << "\t" << d << endl;
	}
	else if (type == "string"){
		string s;
		s = lex.substr(1, lex.size() - 2);
		fout << tok.col << "\t" << tok.row << "\t" << tok.tip << "\t" << tok.lex << "\t" << s << endl;
	}
	else 	fout << tok.col << "\t" << tok.row << "\t" << tok.tip << "\t" << tok.lex << endl;
}
Token  get_token(){
	if (bukva == 0){
		Token cur;
		return cur;
	}
	if (bukva == '\''){//считывание string
		next_plus();
		type = "string";
		while (!bukva == '\''){
			next_plus();
			if (!ifs.eof()){
				break;
			}
			if (bukva == '\n'){
				error("BadNL");
			}
		}
		lex += bukva;
	}

	else if (prover_masiv(bukva, ops, sizeof(ops) / sizeof(ops[0]))){
		type = "op";
		if (prover_masiv(bukva, ops_second, sizeof(ops_second) / sizeof(ops_second[0]))){
			if (bukva == '<'){
				next_plus();
				if (bukva == '>' || bukva == '='){
					next_plus();
				}
			}
			else {
				next_plus();
				if (bukva == '='){
					next_plus();
				}
			}
		}
		else if (bukva == ':'){
			next_plus(); 
			type = "sep"; 
		}
		else if (bukva == '.') {
			next_plus();
			if (bukva == '.') {
				next_plus();
				type = "sep";
			}
		}
	}
	else if (isdigit(bukva)){//експонента
		bool r = false, k = false;
		type = "int";
		while (isdigit(bukva) || bukva == 'e' || bukva == 'E' || bukva == '.') {
			if (bukva == '.')//считывание вещ числа
			{
				if (r == true){
					error("NoFract");
				}
				if (true == k){
					error("NoFract");
				}
				type = "real";
				k = true;
				next_plus();
			}
			else if (bukva == 'e' || bukva == 'E')
			{
				if (true == r){
					error("NoFract");
				}
				next_plus();
				r = true;
				if (bukva == '+' || bukva == '-'){
					next_plus();
				}
			}
			else {
				next_plus();
			}
		}
	}
	else if (isalpha(bukva) || '_' == bukva){//считывание индефекатора
		while (isdigit(bukva) || isalpha(bukva) || '_' == bukva){
			next_plus();
			type = "ident";
			if (ifs.eof()) {
				break;
			}
		}
	}
	else if (bukva == '/'){//строчный комент
		next_plus();
			type = "op";
		if (bukva == '='){
			next_plus();
		}
		else if (bukva == '/'){
			type = "str comment";
		}
			while (bukva != '\n' && !ifs.eof()){
				get_char();
			}
		}
	else if (bukva == '{'){
		next_plus();
		type = "mstr comment";
		while (bukva != '}'){
			next_plus();
			if (!ifs.eof()){
				error("BadEOF");
			}
		}
		next_plus();

	}
	else if (bukva == '('){
		next_plus();
		if (bukva == '*'){
			next_plus();
			type = "mstr comment";
			while (1){
				if (bukva == '*'){
					next_plus();
					if (bukva == ')'){
						next_plus();
						break;
					}
				}
				next_plus();
				if (!ifs.eof()){
					error("BadEOF");
				}
	 		}
		}
	}
	else if (bukva == '$'){//считывание шестнадцатиричного 
		next_plus();
		if (!chislo(bukva)){
			error("NoHex");
		}
		type = "hex";
		while (chislo(bukva)){
			next_plus();
		}
	}
	else if (bukva == '#'){//символьная литера
		next_plus();
		type = "char";
		if (isdigit(bukva)){
			while (isdigit(bukva)){
				next_plus();
			}
		}
		else if (bukva == '$'){//считывание шестнадцатиричного в символьной литере
			next_plus();
			if (!chislo(bukva)){
				error("NoHex");
			}
			while (chislo(bukva)){
				next_plus();
			}
		}
		else if (lex[0] == '#'){
			error("NoCC");
		}
	}
	else error("BadChar");
	Token cur;
	cur.col = st2;
	cur.row = stl2;
	cur.lex = lex;
	cur.tip = type;
	return cur;
}
int main(){
	ifs >> noskipws;
	stl = 1;
	st = 0;
	stl2 = st2 = 1;
	get_char();
	while (!ifs.eof()){	
	tok = get_token();
		if (lex != ""){//для того чтоб не выводилась пустая лексема
			chit_znach();
			stl2 = st;
			st2=stl;
			lex = "";
		}
	}
	system("pause");
}
