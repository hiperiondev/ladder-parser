# ladder-parser
Library for parse and simplify ladder diagram

### BUILD
- git clone https://github.com/hiperiondev/ladder-parser/
- cd ladder-parser/build
- make

#### TODO
- Evaluation Order

### EXAMPLE
```
- rung -
 A--+-+------------{ton%4_q}--------+---C---+------+--Y--Q 
    | |                             |       |      |       
 D--+ |                             +--/E---+      |       
    | |                             |       |      |       
 F--+ |                             +---G---+      |       
      |                             |              |       
      |            /{mux%1_out}-----+              |       
      +-------------{mux%1_in0}                    |       
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
      INFIX: _N_2 = ((A) | (D) | (F)) 
    POSTFIX: _N_2 = A D or F or 

      INFIX: _N_5 = ((((_N_2 & {ton%4_q}) | (!{mux%1_out})) & C) | (((_N_2 & {ton%4_q}) | (!{mux%1_out})) & !E) | (((_N_2 & {ton%4_q}) | (!{mux%1_out})) & G)) | (_N_6 & !X) | ({eq%3_out}) 
    POSTFIX: _N_5 = _N_2 {ton%4_q} and {mux%1_out} not or C and _N_2 {ton%4_q} and {mux%1_out} not or E not and or _N_2 {ton%4_q} and {mux%1_out} not or G and or _N_6 X not and or {eq%3_out} or 

      INFIX: _N_6 = (K & L & M & !N) | ({eq%3_eno}) 
    POSTFIX: _N_6 = K L and M and N not and {eq%3_eno} or 

      INFIX: _N_7 = ($c) | (V) 
    POSTFIX: _N_7 = $c V or 

      INFIX: _N_9 = (W) | (R) 
    POSTFIX: _N_9 = W R or 


[OUT] 
      INFIX: Q = (_N_5 & Y) 
    POSTFIX: Q = _N_5 Y and 

      INFIX: $b = (_N_6) 
    POSTFIX: $b = _N_6 


[FUNCTIONS] 
         FN: ton%4 = pt::([10ms]) 
         FN: mux%1 = in0::(_N_2) in1::(!I & P) in2::($a1) in3::(T) S1::(_Q) S0::(_N_9) 
         FN: eq%3 = en::({gt%2_out}) in1::(_N_7) in2::(_N_9) 
         FN: gt%2 = in0::($b) in1::(_N_7) 

--------------------------------
```
