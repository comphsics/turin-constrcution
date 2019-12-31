
;本图灵机思想是三条带模拟，最下面两条带存放斐波那契数列的两个项,如1,1;1,2;2,3等
;然后将第二条带全部放置第三条上，第三条又放在第二条上，这样相当实现了加法
;若最后放完一趟后与第一条对齐，则是一个斐波那契数
#Q = 
{start,q0,q1,q2,q3,q4,reject11,reject12,accept1,accept2,accept3,accept4,accept5,accept6,accept7,accept8,reject1,reject2,reject3,reject4,reject5,reject6,reject7,reject8,halt_accept,halt_reject}
;
#S = {0}

;
#G = {0,_,F,a,l,s,T,r,u,e}


#q0 = start

#B = _

#F = {halt_accept}

#N = 3

;首先设置初始项，剩余两条带都放置一个
start 0__ 000 rrr q4
;检查初始状态，若第一条带只有一个，则接受
q4 ___ *** lll accept1
;将第二条放在第三条上
q4 0__ *** lll q0 
q0 000 *** rr* q0
q0 0_0 *0* rrr q0
q0 0__ *** lll q1
q0 ___ *** lll accept1
q0 __0 *** *** reject1
q1 000 *** lll q1
;将第三条放在第二条上
q1 00_ *** llr q2
q1 0_0 *** lrl q4
q4 000 *** l*l q4
q4 _0_ *** r*r q0
q2 000 *** ll* q2
q2 __0 *** rr* q3
q3 000 *** r*r q3
q3 00_ **0 rrr q3
;回到q1，循环
q3 0__ *** lll q1
q3 ___ *** lll accept1
q3 _0_ *** *** reject1
;打印True，并消掉其余字符
accept1 *** ___ lll accept1
accept1 *_* ___ lrl accept2
accept1 **_ ___ llr accept8
accept2 *** _*_ l*l accept2
accept2 _*_ *** r*r accept3
accept3 *** T** r** accept4
accept4 *** r** r** accept5
accept5 *** u** r** accept6
accept6 *** e** r** accept7
accept8 *** __* ll* accept8
accept8 __* *** *** accept3
accept7 *** *** l** halt_accept
;打印False，并消掉其余字符
reject1 __0 *** rrr reject1
reject1 _0_ *** rrr reject1
reject1 ___ *** lll reject2
reject2 *** ___ lll reject2
reject2 ___ ___ rrr reject3
reject3 *** F** r** reject4
reject4 *** a** r** reject5
reject5 *** l** r** reject6
reject6 *** s** r** reject7
reject7 *** e** r** reject8
reject8 *** *** l** halt_reject