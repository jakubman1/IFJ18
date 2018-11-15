# LL gramatika

<def_function> -> def functionID (<params>) EOL
<params> -> ID, <params>
<params> -> ε

<statement> -> if <expression> then EOL else EOL
<statement> -> ID = <def_value>
<statement> -> while <expression> do EOL <statement> end EOL

<type> INT
<type> STRING
<type> FLOAT
