#include <iostream> 
#include <string> 
#include <stack> 
#include <vector>

using namespace std;

//
// 추상클래스 Number String을 기반으로 하는 10진수, 16진수, 2진수 객체
//
class NumberString {
protected:
	int num;
public:
	// 입력받은 문자열을 각각의 방식으로 10진수로 저장
	virtual void setNum(string numstr) = 0; 
	// 저장된 숫자를 출력
	virtual int getNum() = 0;
};
//10진수 클래스
class DecNumberString : public NumberString {
public:
	DecNumberString(string numstr) {
		this->setNum(numstr);
	}
	void setNum(string numstr) {
		this->num = stoi(numstr);
	}
	int getNum() {
		return this->num;
	}
};
//16진수 클래스
class HexNumberString : public NumberString {
public:
	HexNumberString(string numstr) {
		this->setNum(numstr);
	}
	void setNum(string numstr) {
		this->num = stoi(numstr.substr(2, numstr.length() - 2), nullptr, 16);
	}
	int getNum() {
		return this->num;
	}
};
//2진수 클래스
class BinNumberString : public NumberString {
public:
	BinNumberString(string numstr) {
		this->setNum(numstr);
	}
	void setNum(string numstr) {
		this->num = stoi(numstr.substr(2,numstr.length()-2),nullptr,2);
	}
	int getNum() {
		return this->num;
	}
};
//
// +, -, *, /, ( 등 후위 표기법으로 변환하기 위한 스텍에 저장될 객체를 선언하는 클래스
// operator의 우선순위와 각 연산에 따른 다른 출력을 수행함.
//
//추상클래스 Oper
class Oper {
protected:
	//우선순위
	int prior;
public:
	//연산자를 문자열로 출력
	virtual string getOper() = 0;
	//우선순위 비교를 위한 출력
	virtual int getPrior() = 0;
	// 입력받는 정수 2개의 연산
	virtual int calculrate(int a, int b) = 0;
};
// 덧셈 클래스
class AddOper : public Oper {

public:
	AddOper() {
		this->prior = 2;
		// 덧셈/뺄셈은 곱셈/나눗셈보다 늦게 연산되므로 중간 우선순위 2
	}
	string getOper() {
		return "+";
	}
	int getPrior() {
		return this->prior;
	}
	int calculrate(int a, int b) {
		return a + b;
	}
};
// 뺄셈 클래스
class SubOper : public Oper {

public:
	SubOper() {
		this->prior = 2;
		// 덧셈/뺄셈은 곱셈/나눗셈보다 늦게 연산되므로 중간 우선순위 2
	}
	string getOper() {
		return "-";
	}
	int getPrior() {
		return this->prior;
	}
	int calculrate(int a, int b) {
		return a - b;
	}
};
// 곱셈 클래스
class MulOper : public Oper {

public:
	MulOper() {
		this->prior = 3;
		// 곱셈/나눗셈은 덧셈/뺄셈보다 먼저 연산되므로 덧셈/뺄셈보다 더 높은 우선순위 3
	}
	string getOper() {
		return "*";
	}
	int getPrior() {
		return this->prior;
	}
	int calculrate(int a, int b) {
		return a * b;
	}
};
// 나눗셈 클래스
class DivOper : public Oper {

public:
	DivOper() {
		this->prior = 3;
		// 곱셈/나눗셈은 덧셈/뺄셈보다 먼저 연산되므로 덧셈/뺄셈보다 더 높은 우선순위 1
	}
	string getOper() {
		return "/";
	}
	int getPrior() {
		return this->prior;
	}
	int calculrate(int a, int b) {
		return a / b;
	}
};
// 열기 괄호 클래스
class BraOper : public Oper {

public:
	BraOper() {
		this->prior = 1; 
		// 후위 표기법으로 전환하기 앞어 괄호는 닫는 괄호가 나올때까지 pop되면 안되므로 제일 낮은 우선순위
	}
	string getOper() {
		return NULL; // 후위 표기법에서 괄호는 쓰이지 않으므로 초기화만 함.
	}
	int getPrior() {
		return this->prior;
	}
	int calculrate(int a, int b) {
		return 0; // 후위 표기법 계산에서 괄호는 쓰이지 않으므로 초기화만 함.
	}
};
//
// 추상클래스 Oper와 구현되는 함수를 떨어뜨려 놓을 수 있도록 다리 역할을 맡을 클래스 
// 단, 한번만 객체를 선언하고 연산자는 유동적으로 바꾸기 위해 매개변수를 받아온다.
//
class Calculate {
protected:
	Oper *oper;
public:
	Calculate(Oper *oper) {
		this->oper = oper;
	}
	virtual int operCalculate(int b, int a, Oper* oper) = 0;
};

