/*This file denotes the correct turing files
  run() checks the input string and call run_string 
  run_file() runs on the file
  run_string() runs the correct input string
  dump_tm() prints the turin machine to the stdin
*/
#ifndef __TM_H__
#define __TM_H__
#include<iostream>
#include<fstream>
#include<vector>
//We specify '\t' with four spaces
//MAX_INPUT is the max length of the every tape input
#define  MAX_INPUT  1000000
//direction left,right or donot move
enum DIRECTION{
    LEFT,RIGHT,STAY
};

struct transition_function{
    int from_state;
    int *from_symbols_index;
    int *to_symbols_index;
    DIRECTION *directions;
    int to_state;
};
class t_m{
private:
    std::vector<std::string> states;
    std::vector<char> alphabets;
    std::vector<char> tape_alphabets;
    int start_state;
    int blank_symbol;
    std::vector<int> final_states;
    int num_of_tapes;
    /*transition_functions*/
    std::vector<transition_function*> tran_func; 
    /*instaneous desciption*/
    //cur_state denotes the current state
    int cur_state;
    /*buffer is the buffer storing the multi-tapes'strings
      we use two buffers to denote one stream input that are biinfinite
    */
    char **left_buffer;
    char **right_buffer;
    //we use cur to denote the current position relative to the right buffers
    int *cur;
    //we use left_offset to denote the left offset
    int *left_offset;
    //we use right_offset to denote the right offset
    int *right_offset;
    /*Auxilary,we can get some properties*/
    int num_of_states;
    int num_of_alphabets;
    int num_of_tape_alphabets;
    int num_of_final_states;
    int num_of_tran_func;
    int find_index_of_state(std::string);
    int find_index_of_tape_symbol(char);
public:
    t_m();
    int get_num_of_tapes();
    void set_num_of_tapes(int);
    std::string get_start_state();
    void add_state(std::string);
    void add_alphabet(char);
    void add_tape_alphabet(char);
    void set_start_state(std::string);
    void add_final_state(std::string);
    void add_transition_function(std::string,char*,char*,DIRECTION*,std::string);
    void set_blank_symbol(char);
    int get_num_of_states();
    int get_num_final_states();
    int get_num_of_tape_alphabets();
    int get_num_of_alphabets();
    int get_num_of_tran_func();
    bool check_input_validity(std::string);
    //transiton return whether we have found or not
    bool transition(int,char*,char*,int&,DIRECTION*);
    /*print instaneous descrption
      step denotes the current step
      stop means that we reach the final state or the turing machine stop and reject*/
    void print_ID(int,std::ofstream&);
    //print the result
    void print_result(bool,std::ofstream&,std::ofstream&);
    void run(std::string,std::ofstream&,std::ofstream&);
    void run_file(std::string);
    bool run_string(std::string,std::ofstream&,std::ofstream&);
    void dump_tm();
};
#endif
