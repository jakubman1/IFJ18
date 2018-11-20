# LL gramatika

```
<prog> → def ID <params> EOL <statement> end EOL  
<prog> → ID <params> EOL  
<prog> → ID = ID <params> EOL  
<prog> → <statement>  
<params> → (<params>)  
<params> → ID <params_n>  
<params> → ε  
<params_n> → , ID <params_n>  
<params_n> → ε  
<statement> → if <expression> then EOL <statement> else EOL <statement> end EOL  
<statement> → while <expression> do EOL <statement> end EOL  
<statement> → ID = <define> EOL  
<statement> → <define> EOL  
<statement> → ε  
<define> → print <args>  
<define> → inputs <args>  
<define> → inputi <args>  
<define> → inputf <args>  
<define> → lenght <args>  
<define> → substr <args>  
<define> → ord <args>  
<define> → chr <args>  
<define> → <expression>  
<define> → ε  
<args> → <value> <args_n>  
<args> → ε  
<args_n> → , <value> <args_n>  
<args_n> → ε  
<value> → INT  
<value> → STRING  
<value> → FLOAT  
<value> → ID  
```
