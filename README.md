# ladder-parser
Library for parse and simplify ladder diagram

### BUILD
- git clone https://github.com/hiperiondev/ladder-parser/
- cd ladder-parser/build
- make

### EXAMPLE
```
- rung -
 A--+-+-/B--------------------------+---C---+---+-----Q 
    | |                             |       |   |       
 D--+ |                             +--/E---+   |       
    | |                             |       |   |       
 F--+ |                             +---G---+   |       
      |                             |           |       
      |            /{mux%1_out}-----+           |       
      +-------------{mux%1_in[0]}               |       
 /I---P-------------{mux%1_in[1]}               |       
    $a1-------------{mux%1_in[2]}               |       
 T------------------{mux%1_in[3]}               |       
 R------------------{mux%1_S[0]}                |       
 [previous_Q]-------{mux%1_S[1]}                |       
                                                |       
     $b--{gt%2_in[0]}                           |       
     $c--{gt%2_in[1]}                           |       
         {gt%2_out}-----------------------------+       
                                                |       
 K--+--L---M---/N-------------------------------+       
    |                                           |       
    +--{eq%3_en}                                |       
    V--{eq%3_in[1]}                             |       
    W--{eq%3_in[2]}                             |       
       {eq%3_eno}                               |       
       {eq%3_out}-------------------------------+       

--------- FINAL RESULT ---------

[IN]: 
      INFIX: _N[2] = ((A) | (D) | (F)) 
    POSTFIX: _N[2] = A D or F or 

      INFIX: _N[5] = ((((_N[2] & !B) | (!{mux%1_out})) & C) | (((_N[2] & !B) | (!{mux%1_out})) & !E) | (((_N[2] & !B) | (!{mux%1_out})) & G)) | ({gt%2_out}) | ((K) & L & M & !N) | ({eq%3_out}) 
    POSTFIX: _N[5] = _N[2] B not and {mux%1_out} not or C and _N[2] B not and {mux%1_out} not or E not and or _N[2] B not and {mux%1_out} not or G and or {gt%2_out} or K L and M and N not and or {eq%3_out} or 


[OUT]: 
      INFIX: Q = (_N[5]) 
    POSTFIX: Q = _N[5] 

      INFIX: {mux%1_in[0]} = (_N[2]) 
    POSTFIX: {mux%1_in[0]} = _N[2] 

      INFIX: {mux%1_in[1]} = (!I & P) 
    POSTFIX: {mux%1_in[1]} = I not P and 

      INFIX: {mux%1_in[2]} = ($a1) 
    POSTFIX: {mux%1_in[2]} = $a1 

      INFIX: {mux%1_in[3]} = (T) 
    POSTFIX: {mux%1_in[3]} = T 

      INFIX: {mux%1_S[0]} = (R) 
    POSTFIX: {mux%1_S[0]} = R 

      INFIX: {mux%1_S[1]} = ([previous_Q]) 
    POSTFIX: {mux%1_S[1]} = [previous_Q] 

      INFIX: {gt%2_in[0]} = ($b) 
    POSTFIX: {gt%2_in[0]} = $b 

      INFIX: {gt%2_in[1]} = ($c) 
    POSTFIX: {gt%2_in[1]} = $c 

      INFIX: {eq%3_en} = (K) 
    POSTFIX: {eq%3_en} = K 

      INFIX: {eq%3_in[1]} = (V) 
    POSTFIX: {eq%3_in[1]} = V 

      INFIX: {eq%3_in[2]} = (W) 
    POSTFIX: {eq%3_in[2]} = W 

--------------------------------
```
