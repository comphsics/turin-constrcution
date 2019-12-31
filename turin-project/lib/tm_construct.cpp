/*This file implements tm_construct.h
*/
#include"tm_error.h"
#include"tm_construct.h"
#include"tm.h"
#include<iostream>
#include<vector>
#include<fstream>
turin_construct::turin_construct(std::string fileName):tm_file(fileName){
	TM=new t_m();
	states=new std::vector<std::string>();
	alphabets=new std::vector<char>();
	tape_alphabets=new std::vector<char>();
	final_states=new std::vector<std::string>();
	trans=new std::vector<tran_func>();
	start_state="";
	tape_number=0;
	blank_symbol=-1;
	hasLeadingLine=true;
	line_number=0;
	cur_tape_in_tran_func_number=0;
	file_name=fileName;
}
turin_construct::~turin_construct(){
	tm_file.close();
	delete states;
	delete alphabets;
	delete tape_alphabets;
	delete final_states;
}
t_m* turin_construct::run(){
	if(!tm_file){
	    error_handle(FILE_NOT_EXISTS,0,"file:"+file_name+" does not exists!");
	    return nullptr;
	}
	bool ok=construct();
	if(ok)
	    return TM;
	else
	    return nullptr;
}
char turin_construct::turin_construct::get_advanced_char(){
    char c;
    c=tm_file.get();
    if(c!=EOF){
            return c;
    }
    else{
        return -1;
    }
}
char turin_construct::get_no_advanced_char(){
    char c=tm_file.peek();
    if(c!=EOF){
            return c;
    }
    else{
        return -1;
    }
}
std::string turin_construct::turin_construct::get_advanced_str(){
         std::string tmpS="";
         char c;
         labelS:   c=get_advanced_char();
        switch(c){
            case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':
            case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':
            case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':
            case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':
            case  '0':case  '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '_':
            tmpS+=c;goto labelS;
            default:if(c!=-1)//If we see the end of file, we should not withdraw a char  because it cannot go over the end of file!
                        tm_file.seekg(-1,std::ios::cur);
                    break;
        }
        //It is possible that return null string
        return tmpS;
}
bool turin_construct::construct()
{
    bool flag=false;
    line_number=1;
    hasLeadingLine=true;
    do{
        beginLabel: char c=get_advanced_char();
        if(c==-1){
            file_end_handle();
            break;
        }
        switch(c){
            case ';':flag=comment_handle();if(flag==false) return false;break;
            case '#':{
                if(!hasLeadingLine){
                    error_handle(ELEMENT_MUST_LEADING_LINE,line_number,"# must lead a line!");
                    return false;
                }
                flag=element_handle();
                if(flag==false) 
                    return false;
                break;}
            case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':
            case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':
            case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':
            case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':
            case  '0':case  '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '_':
            {
                if(!hasLeadingLine){
                    error_handle(ELEMENT_MUST_LEADING_LINE,line_number,"transition function must lead at line!");
                    return false;
                }
                tm_file.seekg(-1,std::ios::cur);//Let this char be in the first state
                flag=tran_func_handle();
                if(flag==false) 
                    return false;
                break;
            }
            case ' ':case '\t':hasLeadingLine=false;goto beginLabel;
            case '\n':hasLeadingLine=true;line_number++;goto beginLabel;
            default:            
            std::string tmp;
            tmp.push_back(c);
            error_handle(ILLEGAL_INPUT,line_number,tmp+":illegal input!");return false;
        }
    }while(flag==true);
    flag=check_validity();
    if(flag){
        set_states();
        set_alphabets();
        set_tape_symbols();
        TM->set_start_state(start_state);
        TM->set_blank_symbol(blank_symbol);
        set_final_states();
        TM->set_num_of_tapes(tape_number);
        set_tran_func();
        return true;
    }else{
        return false;
    }
}
bool turin_construct::comment_handle()
{
    char c;
    do{
        c=get_advanced_char();
    }while(c!='\n'&&c!=-1);
    if(c=='\n')
        tm_file.seekg(-1,std::ios::cur);//If we see end of file,do not seekg,because eof is the last of the file.
    return true;
}
bool turin_construct::element_handle()
{
    char c=get_advanced_char();
    if(c=='Q'){
        if(get_no_advanced_char()==' '){
            get_advanced_char();
                if(get_no_advanced_char()=='='){
                    get_advanced_char();
                    if(get_no_advanced_char()==' '){
                        get_advanced_char();
                        if(get_no_advanced_char()=='\n'){
                            //Notice, we skip a line
                            line_number++;
                            get_advanced_char();
                            bool flag=states_handle();
                            return flag;
                        }else{
                            error_handle(ERROR_STATES_SYNTAX,line_number,"[end of line] must be the proper character!");
                            return false;
                        }
                    }else{
                        error_handle(ERROR_STATES_SYNTAX,line_number,"[space] must be the proper character!");
                        return false;
                    }
                }else{
                    error_handle(ERROR_STATES_SYNTAX,line_number,"= must be the proper character!");
                    return false;
                }
            }else{
                error_handle(ERROR_STATES_SYNTAX,line_number,"[space] must be the proper character!");
                return false;
            }
    }else if(c=='S'){
        if(get_no_advanced_char()==' '){
            get_advanced_char();
            if(get_no_advanced_char()=='='){
                get_advanced_char();
                if(get_no_advanced_char()==' '){
                    get_advanced_char();
                        bool flag=alphabets_handle();
                        return flag;
                }else{
                    error_handle(ERROR_ALPHABETS_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
            }else{
                    error_handle(ERROR_ALPHABETS_SYNTAX,line_number,"= must be the proper character!");
                    return false;
                }
        }else{
                    error_handle(ERROR_ALPHABETS_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
    }else if(c=='G'){
        if(get_no_advanced_char()==' '){
            get_advanced_char();
            if(get_no_advanced_char()=='='){
                get_advanced_char();
                if(get_no_advanced_char()==' '){
                    get_advanced_char();
                        bool flag=tape_alphabets_handle();
                        return flag;
                }else{
                    error_handle(ERROR_TAPE_ALPHABETS_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
            }else{
                    error_handle(ERROR_TAPE_ALPHABETS_SYNTAX,line_number,"= must be the proper character!");
                    return false;
                }
        }else{
                    error_handle(ERROR_TAPE_ALPHABETS_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
    }else if(c=='q'){
        if(get_no_advanced_char()=='0'){
            get_advanced_char();
            if(get_no_advanced_char()==' '){
                get_advanced_char();
            if(get_no_advanced_char()=='='){
                get_advanced_char();
                if(get_no_advanced_char()==' '){
                    get_advanced_char();
                    bool flag=set_start_state();
                    return flag;
                }else{
                    error_handle(ERROR_START_STATE_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
            }else{
                    error_handle(ERROR_START_STATE_SYNTAX,line_number,"= must be the proper character!");
                    return false;
                }
        }else{
                    error_handle(ERROR_START_STATE_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
        }else{
            error_handle(ERROR_START_STATE_SYNTAX,line_number,"0 must be the proper character!");
            return false;
        }
    }else if(c=='B'){
        if(get_no_advanced_char()==' '){
            get_advanced_char();
            if(get_no_advanced_char()=='='){
                get_advanced_char();
                if(get_no_advanced_char()==' '){
                    get_advanced_char();
                        bool flag=set_blank_symbol();
                        return flag;
                }else{
                    error_handle(ERROR_BLANK_SYMBOL_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
            }else{
                    error_handle(ERROR_BLANK_SYMBOL_SYNTAX,line_number,"= must be the proper character!");
                    return false;
                }
        }else{
                    error_handle(ERROR_BLANK_SYMBOL_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
    }else if(c=='F'){
        if(get_no_advanced_char()==' '){
            get_advanced_char();
            if(get_no_advanced_char()=='='){
                get_advanced_char();
                if(get_no_advanced_char()==' '){
                    get_advanced_char();
                        bool flag=final_states_handle();
                        return flag;
                }else{
                    error_handle(ERROR_FINAL_STATES_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
            }else{
                    error_handle(ERROR_FINAL_STATES_SYNTAX,line_number,"= must be the proper character!");
                    return false;
                }
        }else{
                    error_handle(ERROR_FINAL_STATES_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
    }else if(c=='N'){
        if(get_no_advanced_char()==' '){
            get_advanced_char();
            if(get_no_advanced_char()=='='){
                get_advanced_char();
                if(get_no_advanced_char()==' '){
                    get_advanced_char();
                        bool flag=set_tape_number();
                        return flag;
                }else{
                    error_handle(ERROR_TAPE_NUMBER_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
            }else{
                    error_handle(ERROR_TAPE_NUMBER_SYNTAX,line_number,"= must be the proper character!");
                    return false;
                }
        }else{
                    error_handle(ERROR_TAPE_NUMBER_SYNTAX,line_number,"[space] must be the proper character!");
                    return false;
                }
    }else{
        std::string tmp;
        tmp.push_back(c);
        error_handle(ILLEGAL_INPUT,line_number,tmp+":illegal input,must be Q,S,G,B,F,N,q0!");
        return false;
    }
}
void turin_construct::set_states()
{
    for(int i=0;i<states->size();i++)
    {
        TM->add_state(states->at(i));
    }
}
bool turin_construct::states_handle()
{
    hasLeadingLine=false;
    char c=get_advanced_char();
    if(c!='{'){
        error_handle(ERROR_STATES_SYNTAX,line_number,"{ must be the proper character!");
        return false;
    }
    std::string tmpS;
    while(true){
        tmpS=get_advanced_str();
        if(tmpS==""){
            std::string tmp;
            char c=get_advanced_char();
            if(c=='\n'){
                tmp.push_back('\\');
                tmp.push_back('n');
            }else if(c=='\t'){
                tmp.push_back('\\');
                tmp.push_back('t');
            }else if(c==' '){
                tmp+="[space]";
            }else if(c==-1){
                tmp+="[end of file]";
            }else{
                tmp.push_back(c);
            }
            error_handle(ERROR_STATES_SYNTAX,line_number,tmp+":illegal input!");
            return false;
        }else{
            char c=get_advanced_char();
            if(c==','){
                states->push_back(tmpS);
            }else if(c=='}'){
                states->push_back(tmpS);
                break;
            }else{
                std::string tmp;
                if(c=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c==' '){
                    tmp+="[space]";
                }else if(c==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c);
                }
                error_handle(ERROR_STATES_SYNTAX,line_number,tmp+":illegal input!");
                return false;
            }
        }
    }
    return true;
}
void turin_construct::set_alphabets()
{   
    for(int i=0;i<alphabets->size();i++)
    {
        TM->add_alphabet(alphabets->at(i));
    }
}
bool turin_construct::alphabets_handle()
{
    hasLeadingLine=false;
    char c1=get_advanced_char();
    if(c1!='{'){
        error_handle(ERROR_ALPHABETS_SYNTAX,line_number,"{ must be the proper character!");
        return false;
    }
    char c2;
    labelS:c1=get_advanced_char();
    c2=get_advanced_char();
    if((int)c1>=32&&(int)c1<=126&&c1!=' '&&c1!=','&&c1!=';'&&c1!='*'&&c1!='_'&&c1!='}'&&c1!='{'&&c2==','){
                alphabets->push_back(c1);
                goto labelS;
    }else if((int)c1>=32&&(int)c1<=126&&c1!=' '&&c1!=','&&c1!=';'&&c1!='*'&&c1!='_'&&c1!='}'&&c1!='{'&&c2=='}'){
            alphabets->push_back(c1);
            return true;
    }else{
        std::string tmp;
        if(c1=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c1=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c1==' '){
                    tmp+="[space]";
                }else if(c1==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c1);
                }
        if(c2=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c2=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c2==' '){
                    tmp+="[space]";
                }else if(c2==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c2);
                }
        if(c1==-1&&c2==-1){
            tmp="[end of file]";
        }
        error_handle(ERROR_ALPHABETS_SYNTAX,line_number,tmp+":illegal input!");
        return false;
    }
}
void turin_construct::set_tape_symbols()
{
    for(int i=0;i<tape_alphabets->size();i++)
    {
        TM->add_tape_alphabet(tape_alphabets->at(i));
    }
}
bool turin_construct::tape_alphabets_handle()
{
    hasLeadingLine=false;
    char c1=get_advanced_char();
    if(c1!='{'){
        error_handle(ERROR_TAPE_ALPHABETS_SYNTAX,line_number,"{ must be the proper character!");
        return false;
    }
    char c2;
    labelS:c1=get_advanced_char();
    c2=get_advanced_char();
    if((int)c1>=32&&(int)c1<=126&&c1!=' '&&c1!=','&&c1!=';'&&c1!='*'&&c1!='}'&&c1!='{'&&c2==','){
                tape_alphabets->push_back(c1);
                goto labelS;
    }else if((int)c1>=32&&(int)c1<=126&&c1!=' '&&c1!=','&&c1!=';'&&c1!='*'&&c1!='}'&&c1!='{'&&c2=='}'){
            tape_alphabets->push_back(c1);
            return true;
    }else{
        std::string tmp;
        if(c1=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c1=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c1==' '){
                    tmp+="[space]";
                }else if(c1==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c1);
                }
                if(c2=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c2=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c2==' '){
                    tmp+="[space]";
                }else if(c2==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c2);
                }
        if(c1==-1&&c2==-1){
            tmp="[end of file]";
        }
        error_handle(ERROR_TAPE_ALPHABETS_SYNTAX,line_number,tmp+":illegal input!");
        return false;
    }
}
bool turin_construct::set_start_state()
{
    hasLeadingLine=false;
    std::string tmpS;
    tmpS=get_advanced_str();
    if(tmpS==""){
        char c=get_advanced_char();
        std::string tmp;
        if(c=='\n'){
            tmp+="\\n";
        }else if(c=='\t'){
            tmp+="\\t";
        }else if(c==' '){
            tmp+="[space]";
        }else if(c==-1){
            tmp+="[end of file]";
        }else{
            tmp.push_back(c);
        }
        error_handle(ERROR_START_STATE_SYNTAX,line_number,tmp+":illegal input!");
        return false;
    }else{
        start_state=tmpS;
        return true;
    }
 
}
bool turin_construct::set_blank_symbol()
{
    hasLeadingLine=false;
    char c1=get_advanced_char();
    if((int)c1>=32&&(int)c1<=126&&c1!=' '&&c1!=','&&c1!=';'&&c1!='*'&&c1!='}'&&c1!='{')
    {
        blank_symbol=c1;
        char c2=get_no_advanced_char();
        if(c2==' '||c2=='\t'||c2=='\n'||c2==-1){
           if(c2!=-1){
                tm_file.clear();      //If we see end of file,do not seekg,because eof is the last of the file.
                tm_file.seekg(0,std::ios::cur);//Do not move to the next not(-1,ios::cur) because peek()!
           }
            return true;
        }else{
            error_handle(ERROR_BLANK_SYMBOL_SYNTAX,line_number,"[space] must be the proper character!");
            return false;
        }
    }else{
        std::string tmp;
        if(c1=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c1=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c1==' '){
                    tmp+="[space]";
                }else if(c1==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c1);
                }
        error_handle(ERROR_BLANK_SYMBOL_SYNTAX,line_number,tmp+":illegal input!");
            return false;
    }
}
void turin_construct::set_final_states()
{
    for(int i=0;i<final_states->size();i++)
    {
        TM->add_final_state(final_states->at(i));
    }
}
bool turin_construct::final_states_handle()
{
    hasLeadingLine=false;
    char c=get_advanced_char();
    if(c!='{'){
        error_handle(ERROR_FINAL_STATES_SYNTAX,line_number,"{ must be the proper character!");
        return false;
    }
    std::string tmpS;
    while(true){
        tmpS=get_advanced_str();
        if(tmpS==""){
            std::string tmp;
            char c=get_advanced_char();
            if(c=='\n'){
                tmp.push_back('\\');
                tmp.push_back('n');
            }else if(c=='\t'){
                tmp.push_back('\\');
                tmp.push_back('t');
            }else if(c==' '){
                tmp+="[space]";
            }else if(c==-1){
                tmp+="[end of file]";
            }else{
                tmp.push_back(c);
            }
            error_handle(ERROR_FINAL_STATES_SYNTAX,line_number,tmp+":illegal input!");
            return false;
        }else{
            char c=get_advanced_char();
            if(c==','){
                final_states->push_back(tmpS);
            }else if(c=='}'){
                final_states->push_back(tmpS);
                break;
            }else{
                std::string tmp;
                if(c=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c==' '){
                    tmp+="[space]";
                }else if(c==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c);
                }
                error_handle(ERROR_FINAL_STATES_SYNTAX,line_number,tmp+":illegal input!");
                return false;
            }
        }
    }
    return true;
}
bool turin_construct::set_tape_number()
{  
    hasLeadingLine=false;
    int num=0;
    labelS:char c=get_advanced_char();
    if(c>='0'&&c<='9')          //Notice the type!Automated cast
    {
        num=num*10+c-48;
        goto labelS;
    }else if(c==' '||c=='\t'||c=='\n'||c==-1){    
        if(c!=-1){
            tm_file.seekg(-1,std::ios::cur);
        }
        if(num!=0){
            tape_number=num;
            return true;
        }   
        std::string tmp;
        if(c=='\n'){
                    tmp.push_back('\\');
                    tmp.push_back('n');
                }else if(c=='\t'){
                    tmp.push_back('\\');
                    tmp.push_back('t');
                }else if(c==' '){
                    tmp+="[space]";
                }else if(c==-1){
                    tmp+="[end of file]";
                }else{
                    tmp.push_back(c);
                }
                error_handle(ERROR_TAPE_NUMBER_SYNTAX,line_number,tmp+": illegal input!");
                return false;
    }else{
        std::string info;
        info.push_back(c);
        error_handle(ERROR_TAPE_NUMBER_SYNTAX,line_number,info+": illegal input!");
        return false;
    }
}
void turin_construct::error_handle(ERROR_TYPE type,int line,std::string assist)
{
    tm_error(type,line,assist);
}
void turin_construct::file_end_handle()
{
	//If you want to deal with file end, please insert your code here
}
bool turin_construct::tran_func_handle(){
        std::string tmpS=get_advanced_str();
        if(tmpS==""){
            error_handle(TRANSITION_FUNCTION_ERROR,line_number,"lack of state!");
            return false;
        }
        if(get_advanced_char()!=' '){
            error_handle(TRANSITION_FUNCTION_ERROR,line_number,"invalid syntax!");
            return false;
        }
        std::vector<char>c1,c2;
        char input;
        std::vector<DIRECTION>d;
        while(true){
            input=get_advanced_char();
            if((int)input>=32&&(int)input<=126&&input!=' '&&input!=','&&input!=';'&&input!='}'&&input!='{'){
                c1.push_back(input);
            }else if(input==' '){
                break;
            }else{
                error_handle(TRANSITION_FUNCTION_ERROR,line_number,"invalid syntax!");
                return false;
            }
        }
        while(true){
            input=get_advanced_char();
            if((int)input>=32&&(int)input<=126&&input!=' '&&input!=','&&input!=';'&&input!='}'&&input!='{'){
                c2.push_back(input);
            }else if(input==' '){
                break;
            }else{
                error_handle(TRANSITION_FUNCTION_ERROR,line_number,"invalid syntax!");
                return false;
            }
        }
        while(true){
            input=get_advanced_char();
            if(input=='r'){
                d.push_back(RIGHT);
            }else if(input=='l'){
                d.push_back(LEFT);
            }else if(input=='*'){
                d.push_back(STAY);
            }else if(input==' '){
                break;
            }else{
                error_handle(TRANSITION_FUNCTION_ERROR,line_number,"invalid syntax!");
                return false;
            }
        }
        std::string tmpS2=get_advanced_str();
        if(tmpS2==""){
            error_handle(TRANSITION_FUNCTION_ERROR,line_number,"lack of state!");
            return false;
        }
        if(get_no_advanced_char()!=' '&&get_no_advanced_char()!='\t'&&get_no_advanced_char()!='\n'&&get_no_advanced_char()!=-1){
            std::string tmp;
            tmp.push_back(get_advanced_char());
            error_handle(TRANSITION_FUNCTION_ERROR,line_number,tmp+"illegal input!");
            return false;
        }
        if(c1.size()!=c2.size()||c1.size()!=d.size()){
            error_handle(TRANSITION_FUNCTION_ERROR,line_number,"state's number is not equal to direction's number or the first state's number are not equal to the second state's number!");
            return false;
        }
        if(cur_tape_in_tran_func_number==0){
            cur_tape_in_tran_func_number=c1.size();
        }
        if(c1.size()!=cur_tape_in_tran_func_number){
            std::string tmp;
            tmp+="previous:"+int_to_string(cur_tape_in_tran_func_number);
            tmp+=" current:"+int_to_string(c1.size());
            error_handle(TRANSITION_FUNCTION_ERROR,line_number,"the previous tape number is not equal to the current number,"+tmp+"!");
            return false;
        }
        tran_func tmpTran;
        tmpTran.from=tmpS;
        tmpTran.to=tmpS2;
        tmpTran.from_symbols=new char[c1.size()];//be careful of C_str style string
        tmpTran.to_symbols=new char[c1.size()];
        tmpTran.directions=new DIRECTION[c1.size()];
        for(int i=0;i<c1.size();i++){
            tmpTran.from_symbols[i]=c1[i];
            tmpTran.to_symbols[i]=c2[i];
            tmpTran.directions[i]=d[i];
        }
        trans->push_back(tmpTran);
        return true;
}
void turin_construct::set_tran_func(){
    for(int i=0;i<trans->size();i++){
        //May '*' symbol we must deal in the tm.cpp
        TM->add_transition_function(trans->at(i).from,trans->at(i).from_symbols,trans->at(i).to_symbols,trans->at(i).directions,trans->at(i).to);
    }
}
bool turin_construct::check_validity()
{
    bool flag=false;
    for(int i=0;i<states->size();i++){
        for(int j=i+1;j<states->size();j++){
            if(states->at(i)==states->at(j)){
                error_handle(STATES_REPEAT,0,states->at(i)+" is repeated!");
                return false;
            }
        }
    }
    for(int i=0;i<alphabets->size();i++){
        for(int j=i+1;j<alphabets->size();j++){
            if(alphabets->at(i)==alphabets->at(j)){
                std::string tmp;
                tmp.push_back(alphabets->at(i));
                error_handle(ALPHABET_REPEAT,0,tmp+" is repeated!");
                return false;
            }
        }
    }
    for(int i=0;i<tape_alphabets->size();i++){
        for(int j=i+1;j<tape_alphabets->size();j++){
            if(tape_alphabets->at(i)==tape_alphabets->at(j)){
                std::string tmp;
                tmp.push_back(tape_alphabets->at(i));
                error_handle(TAPE_ALPHABET_REPEAT,0,tmp+" is repeated!");
                return false;
            }
        }
    }
    for(int i=0;i<final_states->size();i++){
        for(int j=i+1;j<final_states->size();j++){
            if(final_states->at(i)==final_states->at(j)){
                error_handle(FINAL_STATE_REPEAT,0,final_states->at(i)+" is repeated!");
                return false;
            }
        }
    }
    for(int i=0;i<alphabets->size();i++){
        if(blank_symbol==alphabets->at(i)){
            std::string info="alphabet ";
            info.insert(info.end(),alphabets->at(i));
            error_handle(BLANK_SYMBOL_IN_ALPHABET,0,info+" is the same with the blank symbol \n"+"blank symbol is "+blank_symbol);
            return false;
        }
    }
    for(int i=0;i<alphabets->size();i++){
        flag=false;
        char c=alphabets->at(i);
        for(int j=0;j<tape_alphabets->size();j++)
        {
            if(c==tape_alphabets->at(j)){
                flag=true;
                break;
            }
        }
        if(flag==false)
        {
            std::string tmp;
            tmp.push_back(alphabets->at(i));
            error_handle(ALPHABETS_NOT_IN_TAPE_ALPHABETS,0,tmp+" is not in tape alphabets!");
            return false;
        }
    }
    for(int i=0;i<final_states->size();i++){
        flag=false;
        std::string c=final_states->at(i);
        for(int j=0;j<states->size();j++)
        {
            if(c==states->at(j)){
                flag=true;
                break;
            }
        }
        if(flag==false)
        {
            error_handle(FINAL_STATES_NOT_IN_STATES,0,final_states->at(i)+" is not in states!");
            return false;
        }
    }        
    flag=false;
    for(int i=0;i<states->size();i++)
    {
    if(start_state==states->at(i)){
            flag=true;
            break;
        }
    }
    if(flag==false){
        error_handle(START_STATE_NOT_IN_STATES,0,start_state+" is not in states!");
        return false;
    }
    flag=false;
    for(int i=0;i<tape_alphabets->size();i++)
    {
        if(tape_alphabets->at(i)==blank_symbol){
            flag=true;
            break;
        }
    }
    if(flag==false){
        std::string info="blank symbol ";
        info.insert(info.end(),blank_symbol);
        error_handle(BLANK_SYMBOL_NOT_IN_TAPE_ALPHABETS,0,info+" is not in tape alphabets!");
        return false;
    }
    if(states->size()==0||tape_alphabets->size()==0||alphabets->size()==0||blank_symbol==-1||start_state==""||final_states->size()==0||tape_number==0)
    {
        error_handle(LACK_OF_ELEMENTS,0,"lack of necessary elements!");
        return false;
    }
    if(cur_tape_in_tran_func_number!=tape_number){
        std::string tmp;
        tmp+="tape number by the transition func is "+int_to_string(cur_tape_in_tran_func_number);
        tmp+=",while the true tape number is "+int_to_string(tape_number)+'!';
        error_handle(ERROR_NUMBER_IN_TRANS,0,tmp);
        return false;
    }
    bool flag1=false;
    bool flag2=false,flag3=false,flag4=false;
    for(int i=0;i<trans->size();i++){
        for(int j1=0;j1<states->size();j1++){
            if(trans->at(i).from==states->at(j1)){
                flag1=true;
            }
            if(trans->at(i).to==states->at(j1)){
                flag2=true;
            }
        }
        if(flag1==false||flag2==false){
            std::string tmp;
            tmp+="the state:"+trans->at(i).from;
            tmp+=" or "+trans->at(i).to;
            error_handle(TRAN_STATE_NOT_IN_STATES,0,tmp+" is(are) not in states!");
            return false;
        }
        flag1=flag2=false;  
        for(int j2=0;j2<tape_number;j2++){
            for(int k=0;k<tape_alphabets->size();k++){
                if(trans->at(i).from_symbols[j2]=='*'||trans->at(i).from_symbols[j2]==tape_alphabets->at(k)){
                    flag3=true;
                }
                if(trans->at(i).to_symbols[j2]=='*'||trans->at(i).to_symbols[j2]==tape_alphabets->at(k)){
                    flag4=true;
                }
            }
            if(flag3==false||flag4==false){
                std::string tmp;
                tmp+="the symbol:";
                tmp.push_back(trans->at(i).from_symbols[j2]);
                tmp+=" or ";
                tmp.push_back(trans->at(i).to_symbols[j2]);
                error_handle(TRAN_SYMBOL_NOT_IN_TAPE_ALPHABETS,0,tmp+" is(are) not in tape alphabets!");
                return false;
            }
            flag3=false;
            flag4=false;
        }
        flag3=flag4=false;  
    }
    
    return true;
}
