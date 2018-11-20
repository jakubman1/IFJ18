# LL gramatika
<ol>
<li>test</li>
<li>test2</li>
</ol>
```
<prog> → def ID <params> EOL <statement_list> end EOL    
<prog> → <statement_list>

<params> → (<params>)  
<params> → ID <params_n>  
<params> → ε  

<params_n> → , ID <params_n>  
<params_n> → ε  

<statement_list> → ε

<statement> → if <expression> then EOL <statement> else EOL <statement> end EOL  
<statement> → while <expression> do EOL <statement> end EOL  
<statement> → ID = <define> EOL  
<statement> → <define> EOL  
<statement> → ε

<prog> → ID <params> EOL  
<prog> → ID = ID <params> EOL

<define> → print <args>  
<define> → inputs <args>  
<define> → inputi <args>  
<define> → inputf <args>  
<define> → lenght <value>  
<define> → substr <value> <value> <value>  
<define> → ord <value> <value>  
<define> → chr <value>  
<define> → <expression>  
<define> → ε

<args> → ε
<args> → (<args>)
<args> → <value> <args_n>  
<args_n> → , <value> <args_n>  
<args_n> → ε  

<value> → INT  
<value> → STRING  
<value> → FLOAT  
<value> → ID  
```
