# ladder-parser
Library for parse and simplify ladder diagram

### BUILD
- git clone https://github.com/hiperiondev/ladder-parser/
- cd ladder-parser/build
- make

#### LIMITATIONS
- An output must be the end of line or have only ORs
- Node in first line must be the most left

#### TODO
- 

### EXAMPLE
```
- rung -
          [10ms]--{ton%4_pt}                               
    +-------------{ton%4_in}                               
 A--+             {ton%4_q}---------+---C---+------+--Y--Q 
    |                               |       |      |       
 D--+                               +--/E---+      |       
    |                               |       |      |       
 F--+                               +---G---+      |       
    |                               |              |       
    |              /{mux%1_out}-----+              |       
    +---------------{mux%1_in0}                    |       
 /I---------P-------{mux%1_in1}                    |       
 $a1----------------{mux%1_in2}                    |       
 T------------------{mux%1_in3}                    |       
 _Q-----------------{mux%1_S1}                     |       
                                                   |       
 K-----L---M---/N--------------------+-----/X------+       
                                     |             |       
 $b-----{gt%2_in0}                   |             |       
 $c--+--{gt%2_in1}                   |             |       
     |  {gt%2_out}---+               |             |       
     |               |               |             |       
     |               +-{eq%3_en}     |             |       
 V---+-----------------{eq%3_in1}    |             |       
                                     |             |       
 W---+-----------------{eq%3_in2}    |             |       
     |                 {eq%3_eno}----+----$b       |       
     |                 {eq%3_out}------------------+       
 R---+---------------{mux%1_S0}                            
                                                           
    NODE: _N_1 = A | D | F 
          postfix: A D or F or 
    NODE: _N_6 = $c | V 
          postfix: $c V or 
FUNCTION: ton%4 = pt::[10ms] in::_N_1 
FUNCTION: gt%2 = in0::$b in1::_N_6 
    NODE: _N_8 = W | R 
          postfix: W R or 
FUNCTION: mux%1 = in0::_N_1 in1::(!I & P) in2::$a1 in3::T S1::_Q S0::_N_8 
FUNCTION: eq%3 = en::{gt%2_out} in1::_N_6 in2::_N_8 
    NODE: _N_5 = (K & L & M & !N) | {eq%3_eno} 
          postfix: K L and M and N not and {eq%3_eno} or 
    NODE: _N_4 = ((({ton%4_q} | !{mux%1_out}) & C) | (({ton%4_q} | !{mux%1_out}) & !E) | (({ton%4_q} | !{mux%1_out}) & G)) | (_N_5 & !X) | {eq%3_out} 
          postfix: {ton%4_q} {mux%1_out} not or C and {ton%4_q} {mux%1_out} not or E not and or {ton%4_q} {mux%1_out} not or G and or _N_5 X not and or {eq%3_out} or 
  OUTPUT: $b = _N_5 
          postfix: _N_5 
  OUTPUT: Q = (_N_4 & Y) 
          postfix: _N_4 Y and
```