//연산을 수행할 2개의 숫자를 저장하며 계산까지 확실히 맡는다.
class Calculator : public Calculate {
private:
	int numA, numB;

public:	
	Calculator(int numA, int numB, Oper *oper) : Calculate(oper) {
		this->numA = numA;
		this->numB = numB;
	}
	int operCalculate(int b, int a, Oper* oper) {
		this->numA = a;
		this->numB = b;
		return oper->calculrate(numA, numB);
	}
};


//
// 수식이 올바른지 확인하며 splitString에 잘라서 저장
// 예외처리를 사용하되 나올수 있는 오류의 종류를 아래와 같이 구분하였다.
// 괄호 오류 / 연산 중첩 / 올바르지 않은 문자 / 연산으로 시작/종료(+2 or 2-) /
//
bool IsValid(string aString, vector<string> *splitString) {
	
	//받아온 문자열에서 띄어쓰기만 제거한다.
	aString.erase(remove(aString.begin(), aString.end(), ' '), aString.end());

	int strLength = aString.length();
	int nestingOper = 0; // 연산 중첩 확인용, 계산 연산이 2중첩시 오류 발생
	int invalidBracket = 0; // 괄호 오류 확인용, 음수로 내려가거나 전체 작업이 끝났을때 0이 아니면 오류 발생
	try {
		int i = 0;
		if (aString[i] == '*' || aString[i] == '/' || aString[i] == '+' || aString[i] == '-') {
			throw string("연산으로 시작");
		}
		for (i; i < strLength; i++) {
			//괄호 처리
			if (aString[i] == '(') {
				invalidBracket++;
				if (aString[i + 1] == '*' || aString[i + 1] == '/' || aString[i + 1] == '+' || aString[i + 1] == '-') {
					throw string("연산으로 시작");
				}
				splitString->push_back(aString.substr(i,1));
			}
			else if (aString[i] == ')') {
				if (--invalidBracket < 0) { 
					throw string("괄호 오류");
				}
				else {
					if (aString[i - 1] == '*' || aString[i - 1] == '/' || aString[i - 1] == '+' || aString[i - 1] == '-') {
						throw string("연산으로 종료");
					}
				}
				splitString->push_back(aString.substr(i,1));
			}
			//사칙 연산 처리
			else if (aString[i] == '*' || aString[i] == '/' || aString[i] == '+' || aString[i] == '-') {
				if (++nestingOper >= 2) {
					throw string("연산 중첩");
				}
				splitString->push_back(aString.substr(i,1));
			}
			//2진수, 16진수 처리
			else if (aString[i] == '0') {
				int j = 1;
				if (aString[i + j] == 'x') { // 16진수 처리
					nestingOper = 0;
					j = 2;
					while (1) {
						if ((aString[i + j] >= '0' && aString[i + j] <= '9') || (aString[i + j] >= 'A' && aString[i + j] <= 'F')) {
							j++;
						}
						else if(aString[i + j] == ')' || aString[i + j] == '*' || aString[i + j] == '/' || aString[i + j] == '+' || aString[i + j] == '-' || aString[i + j] == NULL){
							splitString->push_back(aString.substr(i, j));
							i += j - 1;
							break;
						}
						else {
							throw string("올바르지 않은 문자 or 괄호");
						}
					}
				}
				else if (aString[i + j] == 'b') { // 2진수 처리
					nestingOper = 0;
					j = 2;
					while (1) {
						if (aString[i + j] >= '0' && aString[i + j] <= '1') {
							j++;
						}
						else if (aString[i + j] == ')' || aString[i + j] == '*' || aString[i + j] == '/' || aString[i + j] == '+' || aString[i + j] == '-' || aString[i + j] == NULL) {
							splitString->push_back(aString.substr(i, j));
							i += j - 1;
							break;
						}
						else {
							throw string("올바르지 않은 문자 or 괄호");
						}
					}
				}
				else { //0으로 시작하는 숫자 처리
					nestingOper = 0;
					while (1) {
						if (aString[i + j] >= '0' && aString[i + j] <= '9') {
							j++;
						}
						else if (aString[i + j] == ')' || aString[i + j] == '*' || aString[i + j] == '/' || aString[i + j] == '+' || aString[i + j] == '-' || aString[i + j] == NULL) {
							splitString->push_back(aString.substr(i, j));
							i += j - 1;
							break;
						}
						else {
							throw string("올바르지 않은 문자 or 괄호");
						}
					}
				}
				
			}//10진수 처리
			else if (aString[i] >= '0' && aString[i] <= '9') {
				nestingOper = 0;
				int j = 1;
				while (1) {
					if (aString[i + j] >= '0' && aString[i + j] <= '9') {
						j++;
					}
					else if (aString[i + j] == ')' || aString[i + j] == '*' || aString[i + j] == '/' || aString[i + j] == '+' || aString[i + j] == '-' || aString[i + j] == NULL) {
						splitString->push_back(aString.substr(i, j));
						i += j - 1;
						break;
					}
					else {
						throw string("올바르지 않은 문자 or 괄호");
					}
				}
			}
			else if (aString[i] == ' ') {

			}
			else {
				throw string("올바르지 않은 문자");
			}
		}
		if (aString[i - 1] == '*' || aString[i - 1] == '/' || aString[i - 1] == '+' || aString[i - 1] == '-') {
			throw string("연산으로 종료");
		}
		if (invalidBracket != 0) {
			throw string("괄호 오류");
		}

	}
	catch (string exception) {
		cout << "Error- Error- Error- Error- Error- Error\n입력 오류가 발견되었습니다. : " << exception << "\n"<< endl;
		splitString->clear();
		return 0;
	}
	return 1;
}
// 아래InToPost, PostfixCalculate애서 오퍼레이터 객체를 쓰기 위해서 미리 초기화를 해둔다. 
Oper* addOper = new AddOper();
Oper* subOper = new SubOper();
Oper* mulOper = new MulOper();
Oper* divOper = new DivOper();
Oper* braOper = new BraOper();

