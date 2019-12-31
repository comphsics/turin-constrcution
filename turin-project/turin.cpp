#include<iostream>
#include"tm_construct.cpp"
#include"tm.cpp"
int main(int argc,char *argv[])
{
    /*这里图灵机程序文件名必须是test.tm
      输入文件名必须是input.txt
    */
    if(argc!=2){
      std::cout<<"Uncorrect parameters!\n";
      return 0;
    }
    std::string f=argv[1];
    //The following specifies the path separator
    std::string pathDividor="/";
    //The following specifies the test file name
    std::string tm_file="test.tm";
    turin_construct *t_c=new turin_construct(f+pathDividor+tm_file);
    t_m *turin_machine=t_c->run();
    if(turin_machine!=nullptr)
    {
      turin_machine->run_file(f+pathDividor);  
    }
    return 0;
}
