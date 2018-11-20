# LL gramatika

```
<prog> → def ID <params> EOL <statement> end EOL  
<prog> → ID <params> EOL  
<prog> → ID = ID <params> EOL  
<prog> → <statement>  
<params> → <params>  
<params> → (<params>)  
<params> → ID <params_n>  
<params> → ε  
<params_n> → , ID <params_n>  
<params_n> → ε  
<statement> → if <expression> then EOL <statement> EOL <statement> end EOL  
<statement> → while <expression> do EOL <statement> end EOL  
<statement> → ID = <define> EOL  
<statement> → <define> EOL  
<statement> → ε  
<define> → print <arg>  
<define> → inputs <arg>  
<define> → inputi <arg>  
<define> → inputf <arg>  
<define> → lenght <arg>  
<define> → substr <arg>  
<define> → ord <arg>  
<define> → chr <arg>  
<define> → <expression>  
<define> → ε  
<args> → <value> <args_n>  
<args> → ε  
<args_n> → <value> <args_n>  
<args_n> → ε  
<value> → INT  
<value> → STRING  
<value> → FLOAT  
<value> → ID  
```
