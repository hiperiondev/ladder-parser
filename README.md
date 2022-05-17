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
 R---+--{mux%1_S0}                                         
                                                           
    NODE: _N_1 = (A) | (D) | (F) 
    NODE: _N_6 = ($c) | (V) 
FUNCTION: ton%4 = pt::([10ms]) in::(_N_1) 
FUNCTION: gt%2 = in0::($b) in1::(_N_6) 
    NODE: _N_8 = (W) | (R) 
FUNCTION: mux%1 = in0::(_N_1) in1::(!I & P) in2::($a1) in3::(T) S1::(_Q) S0::(_N_8) 
FUNCTION: eq%3 = en::({gt%2_out}) in1::(_N_6) in2::(_N_8) 
    NODE: _N_5 = (K & L & M & !N) | ({eq%3_eno}) 
    NODE: _N_4 = ((((({ton%4_q}) | (!{mux%1_out})) & C) | ((({ton%4_q}) | (!{mux%1_out})) & !E) | ((({ton%4_q}) | (!{mux%1_out})) & G))) | (_N_5 & !X) | ({eq%3_out}) 
  OUTPUT: $b = (_N_5) 
  OUTPUT: Q = (_N_4 & Y) 
```
