# ladder-parser
Library for parse and simplify ladder diagram

### BUILD
- git clone https://github.com/hiperiondev/ladder-parser/
- cd ladder-parser/build
- make

#### LIMITATIONS
- An output must be the end of line or have only ORs
- Others?

#### TODO
- Evaluation Order

### EXAMPLE
```
- rung -
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
 R---+--{mux%1_S0}                                         
                                                           
 [10ms]--{ton%4_pt}                                        

--------- FINAL RESULT ---------

[IN] 
      INFIX: _N_1 = (A) | (D) | (F) 
    POSTFIX: _N_1 = A D or F or 

      INFIX: _N_4 = (((({ton%4_q}) | (!{mux%1_out})) & C) | ((({ton%4_q}) | (!{mux%1_out})) & !E) | ((({ton%4_q}) | (!{mux%1_out})) & G)) | (_N_5 & !X) | ({eq%3_out}) 
    POSTFIX: _N_4 = {ton%4_q} {mux%1_out} not or C and {ton%4_q} {mux%1_out} not or E not and or {ton%4_q} {mux%1_out} not or G and or _N_5 X not and or {eq%3_out} or 

      INFIX: _N_5 = (K & L & M & !N) | ({eq%3_eno}) 
    POSTFIX: _N_5 = K L and M and N not and {eq%3_eno} or 

      INFIX: _N_6 = ($c) | (V) 
    POSTFIX: _N_6 = $c V or 

      INFIX: _N_8 = (W) | (R) 
    POSTFIX: _N_8 = W R or 


[OUT] 
      INFIX: Q = (_N_4 & Y) 
    POSTFIX: Q = _N_4 Y and 

      INFIX: $b = (_N_5) 
    POSTFIX: $b = _N_5 


[FUNCTIONS] 
         FN: ton%4 = in::(_N_1) pt::([10ms]) 
         FN: mux%1 = in0::(_N_1) in1::(!I & P) in2::($a1) in3::(T) S1::(_Q) S0::(_N_8) 
         FN: eq%3 = en::({gt%2_out}) in1::(_N_6) in2::(_N_8) 
         FN: gt%2 = in0::($b) in1::(_N_6) 

--------------------------------
```
