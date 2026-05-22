#include <iostream>
#include <string>
#define MAX_SIZE 255
using namespace std;

int SIZE_TABLE;

typedef struct STATE{//STATE라는 구조체 정의
	char state_name;
	bool final_state;
	STATE* a_state;
	STATE* b_state;
	STATE* Ramda_state1;
	STATE* Ramda_state2;
}STATE;

typedef STATE* pState;

unsigned int isp_exc(char token) //중위수식을 후위수식으로 변환할 때 stack에 있는 문자에 대해 정수값으로 반환
{
	if(token == '|')
		return 1;
	else if(token == '.')
		return 2;
	else if(token == '(')
		return 0;
	else
		return 0;
}

unsigned int icp_exc(char token) //stack에 있는 문자와 비교 하기 위해 입력받은 값에 대해 정수값으로 반환
{
	if(token == '|')
		return 1;
	else if(token == '.')
		return 2;
	else if(token == '(')
		return 3;
	else
		return 0;
}

string postfix(string RE) //중위수식을 후위수식으로 변환
{
	unsigned int icp, isp;
	string PostRE;//후위식 저장변수
	char stack[MAX_SIZE];
	stack[0] = 'n';
	int TOP = 0, P_Index = 0;
	for(unsigned int R_Index = 0; R_Index <= RE.size(); R_Index++){
		if((RE[R_Index] == 'a') || (RE[R_Index] == 'b'))
			PostRE = PostRE + RE[R_Index];
			//PostRE.push_back(RE[R_Index]);
		else if((RE[R_Index] == '|') || (RE[R_Index] == '.')){
			isp = isp_exc(stack[TOP]);
			icp = icp_exc(RE[R_Index]);
			if(isp <= icp){
				stack[++TOP] = RE[R_Index];
				isp = isp_exc(stack[TOP]);
			}
			else{
				while(isp > icp){
					PostRE = PostRE + stack[TOP--];
					//PostRE.push_back(stack[TOP--]);
					isp = isp_exc(stack[TOP]);
				}
				stack[++TOP] = RE[R_Index];
				isp = isp_exc(stack[TOP]);
			}
		}
		else if(RE[R_Index] == '('){
			stack[++TOP] = RE[R_Index];
		}
		else if(RE[R_Index] == ')'){
			while(stack[TOP] != '(')
				PostRE = PostRE + stack[TOP--];
				//PostRE.push_back(stack[TOP--]);
			TOP--;
		}
		else if(RE[R_Index] == '*'){
			PostRE = PostRE + '*';
			//PostRE.push_back('*');
		}
		else if(RE[R_Index] == '\0'){
			while(TOP != 0)
				PostRE = PostRE + stack[TOP--];
				//PostRE.push_back(stack[TOP--]);

		}
	}
	cout << "입력한 정규표현의 후위식 변환"<<endl;
	cout <<"=>";
	for(unsigned int INDEX = 0; INDEX <= PostRE.size(); INDEX++)
		cout << PostRE[INDEX];
	cout << endl;
	return PostRE;
}

