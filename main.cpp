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
string lex;
char bukva;
char ops[] = { '+', '*', ' :', '-', '>', '^', '<', '@', '.', '=' };
char ops_second[] = { '+', '-' , '*', '/' , '<', '>', ':' };
string type;
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
}
TOKEN * get_token(){
	lex = "";
	if (bukva == 0){
		return 0;
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
				error("\\n in str");
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
				if (bukva = '='){
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
	else if (isdigit(bukva)){//експонента
		bool r = false, k = false;
		type = "int";
		while (isdigit(bukva) || bukva == 'e' || bukva == 'E' || bukva == '.') {
			if (bukva == '.')//считывание вещ числа
			{
				if (r == true){
					error("dot after exp");
				}
				if (true == k){
					error("second dot in real");
				}
				type = "real";
				k = true;
				next_plus();
			}
			else if (bukva == 'e' || bukva == 'E')
			{
				if (true == r){
					error("second exp in real");
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
	else if (isalpha(bukva) || '_' == bukva){//считываю лексемы
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
	}
	else if (bukva == '{'){
		next_plus();
		type = "mstr comment";
		while (bukva != '}'){
			next_plus();
			if (!ifs.eof()){
				error("eof in comment");
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
					error("eof in comment");
				}
	 		}
		}
	}
	else if (bukva == '$'){//считывание шестнадцатиричного 
		next_plus();
		type = "hex";
		while (isdigit(bukva) || (bukva >= 'A' && bukva <= 'F') || (bukva >= 'a' && bukva <= 'f')){
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
			type = "char";
			while (isdigit(bukva) || (bukva >= 'A' && bukva <= 'F') || (bukva >= 'a' && bukva <= 'f')){
				next_plus();
			}
		}
	}
	else get_char();
	TOKEN *cur;
	cur.cul = st2;
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
	TOKEN *tok
	while (!ifs.eof()){	
	tok = get_token();
		if (lex != ""){//для того чтоб не выводилась пустая лексема
			cout << tok.col << "\t" << tok.row << "\t" << tok.lex << tok.type << endl;
		
			stl2 = st;
			st2=stl;
		}
	}
	system("pause");
}
