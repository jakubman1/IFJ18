## Table of operators

| Priority      | Operator      | Asociativity |
| :-----------: |:-------------:| :-----------:|
| 1             | */            | LEFT         |
| 2             | +-            | LEFT         |
| 3             | < <= > >=     | NONE         |
| 4             | == !=         | NONE         |

## Rules for expressions
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
## Precedent table

|    | +- | */ | r | e | ( | ) | i | $ |
|:--:|:--:|:--:|:-:|:-:|:-:|:-:|:-:|:-:|
| +- | >  | <  | > | > | < | > | < | > |
| */ | >  | >  | > | > | < | > | < | > |
| r  | <  | <  |   | > | < | > | < | > |
| e  | <  | <  | < |   | < | > | < | > |
| (  | <  | <  | < | < | < | = | < |   |
| )  | >  | >  | > | > |   | > |   | > |
| i  | >  | >  | > | > |   | > |   | > |
| $  | <  | <  | < | < | < |   | < |   |

r - "< <= > >="  
e - "== !="  
i - identificator