char** NFA_Table(pState nfa, int NAME, char start) //2차원배열 생성 및 nfa의 Table를 만듦
{
	int num = 65, Index1 = 0;
	pState temp = nfa;
	char **List = new char*[NAME-65];
	for(int i = 0; i < NAME-65; i++)
		List[i] = new char[6];
	while(num < NAME){
		if((char)num == temp->state_name){	
			List[Index1][0] = temp->state_name;
			if(temp->a_state != NULL)
				List[Index1][1] = temp->a_state->state_name;
			else
				List[Index1][1] = '0';
			if(temp->b_state != NULL)
				List[Index1][2] = temp->b_state->state_name;
			else
				List[Index1][2] = '0';
			if(temp->Ramda_state1 != NULL)
				List[Index1][3] = temp->Ramda_state1->state_name;
			else
				List[Index1][3] = '0';
			if(temp->Ramda_state2 != NULL)
				List[Index1][4] = temp->Ramda_state2->state_name;
			else
				List[Index1][4] = '0';
			if(temp->state_name == start)
				List[Index1][5] = 'S';
			else if(temp->final_state == true)
				List[Index1][5] = 'F';
			else
				List[Index1][5] = '0';
			
			if(temp->a_state != NULL){
				temp = temp->a_state;}
			else if(temp->b_state != NULL){
				temp = temp->b_state;}
			else if((temp->Ramda_state1 != NULL) && (temp->Ramda_state1->state_name >= (char)num)){
				temp = temp->Ramda_state1;}
			else if((temp->Ramda_state2 != NULL) && (temp->Ramda_state2->state_name >= (char)num)){
				temp = temp->Ramda_state2;}
			else if(temp->final_state == true){
				temp = nfa;}
			else if(temp->Ramda_state1 != NULL){
				temp = temp->Ramda_state1;}
			else if(temp->Ramda_state2 != NULL){
				temp = temp->Ramda_state2;}
			num++;
			Index1++;
		}
		else{
			if(temp->a_state != NULL){
				temp = temp->a_state;}
			else if(temp->b_state != NULL){
				temp = temp->b_state;}
			else if((temp->Ramda_state1 != NULL) && (temp->Ramda_state1->state_name >= (char)num)){
				temp = temp->Ramda_state1;}
			else if((temp->Ramda_state2 != NULL) && (temp->Ramda_state2->state_name >= (char)num)){
				temp = temp->Ramda_state2;}
			else if(temp->final_state == true){
				temp = nfa;}
			else if(temp->Ramda_state1 != NULL){
				temp = temp->Ramda_state1;}
			else if(temp->Ramda_state2 != NULL){
				temp = temp->Ramda_state2;}
		}
	}
	return List;
}
char NFA_Print(pState nfa, int num) //STATE의 이름을 오름차순 순으로 출력 및 Final State의 이름을 반환
{
	int name = 65;
	pState temp = nfa, final;
	cout << "\n      < NFA TABLE>\n";
	cout << "┏━━━━━━━━━━━━┓\n";
	cout << "┃   ┃ a ┃ b ┃   λ    ┃\n";
	cout << "┣━━━━━━━━━━━━┫\n";
	while(name < num){
		if(temp->state_name == (char) name){
			cout << "┃ " << temp->state_name << " ┃ ";
			if(temp->a_state != NULL)
				cout << temp->a_state->state_name;
			else
				cout << '0';
			cout << " ┃ ";
			if(temp->b_state != NULL)
				cout << temp->b_state->state_name;
			else
				cout << '0';
			cout << " ┃ ";
			if(temp->Ramda_state1 != NULL)
				cout << temp->Ramda_state1->state_name;
			else
				cout << '0';
			cout << " ┃ ";
			if(temp->Ramda_state2 != NULL)
				cout << temp->Ramda_state2->state_name;
			else
				cout << '0';
			cout << "  ┃ \n";

			if(temp->a_state != NULL)
				temp = temp->a_state;
			else if(temp->b_state != NULL)
				temp = temp->b_state;
			else if((temp->Ramda_state1 != NULL) && (temp->Ramda_state1->state_name >= (char)name))
				temp = temp->Ramda_state1;
			else if((temp->Ramda_state2 != NULL) && (temp->Ramda_state2->state_name >= (char)name))
				temp = temp->Ramda_state2;
			else if(temp->final_state == true)
				temp = nfa;
			else if(temp->Ramda_state1 != NULL)
				temp = temp->Ramda_state1;
			else if(temp->Ramda_state2 != NULL)
				temp = temp->Ramda_state2;
			name++;
		}
		else{
			if(temp->a_state != NULL)
				temp = temp->a_state;
			else if(temp->b_state != NULL)
				temp = temp->b_state;
			else if((temp->Ramda_state1 != NULL) && (temp->Ramda_state1->state_name >= (char)name))
				temp = temp->Ramda_state1;
			else if((temp->Ramda_state2 != NULL) && (temp->Ramda_state2->state_name >= (char)name))
				temp = temp->Ramda_state2;
			else if(temp->final_state == true)
				temp = nfa;
			else if(temp->Ramda_state1 != NULL)
				temp = temp->Ramda_state1;
			else if(temp->Ramda_state2 != NULL)
				temp = temp->Ramda_state2;
		}
		if(temp->final_state == true)
			final = temp;
	}
	cout << "┗━━━━━━━━━━━━┛\n";
	cout << "start state : " << nfa->state_name << endl;
	cout << "final state : " << final->state_name << endl;
	cout << "___________________________________\n";
	SIZE_TABLE = ((int)final->state_name)-65;
	return final->state_name;
}
void DFA_Print(char **list, int name) //dfa의 내용을 출력
{
	cout << " \n   < DFA TABLE >\n";
	cout << "┏━━━━━━━┓\n";
	cout << "┃    a ┃ b    ┃ \n";
	cout << "┣━━━━━━━┫\n";

	for(int low = 0; low < name-65; low++){
		if(list[low][0] == '0')
			continue;
		cout << "┃  " << list[low][0] << " ┃ " << list[low][1] << " ┃ " << list[low][2] << " ┃";
		if(list[low][5] != '0')
			cout << list[low][5] << endl;
		else
			cout << endl;
	}
	cout << "┗━━━━━━━┛\n";
	cout << "___________________________________\n"<<endl;
	cout <<endl;
}

