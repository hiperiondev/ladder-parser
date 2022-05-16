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
 A--+-+--------------/B-------------+---C---+------+-----Q 
    | |                             |       |      |       
 D--+ |                             +--/E---+      |       
    | |                             |       |      |       
 F--+ |                             +---G---+      |       
      |                             |              |       
      |            /{mux%1_out}-----+              |       
      +-------------{mux%1_in[0]}                  |       
 /I---------P-------{mux%1_in[1]}                  |       
 $a1----------------{mux%1_in[2]}                  |       
 T------------------{mux%1_in[3]}                  |       
 [previous_Q]-------{mux%1_S[1]}                   |       
                                                   |       
 K-----L---M---/N--------------------+-----/X------+       
                                     |             |       
 $b-----{gt%2_in[0]}                 |             |       
 $c--+--{gt%2_in[1]}                 |             |       
     |  {gt%2_out}---+               |             |       
     |               |               |             |       
     |               +-{eq%3_en}     |             |       
 V---+-----------------{eq%3_in[1]}  |             |       
                                     |             |       
 W---+-----------------{eq%3_in[2]}  |             |       
     |                 {eq%3_eno}----+----Z        |       
     |                 {eq%3_out}------------------+       
 R---+--{mux%1_S[0]}                                       

--------- FINAL RESULT ---------

[IN] 
      INFIX: _N[2] = ((A) | (D) | (F)) 
    POSTFIX: _N[2] = A D or F or 

      INFIX: _N[5] = ((((_N[2] & !B) | (!{mux%1_out})) & C) | (((_N[2] & !B) | (!{mux%1_out})) & !E) | (((_N[2] & !B) | (!{mux%1_out})) & G)) | (_N[6] & !X) | ({eq%3_out}) 
    POSTFIX: _N[5] = _N[2] B not and {mux%1_out} not or C and _N[2] B not and {mux%1_out} not or E not and or _N[2] B not and {mux%1_out} not or G and or _N[6] X not and or {eq%3_out} or 

      INFIX: _N[6] = (K & L & M & !N) | ({eq%3_eno}) 
    POSTFIX: _N[6] = K L and M and N not and {eq%3_eno} or 

      INFIX: _N[7] = ($c) | (V) 
    POSTFIX: _N[7] = $c V or 

      INFIX: _N[9] = (W) | (R) 
    POSTFIX: _N[9] = W R or 


[OUT] 
      INFIX: Q = (_N[5]) 
    POSTFIX: Q = _N[5] 

      INFIX: Z = (_N[6]) 
    POSTFIX: Z = _N[6] 


[FUNCTIONS] 
         fn: mux%1 (in[0]=(_N[2]), in[1]=(!I & P), in[2]=($a1), in[3]=(T), S[1]=([previous_Q]), S[0]=(_N[9]))
         fn: eq%3 (en=({gt%2_out}), in[1]=(_N[7]), in[2]=(_N[9]))
         fn: gt%2 (in[0]=($b), in[1]=(_N[7]))

--------------------------------
```