//
// 중위표기법 -> 후위표기법으로 변경시킴.
//
void InToPost(vector<string> *infixString) {
	//변경될 문자열이 저장될 공간.
	vector<string> *postfixString = new vector<string>;
	//연산자를 저장할 스택.
	stack<Oper*> *aStack = new stack<Oper*>;
		
	for (int i = 0; i < infixString->size(); i++) {
		string data = infixString->at(i);
		if (data == "(") {
			//괄호는 반드시 push한다.
			aStack->push(braOper);
		}
		else if (data == "+") {
			// 우선순위 비교시 null포인터 오류를 피하기 위해 비어있지 않을땐 바로 push한
			if (!aStack->empty()) {
				// 스텍 탑에 있는 우선순위보다 크거나 같을때 우선순위가 작아질때까지 pop
				while (aStack->top()->getPrior() >= addOper->getPrior()) {
					postfixString->push_back(aStack->top()->getOper());
					aStack->pop();
					if (aStack->empty()) {
						break;
					}
				}
			}
			aStack->push(addOper);
		}
		else if (data == "-") {
			if (!aStack->empty()) {
				while (aStack->top()->getPrior() >= subOper->getPrior()) {
					postfixString->push_back(aStack->top()->getOper());
					aStack->pop();
					if (aStack->empty()) {
						break;
					}
				}
			}
			aStack->push(subOper);
		}
		else if (data == "*") {
			if (!aStack->empty()) {
				while (aStack->top()->getPrior() >= mulOper->getPrior()) {
					postfixString->push_back(aStack->top()->getOper());
					aStack->pop();
					if (aStack->empty()) {
						break;
					}
				}
			}
			aStack->push(mulOper);
		}
		else if (data == "/") {
			if (!aStack->empty()) {
				while (aStack->top()->getPrior() >= divOper->getPrior()) {
					postfixString->push_back(aStack->top()->getOper());
					aStack->pop();
					if (aStack->empty()) {
						break;
					}
				}
			}
			aStack->push(divOper);
		}
		//괄호 닫기가 나왔을땐 괄호가 나올때까지 pop해준다.
		else if (data == ")") {
			while (aStack->top()->getPrior() != 1) {
				postfixString->push_back(aStack->top()->getOper());
				aStack->pop();
			}
			aStack->pop();
		}
		else {
			// 예외 입력은 이전에 다 처리했으므로 나머지 숫자일 경우는 바로 스트링에 저장한다.
			postfixString->push_back(data);
		}
	}
	//입력이 끝나면 스택의 모든 연산자를 pop한다.
	while (!aStack->empty()) {
		postfixString->push_back(aStack->top()->getOper());
		aStack->pop();
	}
	//입력받은 중간 표기법 포인터를 삭제하고 덮어씌운다.
	infixString->clear();
	for (int i = 0; i < postfixString->size(); i++) {
		infixString->push_back(postfixString->at(i));
	}

}