char** NFAtoDFA(char **list, int name) //NFA_Table에서 만들어진 Table을 이용하여 DFA로 변환
{
	int NAME = 65, low, temp;
	for(low = 0; low < name-65; low++){
		if(list[low][0] != '0'){
			for(temp = 0; temp < name-65; temp++){
				if(temp == low)
					continue;
				else if(list[temp][3] == list[low][0] || list[temp][4] == list[low][0]){
					if(list[temp][3] == list[low][0] && list[temp][4] != '0'){
						if(list[low][3] == '0')
							list[low][3] = list[temp][4];
						else if(list[low][4] == '0')
							list[low][4] = list[temp][4];
					}
					else if(list[temp][4] == list[low][0] && list[temp][3] != '0'){
						if(list[low][3] == '0')
							list[low][3] = list[temp][3];
						else if(list[low][4] == '0')
							list[low][4] = list[temp][3];
					}
					if(list[temp][1] != '0')
						list[low][1] = list[temp][1];
					if(list[temp][2] != '0')
						list[low][2] = list[temp][2];
					if(list[temp][5] != '0')
						list[low][5] = list[temp][5];

					for(int i = 0; i < name - 65; i++){
						for(int j = 1; j < 5; j++){
							if(list[i][j] == list[temp][0])
								list[i][j] = list[low][0];
						}
					}
					list[temp][0] = '0';
					list[temp][1] = '0';
					list[temp][2] = '0';
					list[temp][3] = '0';
					list[temp][4] = '0';
					list[temp][5] = '0';
					temp = 0;
				}
			}
		}
	}
	DFA_Print(list, name);

	return list;
}
char** REtoNFA(string RE) //후위식으로 변환된 정규표현을 nfa로 변환
{
	string PostRE = postfix(RE);
	pState stack[MAX_SIZE];
	int TOP = -1, NAME = 65, label = 0;
	char start, final;
	for(unsigned int Index = 0; Index <= PostRE.size(); Index++){
		if(PostRE[Index] == 'a'){
			pState temp1 = new STATE;
			pState temp2 = new STATE;
			temp1->a_state = temp2;
			temp1->b_state = NULL;
			temp1->final_state = false;
			temp1->Ramda_state1 = NULL;
			temp1->Ramda_state2 = NULL;
			temp1->state_name = NAME++;
			temp2->a_state = NULL;
			temp2->b_state = NULL;
			temp2->final_state = true;
			temp2->Ramda_state1 = NULL;
			temp2->Ramda_state2 = NULL;
			temp2->state_name = NAME++;
			stack[++TOP] = temp1;
		}
		else if(PostRE[Index] == 'b'){
			pState temp1 = new STATE;
			pState temp2 = new STATE;
			temp1->a_state = NULL;
			temp1->b_state = temp2;
			temp1->final_state = false;
			temp1->Ramda_state1 = NULL;
			temp1->Ramda_state2 = NULL;
			temp1->state_name = NAME++;
			temp2->a_state = NULL;
			temp2->b_state = NULL;
			temp2->final_state = true;
			temp2->Ramda_state1 = NULL;
			temp2->Ramda_state2 = NULL;
			temp2->state_name = NAME++;
			stack[++TOP] = temp1;
		}
		else if(PostRE[Index] == '|'){
		pState temp1 = new STATE;
			pState temp2 = new STATE;
			pState temp3 = stack[TOP--];
			pState temp4 = stack[TOP--];
			temp1->a_state = NULL;
			temp1->b_state = NULL;
			temp2->a_state = NULL;
			temp2->b_state = NULL;
			temp1->final_state = false;
			temp2->final_state = true;
			temp1->Ramda_state1 = temp4;
			temp1->Ramda_state2 = temp3;
			temp1->state_name = NAME++;
			temp2->state_name = NAME++;
			temp2->Ramda_state1 = NULL;
			temp2->Ramda_state2 = NULL;
			while(temp4->final_state == false){
				if(temp4->a_state != NULL)
					temp4 = temp4->a_state;
				else if(temp4->b_state != NULL)
					temp4 = temp4->b_state;
				else if(temp4->Ramda_state1 != NULL)
					temp4 = temp4->Ramda_state1;
				else if(temp4->Ramda_state2 != NULL)
					temp4 = temp4->Ramda_state2;
			}
			temp4->final_state = false;
			if(temp4->Ramda_state1 == NULL)
				temp4->Ramda_state1 = temp2;
			else
				temp4->Ramda_state2 = temp2;
			while(temp3->final_state == false){
				if(temp3->a_state != NULL)
					temp3 = temp3->a_state;
				else if(temp3->b_state != NULL)
					temp3 = temp3->b_state;
				else if(temp3->Ramda_state1 !=NULL)
					temp3 = temp3->Ramda_state1;
				else if(temp3->Ramda_state2 != NULL)
					temp3 = temp3->Ramda_state2;
			}
			temp3->final_state = false;
			if(temp3->Ramda_state1 == NULL)
				temp3->Ramda_state1 = temp2;
			else
				temp3->Ramda_state2 = temp2;
			stack[++TOP] = temp1;
		}
		else if(PostRE[Index] == '.'){
			pState temp1 = stack[TOP--];
			pState temp2 = stack[TOP--];
			pState temp_LState = temp2;
			while(temp_LState->final_state == false){
				if(temp_LState->a_state != NULL)
					temp_LState = temp_LState->a_state;
				else if(temp_LState->b_state != NULL)
					temp_LState = temp_LState->b_state;
				else if(temp_LState->Ramda_state1 != NULL)
					temp_LState = temp_LState->Ramda_state1;
				else if(temp_LState->Ramda_state2 != NULL)
					temp_LState = temp_LState->Ramda_state2;
			}
			if(temp_LState->Ramda_state1 == NULL)
				temp_LState->Ramda_state1 = temp1;
			else
				temp_LState->Ramda_state2 = temp1;
			temp_LState->final_state = false;
			stack[++TOP] = temp2;
		}
		else if(PostRE[Index] == '*'){
			pState temp1 = new STATE;
			pState temp2 = new STATE;
			pState temp3 = stack[TOP--];
			temp1->a_state = NULL;
			temp1->b_state = NULL;
			temp2->a_state = NULL;
			temp2->b_state = NULL;
			temp1->final_state = false;
			temp2->final_state = true;
			temp1->Ramda_state1 = temp3;
			temp1->Ramda_state2 = temp2;
			temp2->Ramda_state1 = temp1;
			temp2->Ramda_state2 = NULL;
			temp1->state_name = NAME++;
			temp2->state_name = NAME++;
			while(temp3->final_state == false){
				if(temp3->a_state != NULL)
					temp3 = temp3->a_state;
				else if(temp3->b_state != NULL)
					temp3 = temp3->b_state;
				else if(temp3->Ramda_state1 != NULL)
					temp3 = temp3->Ramda_state1;
				else if(temp3->Ramda_state2 != NULL)
					temp3 = temp3->Ramda_state2;
			}
			temp3->Ramda_state1 = temp2;
			temp3->Ramda_state2 = NULL;
			temp3->final_state = false;
			stack[++TOP] = temp1;
		}
	}
	pState temp = stack[TOP--];
	start = temp->state_name;
	final = NFA_Print(temp, NAME);

	return NFAtoDFA(NFA_Table(temp, NAME, start), NAME);
}


