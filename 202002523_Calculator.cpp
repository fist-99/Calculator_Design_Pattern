#include <iostream> 
#include <string> 
#include <stack> 
#include <vector>

using namespace std;
//대충 주석
//
// �߻�Ŭ���� Number String�� ������� �ϴ� 10����, 16����, 2���� ��ü
//
class NumberString {
protected:
	int num;
public:
	// �Է¹��� ���ڿ��� ������ ������� 10������ ����
	virtual void setNum(string numstr) = 0; 
	// ����� ���ڸ� ���
	virtual int getNum() = 0;
};
//10���� Ŭ����
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
//16���� Ŭ����
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
//2���� Ŭ����
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
// +, -, *, /, ( �� ���� ǥ������� ��ȯ�ϱ� ���� ���ؿ� ����� ��ü�� �����ϴ� Ŭ����
// operator�� �켱������ �� ���꿡 ���� �ٸ� ����� ������.
//
//�߻�Ŭ���� Oper
class Oper {
protected:
	//�켱����
	int prior;
public:
	//�����ڸ� ���ڿ��� ���
	virtual string getOper() = 0;
	//�켱���� �񱳸� ���� ���
	virtual int getPrior() = 0;
	// �Է¹޴� ���� 2���� ����
	virtual int calculrate(int a, int b) = 0;
};
// ���� Ŭ����
class AddOper : public Oper {

public:
	AddOper() {
		this->prior = 2;
		// ����/������ ����/���������� �ʰ� ����ǹǷ� �߰� �켱���� 2
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
// ���� Ŭ����
class SubOper : public Oper {

public:
	SubOper() {
		this->prior = 2;
		// ����/������ ����/���������� �ʰ� ����ǹǷ� �߰� �켱���� 2
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
// ���� Ŭ����
class MulOper : public Oper {

public:
	MulOper() {
		this->prior = 3;
		// ����/�������� ����/�������� ���� ����ǹǷ� ����/�������� �� ���� �켱���� 3
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
// ������ Ŭ����
class DivOper : public Oper {

public:
	DivOper() {
		this->prior = 3;
		// ����/�������� ����/�������� ���� ����ǹǷ� ����/�������� �� ���� �켱���� 1
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
// ���� ��ȣ Ŭ����
class BraOper : public Oper {

public:
	BraOper() {
		this->prior = 1; 
		// ���� ǥ������� ��ȯ�ϱ� �վ� ��ȣ�� �ݴ� ��ȣ�� ���ö����� pop�Ǹ� �ȵǹǷ� ���� ���� �켱����
	}
	string getOper() {
		return NULL; // ���� ǥ������� ��ȣ�� ������ �����Ƿ� �ʱ�ȭ�� ��.
	}
	int getPrior() {
		return this->prior;
	}
	int calculrate(int a, int b) {
		return 0; // ���� ǥ��� ��꿡�� ��ȣ�� ������ �����Ƿ� �ʱ�ȭ�� ��.
	}
};
//
// �߻�Ŭ���� Oper�� �����Ǵ� �Լ��� ����߷� ���� �� �ֵ��� �ٸ� ������ ���� Ŭ���� 
// ��, �ѹ��� ��ü�� �����ϰ� �����ڴ� ���������� �ٲٱ� ���� �Ű������� �޾ƿ´�.
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

//������ ������ 2���� ���ڸ� �����ϸ� ������ Ȯ���� �ô´�.
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
// ������ �ùٸ��� Ȯ���ϸ� splitString�� �߶� ����
// ����ó���� ����ϵ� ���ü� �ִ� ������ ������ �Ʒ��� ���� �����Ͽ���.
// ��ȣ ���� / ���� ��ø / �ùٸ��� ���� ���� / �������� ����/����(+2 or 2-) /
//
bool IsValid(string aString, vector<string> *splitString) {
	
	//�޾ƿ� ���ڿ����� ���⸸ �����Ѵ�.
	aString.erase(remove(aString.begin(), aString.end(), ' '), aString.end());

	int strLength = aString.length();
	int nestingOper = 0; // ���� ��ø Ȯ�ο�, ��� ������ 2��ø�� ���� �߻�
	int invalidBracket = 0; // ��ȣ ���� Ȯ�ο�, ������ �������ų� ��ü �۾��� �������� 0�� �ƴϸ� ���� �߻�
	try {
		int i = 0;
		if (aString[i] == '*' || aString[i] == '/' || aString[i] == '+' || aString[i] == '-') {
			throw string("�������� ����");
		}
		for (i; i < strLength; i++) {
			//��ȣ ó��
			if (aString[i] == '(') {
				invalidBracket++;
				if (aString[i + 1] == '*' || aString[i + 1] == '/' || aString[i + 1] == '+' || aString[i + 1] == '-') {
					throw string("�������� ����");
				}
				splitString->push_back(aString.substr(i,1));
			}
			else if (aString[i] == ')') {
				if (--invalidBracket < 0) { 
					throw string("��ȣ ����");
				}
				else {
					if (aString[i - 1] == '*' || aString[i - 1] == '/' || aString[i - 1] == '+' || aString[i - 1] == '-') {
						throw string("�������� ����");
					}
				}
				splitString->push_back(aString.substr(i,1));
			}
			//��Ģ ���� ó��
			else if (aString[i] == '*' || aString[i] == '/' || aString[i] == '+' || aString[i] == '-') {
				if (++nestingOper >= 2) {
					throw string("���� ��ø");
				}
				splitString->push_back(aString.substr(i,1));
			}
			//2����, 16���� ó��
			else if (aString[i] == '0') {
				int j = 1;
				if (aString[i + j] == 'x') { // 16���� ó��
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
							throw string("�ùٸ��� ���� ���� or ��ȣ");
						}
					}
				}
				else if (aString[i + j] == 'b') { // 2���� ó��
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
							throw string("�ùٸ��� ���� ���� or ��ȣ");
						}
					}
				}
				else { //0���� �����ϴ� ���� ó��
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
							throw string("�ùٸ��� ���� ���� or ��ȣ");
						}
					}
				}
				
			}//10���� ó��
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
						throw string("�ùٸ��� ���� ���� or ��ȣ");
					}
				}
			}
			else if (aString[i] == ' ') {

			}
			else {
				throw string("�ùٸ��� ���� ����");
			}
		}
		if (aString[i - 1] == '*' || aString[i - 1] == '/' || aString[i - 1] == '+' || aString[i - 1] == '-') {
			throw string("�������� ����");
		}
		if (invalidBracket != 0) {
			throw string("��ȣ ����");
		}

	}
	catch (string exception) {
		cout << "Error- Error- Error- Error- Error- Error\n�Է� ������ �߰ߵǾ����ϴ�. : " << exception << "\n"<< endl;
		splitString->clear();
		return 0;
	}
	return 1;
}
// �Ʒ�InToPost, PostfixCalculate�ּ� ���۷����� ��ü�� ���� ���ؼ� �̸� �ʱ�ȭ�� �صд�. 
Oper* addOper = new AddOper();
Oper* subOper = new SubOper();
Oper* mulOper = new MulOper();
Oper* divOper = new DivOper();
Oper* braOper = new BraOper();