//
//후위표기법 계산
// 브릿지 패턴 사용
//
int PostfixCalculate(vector<string>* postfixString) {
	//계산할 숫자를 저장할 stack
	stack<int> *numStack = new stack<int>;
	// 계산을 수행해줄 Calculator 객체를 선언하고 의미 없는 값으로 초기화해준다.
	Calculate* calculator = new Calculator(0, 0, braOper);

	for(int i = 0; i < postfixString->size(); i++) {
		//입력받은 data 저장
		string data = postfixString->at(i);
		int a, b;
		if (data == "+") {
			//계산할 숫자를 먼저 저장한 후 pop하고
			a  = numStack->top();
			numStack->pop();
			//계산될 숫자를 나중으로 하여 연산 순서를 맞춘다.
			b = numStack->top();
			numStack->pop();
			numStack->push(calculator->operCalculate(a , b,addOper));
		}
		else if (data == "-") {
			a = numStack->top();
			numStack->pop();
			b = numStack->top();
			numStack->pop();
			numStack->push(calculator->operCalculate(a, b, subOper));
		}
		else if (data == "*") {
			a = numStack->top();
			numStack->pop();
			b = numStack->top();
			numStack->pop();
			numStack->push(calculator->operCalculate(a, b, mulOper));
		}
		else if (data == "/") {
			a = numStack->top();
			numStack->pop();
			b = numStack->top();
			numStack->pop();
			numStack->push(calculator->operCalculate(a, b, divOper));
		}
		//2진수 16진수를 구분할 x,b를 만들기 위한 조건문.
		else if(data.length() > 2){
			if (data.at(1) == 'x') {//16진수
				//숫자는 바로 스텍에 저장한다.
				numStack->push((new HexNumberString(data))->getNum());
			}
			else if (data.at(1) == 'b') {//2진수
				numStack->push((new BinNumberString(data))->getNum());
			}
			else {// 2자리 이상 10진수
				numStack->push((new DecNumberString(data))->getNum());
			}
		}//예외처리는 끝났으므로 나머지는 한자리 10진수
		else {
			numStack->push((new DecNumberString(data))->getNum());
		}
	}//마지막에 스텍에 최종 결과가 들어있으므로 top해서 반환한다.
	return numStack->top();
}


int main() {
	//수식 입력
	string input;
	cout << "수식을 입력하세요 : ";
	getline(cin, input);

	// 저장될 스트링 배열/백터
	vector<string>* splitString = new vector<string>;

	// 입력받은 수식에 오류가 있나 확인
	while (!IsValid(input, splitString)) {
		cout << "수식을 입력하세요 : ";

		getline(cin, input);
	}
	// 입력받은 수식을 출력한다.
	cout <<  "\n입력된 수식 :  ";
	for (int i = 0; i < splitString->size(); i++) {
		cout << splitString->at(i) << " ";
	}
	cout << endl;
	//후위 표기법으로 변환한다.
	InToPost(splitString);

	// 변환된 수식을 출력한다.
	cout << "\n후위 표기법으로 변환한 수식 : ";
	for (int i = 0; i < splitString->size(); i++) {
		cout << splitString->at(i) << " ";
	}
	cout << endl;

	//계산 결과 출력
	cout <<"\n정답 : " << PostfixCalculate(splitString) << endl;
	return 0;
}