bool accept(char **list, string str, int size) //State가 Final일 경우 true를 리턴하고 그렇지 않을 경우 false를 리턴
{
	unsigned int index;
	char nst;
	for(int i = 0; i <= size; i++){
		if(list[i][5] == 'S')
			nst = list[i][0];
	}
	for(index = 0; index < str.size(); index++){
		for(int k = 0; k <= size; k++){
			if(list[k][0] == nst){
				if(str[index] == 'a')
					nst = list[k][1];
				else if(str[index] == 'b')
					nst = list[k][2];
				break;
			}
		}
	}
	for(int label = 0; label <= size; label++){
		if(list[label][0] == nst)
			if(list[label][5] == 'F')
				return true;
			else
				return false;
	}
	return false;
}

int main() //메인함수
{
	string RE, str;
	cout << "a,b로 구성된 정규표현을 입력해주세요"<< endl;
	cout << " ━━━━━━━━━━━" << endl;
	cout << "┃ AND 연산자   = .   ┃" << endl;
	cout << "┃ OR  연산자   = |   ┃" << endl;
	cout << "┃ *   연산자   = *   ┃" << endl;
	cout << " ━━━━━━━━━━━" << endl;
	cout <<"=>";
	cin >> RE;
	
		
	cout << endl;
	char **list = REtoNFA(RE);
	while(str[0] != 'q'){
		if(str[0] != 'q'){
			cout << "테스트할 string을 입력해주세요. ex) abbbb"<<endl;
			cout <<"=>";
			cin >> str;	
			if(accept(list, str, SIZE_TABLE))
				cout << "Accept 되었습니다.\n";
			else
				cout << "Reject 되었습니다.\n";
		}
	}
	
	return 0;
}