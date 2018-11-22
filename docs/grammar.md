# LL gramatika

```
1. <prog> → def ID <params> EOL <statement> end EOL    
2. <prog> → <statement>  

3. <params> → (<params>)  
4. <params> → ID <params_n>  
5. <params> → ε  

6. <params_n> → , ID <params_n>  
7. <params_n> → ε  

8. <statement> → if <expression> then EOL <statement> else EOL <statement> end EOL <statement>  
9. <statement> → while <expression> do EOL <statement> end EOL <statement>  
10. <statement> → ID <def_args> EOL <statement> <statement>  
11. <statement> → ε  

12. <def_args> → = <define>  
13. <def_args> → <args>  

14. <define> → ID <args>  
15. <define> → print <args>  
16. <define> → inputs <args>  
17. <define> → inputi <args>  
18. <define> → inputf <args>  
19. <define> → lenght <args>  
20. <define> → substr <args>   
21. <define> → ord <args>    
22. <define> → chr <args>  
23. <define> → <expression>  
24. <define> → ε  

25. <args> → ε  
26. <args> → (<args>)  
27. <args> → <value> <args_n>  

28. <args_n> → , <value> <args_n>  
29. <args_n> → ε   

30. <value> → INT  
31. <value> → FLOAT  
32. <value> → ID  
33. <value> → STRING   
```
