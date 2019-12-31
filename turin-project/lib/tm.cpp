/*This file implements tm.h*/
#include"tm.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
//We specify '\t' with four spaces
int t_m::find_index_of_state(std::string s){
    for(int i=0;i<states.size();i++)
    {
        if(states[i]==s){
       	    return i;
    	}
    }
    return -1;
}
int t_m::find_index_of_tape_symbol(char c){
    //do not forget the wildcard '*'
    if(c=='*'){
    	return -1;
    }
    for(int i=0;i<tape_alphabets.size();i++){
        if(tape_alphabets[i]==c){
            return i;
        }
    }
    return -1;
}
t_m::t_m()
{
	states.clear();
	alphabets.clear();
	tape_alphabets.clear();
	start_state=-1;
	final_states.clear();
	num_of_tapes=0;
	tran_func.clear();
	num_of_alphabets=0;
	num_of_states=0;
	num_of_final_states=0;
	num_of_tape_alphabets=0;
	num_of_tran_func=0;
}
int t_m::get_num_of_tapes(){
	return num_of_tapes;
}
void t_m::set_num_of_tapes(int v)
{
	num_of_tapes=v;
}
std::string t_m::get_start_state(){
	return states[start_state];
}
void t_m::add_state(std::string s){
	states.push_back(s);
	num_of_states++;
}
void t_m::add_alphabet(char a){
	alphabets.push_back(a);
	num_of_alphabets++;
}
void t_m::add_tape_alphabet(char a){
	tape_alphabets.push_back(a);
	num_of_tape_alphabets++;
}
void t_m::set_start_state(std::string s){
	int index=find_index_of_state(s);
	start_state=index;
}
void t_m::add_final_state(std::string f){
	int index=find_index_of_state(f);
	final_states.push_back(index);
	num_of_final_states++;
}
void t_m::add_transition_function(std::string from,char *c1,char *c2,DIRECTION *d,std::string to){
	transition_function *tf=new transition_function();
	tf->from_state=find_index_of_state(from);
	tf->to_state=find_index_of_state(to);
	tf->directions=new DIRECTION[num_of_tapes];
	tf->from_symbols_index=new int[num_of_tapes];
	tf->to_symbols_index=new int[num_of_tapes];
	for(int i=0;i<num_of_tapes;i++){
		tf->from_symbols_index[i]=find_index_of_tape_symbol(c1[i]);
		tf->to_symbols_index[i]=find_index_of_tape_symbol(c2[i]);
		tf->directions[i]=d[i];
	}
	tran_func.push_back(tf);
	num_of_tran_func++;
}
void t_m::set_blank_symbol(char c)
{
	int index=find_index_of_tape_symbol(c);
	blank_symbol=index;
}
int t_m::get_num_of_states()
{
	return num_of_states;
}
int t_m::get_num_final_states()
{
	return num_of_final_states;
}
int t_m::get_num_of_tape_alphabets()
{
	return num_of_tape_alphabets;
}
int t_m::get_num_of_alphabets()
{
	return num_of_alphabets;
}
int t_m::get_num_of_tran_func(){
	return num_of_tran_func;
}
bool t_m::check_input_validity(std::string input){
	for(int i=0;i<input.size();i++){
		if(input[i]=='*'||find_index_of_tape_symbol(input[i])==-1){
		    return false;
		}
	}
	return true;
}
//transiton return whether we have found or not
bool t_m::transition(int pre,char *encounter,char *result,int &next,DIRECTION *d){
	//num_of_star denotes the matching star('*') numbers, if we deal with the wildcard '*'
	int pre_num_of_star=num_of_tapes+1,num_of_star=0;
	//flag denotes that if we found a match
	bool flag=false,flag_ret=false;;
	for(int i=0;i<tran_func.size();i++){
		transition_function *tf=tran_func.at(i);
		if(tf->from_state==pre){
		    for(int j=0;j<num_of_tapes;j++){
		        if(tf->from_symbols_index[j]==-1){
		            num_of_star++;
		            flag=true;
		            continue;
		        }else if(tape_alphabets[tf->from_symbols_index[j]]==encounter[j]){
		            flag=true;
		            continue;
		        }else{
		            //reset num_of_star
		            num_of_star=0;
		            flag=false;
		            break;
		        }
		    }
		    if(flag){
		        flag_ret=true;
		        if(num_of_star<pre_num_of_star){
		            next=tf->to_state;
		            pre_num_of_star=num_of_star;
		            for(int k=0;k<num_of_tapes;k++){
		                if(tf->to_symbols_index[k]==-1){
		                    result[k]='*';
		                }else{
		                    result[k]=tape_alphabets[tf->to_symbols_index[k]];
		                }
		                d[k]=tf->directions[k];
		            }
		        }
		    }
		    num_of_star=0;
		    flag=false;
		}
	}
	if(flag_ret){
		return true;
	}else{
		return false;
	}
}
/*print instaneous descrption
step denotes the current step
stop means that we reach the final state or the turing machine stops and reject*/
void t_m::print_ID(int step,std::ofstream&cf){
	cf<<"Step    :"<<"    "<<step<<'\n';
	for(int i=0;i<num_of_tapes;i++){
		    cf<<"Index"<<i<<"  :"<<"    ";
		    /*print the index
		    */
		    if(cur[i]<left_offset[i]){
		        for(int j=cur[i];j<=right_offset[i];j++){
		            if(j<0){
		                cf<<std::abs(j)<<"    ";
		            }else{
		                cf<<j<<"    ";
		            }
		        }
		    }else if(cur[i]>=left_offset[i]&&cur[i]<=right_offset[i]){
		        for(int j=left_offset[i];j<=right_offset[i];j++){
		            if(j<0){
		                cf<<std::abs(j)<<"    ";
		            }else{
		                cf<<j<<"    ";
		            }
		        }
		    }else if(cur[i]>right_offset[i]){
		        for(int j=left_offset[i];j<=cur[i];j++){
		            if(j<0){
		                cf<<std::abs(j)<<"    ";
		            }else{
		                cf<<j<<"    ";
		            }
		        }
		    }
		    cf<<'\n';
		    cf<<"Tape"<<i<<"   :"<<"    ";
		    if(cur[i]<left_offset[i]){
		        for(int j=cur[i];j<=right_offset[i];j++){
		            if(j<0){
		                cf<<left_buffer[i][std::abs(j)]<<"    ";
		            }else{
		                cf<<right_buffer[i][j]<<"    ";
		            }
		        }
		    }else if(cur[i]>=left_offset[i]&&cur[i]<=right_offset[i]){
		        for(int j=left_offset[i];j<=right_offset[i];j++){
		            if(j<0){
		                cf<<left_buffer[i][std::abs(j)]<<"    ";
		            }else{
		                cf<<right_buffer[i][j]<<"    ";
		            }
		        }
		    }else if(cur[i]>right_offset[i]){
		        for(int j=left_offset[i];j<=cur[i];j++){
		            if(j<0){
		                cf<<left_buffer[i][std::abs(j)];
		                cf<<"    ";
		            }else{
		                cf<<right_buffer[i][j];
		                cf<<"    ";
		            }
		        }
		    }
		    cf<<'\n';
		    cf<<"Head"<<i<<"   :"<<"    ";
		    if(cur[i]<left_offset[i]){
		        cf<<'^'<<'\n';
		    }else if(cur[i]>=left_offset[i]&&cur[i]<=right_offset[i]){
		        for(int j=0;j<cur[i]-left_offset[i];j++){
		            //must be five space because ^ takes a position
		            cf<<"     ";
		        }
		        cf<<'^'<<'\n';
		    }else if(cur[i]>right_offset[i]){
		        for(int j=0;j<cur[i]-left_offset[i];j++){
		            //must be five space because ^ takes a position
		            cf<<"     ";
		        }
		        cf<<'^'<<'\n';
		    }
		}
	cf<<"State   :"<<"    "<<states[cur_state]<<'\n';
	cf<<"------------------------------------------------------\n";
}
//print the result
void t_m::print_result(bool isFinal,std::ofstream&rf,std::ofstream&cf){
	cf<<"Result: ";
	for(int i=left_offset[0];i<=right_offset[0];i++){
		if(i<0){
		    if(left_buffer[0][std::abs(i)]!=tape_alphabets[blank_symbol]){
		        cf<<left_buffer[0][std::abs(i)];
		    }
		}else if(i>=0){
		    if(right_buffer[0][i]!=tape_alphabets[blank_symbol]){
		        cf<<right_buffer[0][i];
		    }
		}
	}
	cf<<'\n';
	cf<<"=================== END ===================\n";
	if(isFinal){
		rf<<"True"<<std::endl;
	}else{
		rf<<"False"<<std::endl;
	}
}
void t_m::run(std::string input,std::ofstream& rf,std::ofstream& cf)
{
	cf<<"Input: "<<input<<'\n';
	if(!check_input_validity(input)){
		    cf<<"=================== ERR ===================\n";
		    cf<<"The input \""<<input<<"\" is illegal\n";
		    cf<<"=================== END ===================\n";
		    rf<<"Error"<<'\n';
		    return;
	}else{
		cf<<"=================== RUN ===================\n";
		run_string(input,rf,cf); 
	}
}
void t_m::run_file(std::string dir){
	std::ifstream file;
	//input_file specifies the file we get test string 
	std::string input_file=dir+"input.txt";
	//result_file specifies the file we output the result
	std::string result_file=dir+"result.txt";
	//output_file specifies the file we output the infos
	std::string output_file=dir+"console.txt";
	std::ofstream rf,cf;
	rf.open(result_file,std::ios::trunc);
	cf.open(output_file,std::ios::trunc);
	file.open(input_file,std::ios::in);
	if(!file){
		std::cerr<<"No such file:"<<input_file<<std::endl;
		return;
	}
	char* input=new char[MAX_INPUT+1];
	while(!file.eof()){
		file.getline(input,MAX_INPUT);
		/*If we meet the end of line,just break*/
		if(input[0]=='\0')
			break;
		/*The following are depend on your operating system
		  Just for linux,you need to add! Linux '\r'
		*/
		std::string s=input;
		if(s[s.size()-1]=='\r')
			s.pop_back();
		//End specific codes
		run(s,rf,cf);
	}
	file.close();
	rf.close();
	cf.close();
	return;
}
bool t_m::run_string(std::string input,std::ofstream&rf,std::ofstream&cf){
	/*Fisrtly,we need to initialize instaneous description*/
	left_buffer=new char*[num_of_tapes];
	right_buffer=new char*[num_of_tapes];
	for(int i=0;i<num_of_tapes;i++){
		//we let the start of the left buffer be 1
		left_buffer[i]=new char[MAX_INPUT+1];
		right_buffer[i]=new char[MAX_INPUT];
	}
	left_offset=new int[num_of_tapes];
	right_offset=new int[num_of_tapes];
	cur=new int[num_of_tapes];
	//initialize all buffers
	for(int j=0;j<num_of_tapes;j++){
		if(j==0){
		    left_offset[0]=0;
		    right_offset[0]=input.size()-1;
		    cur[0]=0;
		    for(int k1=0;k1<MAX_INPUT;k1++){
		        if(k1>=0&&k1<input.size()){
		            left_buffer[0][k1]=tape_alphabets[blank_symbol];
		            right_buffer[0][k1]=input[k1];
		        }else{
		            left_buffer[0][k1]=tape_alphabets[blank_symbol];
		            right_buffer[0][k1]=tape_alphabets[blank_symbol];
		        }
		        left_buffer[0][MAX_INPUT]=tape_alphabets[blank_symbol];
		    }
		}else{
		    left_offset[j]=0;
		    right_offset[j]=0;
		    cur[j]=0;
		    for(int k2=0;k2<MAX_INPUT;k2++){
		        left_buffer[j][k2]=tape_alphabets[blank_symbol];
		        right_buffer[j][k2]=tape_alphabets[blank_symbol];
		    }
		    left_buffer[j][MAX_INPUT]=tape_alphabets[blank_symbol];
		}
	} 
	//the step we run in the tm machine
	int step=0;
	bool flag_ret=true;
	cur_state=start_state;
	labelS:bool flag=false;
	for(int i=0;i<final_states.size();i++){
		if(cur_state==final_states[i])
		    flag=true;
	}
	if(flag_ret==true){
		print_ID(step,cf);
	}
	if(flag||!flag_ret){
		print_result(flag,rf,cf);
	}
	while(flag_ret&&!flag){ 
		step++;
		//tmpC stores the inputs encounted, result stores the result characters
		char *tmpC=new char[num_of_tapes],*result=new char[num_of_tapes];
		//d denotes the result directions
		DIRECTION *d=new DIRECTION[num_of_tapes];
		//next_state stores the next state from cur_state, next_state may be -1 if no transition function relatively has found
		int next_state;
		for(int j=0;j<num_of_tapes;j++){
		    if(cur[j]<0){
		        tmpC[j]=left_buffer[j][-cur[j]];
		    }else{
		        tmpC[j]=right_buffer[j][cur[j]];
		    }
		}
		flag_ret=transition(cur_state,tmpC,result,next_state,d);
		if(flag_ret){
		    cur_state=next_state;
		    for(int i=0;i<num_of_tapes;i++){
		        if(result[i]!='*'){
		            if(cur[i]<0){
		                left_buffer[i][-cur[i]]=result[i];
		            }else{
		                right_buffer[i][cur[i]]=result[i];
		            }
		        }
		        if(d[i]==RIGHT){
		            cur[i]++;
		        }else if(d[i]==LEFT){
		            cur[i]--;
		        }
		        //this indicates that we have found a boundary
		        bool flagl=false,flagr=false;
		        //we update offsets, notice offsets may be increased or decreased
		        for(int j=std::max(cur[i],right_offset[i]);j>=std::min(cur[i],left_offset[i]);j--){
		            if(j<0){
		                if(left_buffer[i][std::abs(j)]!=tape_alphabets[blank_symbol]){
		                    flagr=true;
		                    right_offset[i]=j;
		                    break;
		                }
		            }else if(j>=0){
		                if(right_buffer[i][j]!=tape_alphabets[blank_symbol]){
		                    flagr=true;
		                    right_offset[i]=j;
		                    break;
		                }
		            }

		        }
		        for(int j=std::min(cur[i],left_offset[i]);j<=std::max(cur[i],right_offset[i]);j++){
		            if(j<0){
		                if(left_buffer[i][std::abs(j)]!=tape_alphabets[blank_symbol]){
		                    flagl=true;
		                    left_offset[i]=j;
		                    break;
		                }
		            }else if(j>=0){
		                if(right_buffer[i][j]!=tape_alphabets[blank_symbol]){
		                    flagl=true;
		                    left_offset[i]=j;
		                    break;
		                }
		            }
		        }
		        if(flagl==false||flagr==false){
		            left_offset[i]=right_offset[i]=cur[i];
		        }
		    }
		}
		free(d);
		free(result);
		free(tmpC);
		goto labelS;
	}
	if(!flag_ret){
		return false;
	}
	if(flag){
		return true;
	}
	return true;
}
void t_m::dump_tm()
{
	std::cout<<"states:";
	for(int i=0;i<num_of_states;i++)
	{
		std::cout<<states[i]<<' ';
	}
	std::cout<<'\n';
	std::cout<<"alphabets:";
	for(int i=0;i<num_of_alphabets;i++)
	{
		std::cout<<alphabets[i]<<' ';
	}
	std::cout<<'\n';
	std::cout<<"tape_alphabets:";
	for(int i=0;i<num_of_tape_alphabets;i++)
	{
		std::cout<<tape_alphabets[i]<<' ';
	}
	std::cout<<'\n';
	std::cout<<"start state: "<<states[start_state]<<'\n';
	std::cout<<"blank symbol: "<<tape_alphabets[blank_symbol]<<'\n';
	std::cout<<"final states:";
	for(int i=0;i<num_of_final_states;i++)
	{
		std::cout<<states[final_states[i]]<<' ';
	}
	std::cout<<'\n';
	std::cout<<"tape number: "<<num_of_tapes<<'\n';
	std::cout<<"transition functions"<<'\n';
	for(int i=0;i<num_of_tran_func;i++){
		    std::cout<<i+1<<": ";
		    std::cout<<states[tran_func[i]->from_state]<<' ';
		    for(int j=0;j<num_of_tapes;j++){
		        int index=tran_func[i]->from_symbols_index[j];
		        if(index==-1)
		            std::cout<<'*';
		        else
		            std::cout<<tape_alphabets[index];
		    }
		    std::cout<<' ';
		    for(int j=0;j<num_of_tapes;j++){
		        int index=tran_func[i]->to_symbols_index[j];
		        if(index==-1)
		            std::cout<<'*';
		        else
		            std::cout<<tape_alphabets[index];
		    }
		    std::cout<<' ';
		    for(int k=0;k<num_of_tapes;k++){
		        switch(tran_func[i]->directions[k]){
		            case LEFT:std::cout<<'l';break;
		            case RIGHT:std::cout<<'r';break;
		            case STAY:std::cout<<'*';break;
		        }
		    }
		    std::cout<<' ';
		    std::cout<<states[tran_func[i]->to_state]<<'\n';
	}
}
