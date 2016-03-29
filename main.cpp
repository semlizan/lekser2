#include <fstream>
#include <iostream>
#include <string>
#include <ctype.h>
using namespace std;

ifstream ifs("input.txt");
string lex;
char bukva;
char get_char()
{
	ifs >> bukva;
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
	};
}

int main(){
	ifs >> noskipws;
	string type;
	get_char();
	while (!ifs.eof()){
		if (bukva == 0){
			break;
		}
		if (bukva == '\''){//считывание string
			next_plus();
			while (!(bukva == '\'' || bukva == '\n')){
				next_plus();
				if (!ifs.eof()){
					break;
				}
			}
			lex += bukva;
		}

		else if (isdigit(bukva)){//експонента
				bool r, k;
				r, k = false;
			while (isdigit(bukva) || bukva == 'e' || bukva == 'E' || bukva == '.') {
				if (bukva == '.')//считывание вещ числа
				{
					type = "real";
					k = true;
				}
				if (bukva == 'e' || bukva == 'E'){
					next_plus();
					r = true;
					if (isdigit(bukva)){
						break;
					}
					if (bukva == '+' || bukva == '-'){
						next_plus();
					}
				}
				next_plus();
			}
		}
		else if (isalpha(bukva) || '_' == bukva){//считываю лексемы
			while (isdigit(bukva) || isalpha(bukva) || '_' == bukva){
				next_plus();
				if (ifs.eof()) {
					break;
				}
			}
		}
		else  if (bukva == '/'){//строчный комент
			next_plus();
			if (bukva == '/'){
				type = "str comment";
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
					break;
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
					if (bukva == '*'){
						next_plus();
						if (bukva == ')'){
							next_plus();
							break;
						}
					}
				}
			}
		}
		else if (bukva == '$'){//считывание шестнадцатиричного 
			next_plus();
			while (isdigit(bukva) || (bukva >= 'A' && bukva <= 'F') || (bukva >= 'a' && bukva <= 'f')){
				next_plus();
			}
		}
		else if (bukva == '#'){//символьная литера
			next_plus();
			if (isdigit(bukva)){
				while (isdigit(bukva)){
					next_plus();
				}
		}
			else if (bukva == '$'){//считывание шестнадцатиричного в символьной литере
				next_plus();
				while (isdigit(bukva) || (bukva >= 'A' && bukva <= 'F') || (bukva >= 'a' && bukva <= 'f')){
					next_plus();
				}
			}
		else get_char();
		if (lex != ""){//для того чтоб не выводилась пустая лексема
			cout << lex << endl;
		}
		lex = "";
	}
	system("pause");
}