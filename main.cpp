#include <fstream>
#include <iostream>
#include <string>
#include <ctype.h>
using namespace std;

ifstream ifs("input.txt");
char get_char()
{
	char bukva;
	ifs >> bukva;
	if (ifs.eof()) {
		return 0;
	}
	return bukva;
}

int main(){

	ifs >> noskipws;
	string type;
	string lex;
	char bukva = get_char();
	while (!ifs.eof()){
		if (bukva == 0){
			break;
		}
		if (bukva == '\''){//считывание string
			lex += bukva;
			bukva = get_char();
			while (!(bukva == '\'' || bukva == '\n')){
				lex += bukva;
				bukva = get_char();
			}
			lex += bukva;
		}

		else if (isdigit(bukva)){
			while (isdigit(bukva)) {
				lex += bukva;
				bukva = get_char();
			}
			if (bukva == '.')//считывание вещ числа
			{
				type = "real";
				lex += bukva;
				bukva = get_char();
				while (isdigit(bukva)) {
					lex += bukva;
					bukva = get_char();
				}
			}
			
		}
		else if (isalpha(bukva) || '_' == bukva){//считываю лексемы
			while (isdigit(bukva) || isalpha(bukva) || '_' == bukva){
				lex += bukva;
				bukva = get_char();
			}
		}
		else  if (bukva == '/'){//строчный комент
			lex += bukva;
			bukva = get_char();
			if (bukva == '/'){
				type = "str comment";
				lex += bukva;
				while (bukva != '\n' && !ifs.eof()){
					bukva = get_char();
				}
			}
		}
		else if (bukva == '$'){//считывание шестнадцатиричного 
			lex += bukva;
			bukva = get_char();
			while (isdigit(bukva) || (bukva >= 'A' && bukva <= 'F') || (bukva >= 'a' && bukva <= 'f')){
				lex += bukva;
				bukva = get_char();
			}
		}
		else if (bukva == '\n') bukva = get_char();
		cout << lex << endl;
	}
	system("pause");
}