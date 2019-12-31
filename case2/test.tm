
;本图灵机程序以双带形式模拟ww,w由0或1组成 的字符串
;具体思想是首先在第二条带上填充X，然后巡回填充Y（X换为Y），找到中间位置，然后将一半复制到
;第二条带上，最后上下同时消掉就行
#Q = 
{start,q0,q1,q2,q3,q4,qm,q5,q6,accept,accept1,accept2,clear1,reject,reject1,reject2,reject3,reject4,reject5,reject6,reject7,halt_reject,accept3,halt_accept,clear}

#S = {0,1}


#G = {0,1,_,Y,X,T,r,u,e,F,a,l,s}



#q0 = start


#B = _

#F = {halt_accept}

#N = 2

;首先第二条带上填充满Y
start ** ** ** q0
q0 ** *Y rr q0
;空字符串接收
start __ ** ** accept
q0 __ __ ll q1
;循环将Y换位X，以找到中间位置
q1 *Y *X *l q2
q2 *Y *Y *l q2
q2 *_ *_ *r q3
q2 *X *X *r q3
q3 *Y *X *r q4
q4 *Y *Y *r q4
q4 *X *X *l q1
;找到中间位置，qm表示此状态
q1 *X *X ** qm
;然后将一半的内容复制到第二条子带上，这样上下都是w
qm 0* *0 ll qm
qm 1* *1 ll qm
qm 0_ ** rr q5
qm 1_ ** rr q5
q5 ** ** l* q5
q5 _* ** r* q6
;消掉同样的字符，如果不同则拒绝
q6 00 __ rr q6
q6 11 __ rr q6
q6 *X ** ** accept
q6 ** ** ** reject
;打印True
accept ** T_ rr accept1
accept1 ** r_ rr accept2
accept2 ** u_ rr accept3
accept3 ** e_ rr clear
clear ** __ rr clear
clear __ ** ll clear1
clear1 __ ** ll clear1
clear1 e_ ** ** halt_accept
q3 ** ** ** reject
;打印False
reject ** FX rr reject1
reject1 ** aX rr reject2
reject2 ** lX rr reject3
reject3 ** sX rr reject4
reject4 ** eX rr reject5
reject5 ** XX rr reject5
reject5 __ ** ll reject6
reject6 ** __ ll reject6
reject6 F* *_ ll reject6
reject6 a* *_ ll reject6
reject6 l* *_ ll reject6
reject6 s* *_ ll reject6
reject6 e* *_ ll reject6
reject6 __ __ rr reject7
reject7 ** ** rr reject7
reject7 F_ ** ** halt_reject
