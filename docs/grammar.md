# LL gramatika

```
1. <prog> → def ID <params> EOL <statement_list> end EOL    
2. <prog> → <statement_list>

3. <params> → (<params>)  
4. <params> → ID <params_n>  
5. <params> → ε  

6. <params_n> → , ID <params_n>  
7. <params_n> → ε  

8. <statement_list> → <statement> EOL <statement_list>
9. <statement_list> → ε

10. <statement> → if <expression> then EOL <statement_list> else EOL <statement_list> end EOL  
11. <statement> → while <expression> do EOL <statement_list> end EOL  
12. <statement> → ID = <define> EOL  
13. <statement> → <define> EOL  
14. <statement> → ε

15. <define> → ID <args>
16. <define> → print <args>  
17. <define> → inputs <args>
18. <define> → inputi <args>
19. <define> → inputf <args>
20. <define> → lenght <args>
21. <define> → substr <args>   
22. <define> → ord <args>  
23. <define> → chr <args>  
24. <define> → <expression>  
25. <define> → ε

26. <args> → ε
27. <args> → (<args>)
28. <args> → <value> <args_n>

29. <args_n> → , <value> <args_n>  
30. <args_n> → ε  

31. <value> → INT  
32. <value> → STRING  
33. <value> → FLOAT  
34. <value> → ID  
```
