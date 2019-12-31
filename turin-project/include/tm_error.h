/*
tm_error.h this file specifies the problem report during the turin file parser
ERROR_TYPE this struct defines the some types of errors, such as ILLEGAL_INPUT means you enter a illegal character
int_to_string parse the integer to string
tm_error to a certain error type and output some infomations
*/
#ifndef __ERROR_H__
#define __ERROR_H__
#include<iostream>
enum ERROR_TYPE{
    //turin file(*.tm) does not exist
    FILE_NOT_EXISTS,
    //state set error
    ERROR_STATES_SYNTAX,
    //alphabets set error
    ERROR_ALPHABETS_SYNTAX,
    //tape alphabets error
    ERROR_TAPE_ALPHABETS_SYNTAX,
    //start state error
    ERROR_START_STATE_SYNTAX,
    //blank symbol error
    ERROR_BLANK_SYMBOL_SYNTAX,
    //final states error
    ERROR_FINAL_STATES_SYNTAX,
    //tape number error
    ERROR_TAPE_NUMBER_SYNTAX,
    //illegal input
    ILLEGAL_INPUT,
    //alphabets not in the tape alphabets
    ALPHABETS_NOT_IN_TAPE_ALPHABETS,
    //final states not in the states
    FINAL_STATES_NOT_IN_STATES,
    //start state not int the states
    START_STATE_NOT_IN_STATES,
    //blank symbol not in the tape alphabets
    BLANK_SYMBOL_NOT_IN_TAPE_ALPHABETS,
    //lack of elements
    LACK_OF_ELEMENTS,
    //# must leading a line
    ELEMENT_MUST_LEADING_LINE,
    //repeated states
    STATES_REPEAT,
    //repeated alphabets
    ALPHABET_REPEAT,
    //repeated tape alphabets
    TAPE_ALPHABET_REPEAT,
    //repeated final state
    FINAL_STATE_REPEAT,
    //blank symbol in alphabets
    BLANK_SYMBOL_IN_ALPHABET,
    //transition functions error
    TRANSITION_FUNCTION_ERROR,
    //transition functions number error
    ERROR_NUMBER_IN_TRANS,
    //transition functions symbol not in tape alphabets
    TRAN_SYMBOL_NOT_IN_TAPE_ALPHABETS,
    //transition functions state not in states
    TRAN_STATE_NOT_IN_STATES
};
std::string int_to_string(int value){
    std::string ret;
    int rem=0;
    while(value!=0){
        rem=value%10;
        value=value/10;
        ret.push_back(rem+48);
    }
    return ret;
}
void tm_error(ERROR_TYPE type,int line,std::string assist){
    switch(type){
        case FILE_NOT_EXISTS:{
            std::cout<<"Error:File Not Exist: "+assist<<std::endl;break;
    }
        case ERROR_STATES_SYNTAX:std::cout<<"Error:States syntax  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ERROR_ALPHABETS_SYNTAX:std::cout<<"Error:Alphabets syntax  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ERROR_TAPE_ALPHABETS_SYNTAX:std::cout<<"Error:tape alphabets syntax  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ERROR_START_STATE_SYNTAX:std::cout<<"Error:Start state syntax  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ERROR_BLANK_SYMBOL_SYNTAX:std::cout<<"Error:Blank symbol syntax  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ERROR_FINAL_STATES_SYNTAX:std::cout<<"Error:Final states syntax  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ERROR_TAPE_NUMBER_SYNTAX:std::cout<<"Error:Tape number syntax  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ILLEGAL_INPUT:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ALPHABETS_NOT_IN_TAPE_ALPHABETS:
        std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<"alphabet:"<<assist<<std::endl;
        break;
        case FINAL_STATES_NOT_IN_STATES:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<"final state:"<<assist<<std::endl;
        break;
        case START_STATE_NOT_IN_STATES:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<"start state:"<<assist<<std::endl;
        break;
        case BLANK_SYMBOL_NOT_IN_TAPE_ALPHABETS:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case LACK_OF_ELEMENTS:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case ELEMENT_MUST_LEADING_LINE:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<assist<<std::endl;
        break;
        case STATES_REPEAT:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<"state:"<<assist<<std::endl;
        break;
        case ALPHABET_REPEAT:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<"alphabet:"<<assist<<std::endl;
        break;
        case TAPE_ALPHABET_REPEAT:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<"tape alphabet:"<<assist<<std::endl;
        break;
        case FINAL_STATE_REPEAT:std::cout<<"Error!  ";
        if(line!=0){
            std::cout<<"in line number "<<line<<"  ";
        }
        std::cout<<"final state:"<<assist<<std::endl;
        break;
        case BLANK_SYMBOL_IN_ALPHABET:std::cout<<"Error! ";
        if(line!=0){
            std::cout<<"in line number "<<line<<" ";
        } 
        std::cout<<"blank symbol:"<<assist<<std::endl;
        break;
        case TRANSITION_FUNCTION_ERROR:std::cout<<"Error! ";
        if(line!=0){
            std::cout<<"in line number "<<line<<" ";
        }
        std::cout<<"transition function error: "<<assist<<'\n';
        break;
        case ERROR_NUMBER_IN_TRANS:std::cout<<"Error! ";
        if(line!=0){
            std::cout<<"in line number "<<line<<" ";
        }
        std::cout<<"inequal error: "<<assist<<'\n';
        break;
        case TRAN_SYMBOL_NOT_IN_TAPE_ALPHABETS:std::cout<<"Error! ";
        if(line!=0){
            std::cout<<"in line number "<<line<<" ";
        }
        std::cout<<assist<<'\n';
        break;
        case TRAN_STATE_NOT_IN_STATES:std::cout<<"Error! ";
        if(line!=0){
            std::cout<<"in line number "<<line<<" ";
        }
        std::cout<<assist<<'\n';
        break;
    }
}
#endif
