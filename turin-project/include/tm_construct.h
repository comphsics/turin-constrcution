/*
  turin_construct: this class can get *.tm and parser it to 
  generate a turin class defined in "tm.h"
  We may parse it and generate some errors so it is better to 
  divide the construct file and turin file
*/
#ifndef __TM_CONSTRUCT_H__
#define __TM_CONSTRUCT_H__
#include"tm.h"
#include<iostream>
#include<vector>
#include<fstream>
#include"tm_error.h"
//We use tran_func to denote a transition function of five parts
struct tran_func{
    //from state
    std::string from;
    //chars we meet
    char *from_symbols;
    //chars we change
    char *to_symbols;
    //directions
    DIRECTION *directions;
    //to some state
    std::string to;
};
//construct a turin file
class turin_construct{
private:
    //TM denotes the aim turin class
    t_m *TM;
    //denotes *.tm file
    std::ifstream tm_file;
    //stores the states we meet
    std::vector<std::string> *states;
    //stores the alphabets we meet
    std::vector<char> *alphabets;
    //stores the tape alphabets we meet
    std::vector<char> *tape_alphabets;
    //stores the final states we meet
    std::vector<std::string> *final_states;
    //denotes the start state
    std::string start_state;
    //denotes the tape number
    int tape_number;
    //denotes the blank symbol, in this project we use _ to represent the blank symbol
    char blank_symbol;
    //stores the transition functions we meet
    std::vector<tran_func> *trans;
    /*this field means during the one transition functions parsing, 
      we can get the tape numbers, and check whether 
      it is equal to the previous tape number from the previous 
      transition function and whether it is equal to the tape number
    */
    int cur_tape_in_tran_func_number;
    //get a character from the tm_file and advance a char
    char get_advanced_char();
    //get a character from the tm_file but not advance file cursor
    char get_no_advanced_char();
    //get a string from the tm_file
    std::string get_advanced_str();
    //if we parsing the whole *.tm file,we check whether it is a correct turing file
    bool check_validity();
    void set_states();
    void set_alphabets();
    void set_tape_symbols();
    bool set_start_state();
    bool set_blank_symbol();
    void set_final_states();
    bool set_tape_number();
    void set_tran_func();
    //error handling
    void error_handle(ERROR_TYPE,int,std::string);
    //comment handling
    bool comment_handle();
    //file end handling
    void file_end_handle();
    //the main function we get a turing file from the tm_file
    bool construct();
    /*deal with states,alphabets,tape alphabets, final states, blank symbol
      tape number,and start state
    */
    bool element_handle();
    //transition functions handling
    bool tran_func_handle();
    //states handling
    bool states_handle();
    //alphabets handling
    bool alphabets_handle();
    //tape alphabets handling
    bool tape_alphabets_handle();
    //final states handling
    bool final_states_handle();
    /*hasLeadingLine defines whether the character reading has at the start of the line*/
    bool hasLeadingLine;
    /*Current line number*/
    int line_number;
    /*Current tm file name*/
    std::string file_name;
public:
    turin_construct(std::string);
    ~turin_construct();
    /*run function calls the construct function*/
    t_m* run();
};
#endif
