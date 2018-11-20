## Table of operators

| Priority      | Operator      | Asociativity |
| :-----------: |:-------------:| :-----------:|
| 1             | */            | LEFT         |
| 2             | +-            | LEFT         |
| 3             | < <= > >=     |    $1        |
| 4             | == !=         |    $1        |

## Rules for expression
```
E → E + E  
E → E - E  
E → E * E  
E → E / E  
E → (E)  
E → i  
E → int  
E → float  
E → string  
E → E < E  
E → E <= E  
E → E > E  
E → E >= E  
E → E == E  
E → E != E  
```
## Precedention table

|    | +- | */ | r | e | ( | ) | i | $ |
|:--:|:--:|:--:|:-:|:-:|:-:|:-:|:-:|:-:|
| +- | >  | <  | > | > | < | > | < | > |
| */ | >  | >  | > | > | < | > | < | > |
| r  | <  | <  |   | > | < | > | < | > |
| e  | <  | <  | < |   | < | > | < | > |
| (  | <  | <  |   |   | < | = | < |   |
| )  | >  | >  |   |   |   | > |   | > |
| i  | >  | >  |   |   |   | > |   | > |
| $  | <  | <  |   |   | < |   | < |   |

r - "< <= > >="

e - "== !="

i - identificator