//
// ����ǥ��� -> ����ǥ������� �����Ŵ.
//
void InToPost(vector<string> *infixString) {
	//����� ���ڿ��� ����� ����.
	vector<string> *postfixString = new vector<string>;
	//�����ڸ� ������ ����.
	stack<Oper*> *aStack = new stack<Oper*>;
		
	for (int i = 0; i < infixString->size(); i++) {
		string data = infixString->at(i);
		if (data == "(") {
			//��ȣ�� �ݵ�� push�Ѵ�.
			aStack->push(braOper);
		}
		else if (data == "+") {
			// �켱���� �񱳽� null������ ������ ���ϱ� ���� ������� ������ �ٷ� push��
			if (!aStack->empty()) {
				// ���� ž�� �ִ� �켱�������� ũ�ų� ������ �켱������ �۾��������� pop
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
		//��ȣ �ݱⰡ �������� ��ȣ�� ���ö����� pop���ش�.
		else if (data == ")") {
			while (aStack->top()->getPrior() != 1) {
				postfixString->push_back(aStack->top()->getOper());
				aStack->pop();
			}
			aStack->pop();
		}
		else {
			// ���� �Է��� ������ �� ó�������Ƿ� ������ ������ ���� �ٷ� ��Ʈ���� �����Ѵ�.
			postfixString->push_back(data);
		}
	}
	//�Է��� ������ ������ ��� �����ڸ� pop�Ѵ�.
	while (!aStack->empty()) {
		postfixString->push_back(aStack->top()->getOper());
		aStack->pop();
	}
	//�Է¹��� �߰� ǥ��� �����͸� �����ϰ� ������.
	infixString->clear();
	for (int i = 0; i < postfixString->size(); i++) {
		infixString->push_back(postfixString->at(i));
	}

}

//
//����ǥ��� ���
// �긴�� ���� ���
//
int PostfixCalculate(vector<string>* postfixString) {
	//����� ���ڸ� ������ stack
	stack<int> *numStack = new stack<int>;
	// ����� �������� Calculator ��ü�� �����ϰ� �ǹ� ���� ������ �ʱ�ȭ���ش�.
	Calculate* calculator = new Calculator(0, 0, braOper);

	for(int i = 0; i < postfixString->size(); i++) {
		//�Է¹��� data ����
		string data = postfixString->at(i);
		int a, b;
		if (data == "+") {
			//����� ���ڸ� ���� ������ �� pop�ϰ�
			a  = numStack->top();
			numStack->pop();
			//���� ���ڸ� �������� �Ͽ� ���� ������ �����.
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
		//2���� 16������ ������ x,b�� ����� ���� ���ǹ�.
		else if(data.length() > 2){
			if (data.at(1) == 'x') {//16����
				//���ڴ� �ٷ� ���ؿ� �����Ѵ�.
				numStack->push((new HexNumberString(data))->getNum());
			}
			else if (data.at(1) == 'b') {//2����
				numStack->push((new BinNumberString(data))->getNum());
			}
			else {// 2�ڸ� �̻� 10����
				numStack->push((new DecNumberString(data))->getNum());
			}
		}//����ó���� �������Ƿ� �������� ���ڸ� 10����
		else {
			numStack->push((new DecNumberString(data))->getNum());
		}
	}//�������� ���ؿ� ���� ����� ��������Ƿ� top�ؼ� ��ȯ�Ѵ�.
	return numStack->top();
}


int main() {
	//���� �Է�
	string input;
	cout << "������ �Է��ϼ��� : ";
	getline(cin, input);

	// ����� ��Ʈ�� �迭/����
	vector<string>* splitString = new vector<string>;

	// �Է¹��� ���Ŀ� ������ �ֳ� Ȯ��
	while (!IsValid(input, splitString)) {
		cout << "������ �Է��ϼ��� : ";

		getline(cin, input);
	}
	// �Է¹��� ������ ����Ѵ�.
	cout <<  "\n�Էµ� ���� :  ";
	for (int i = 0; i < splitString->size(); i++) {
		cout << splitString->at(i) << " ";
	}
	cout << endl;
	//���� ǥ������� ��ȯ�Ѵ�.
	InToPost(splitString);

	// ��ȯ�� ������ ����Ѵ�.
	cout << "\n���� ǥ������� ��ȯ�� ���� : ";
	for (int i = 0; i < splitString->size(); i++) {
		cout << splitString->at(i) << " ";
	}
	cout << endl;

	//��� ��� ���
	cout <<"\n���� : " << PostfixCalculate(splitString) << endl;
	return 0;
}

