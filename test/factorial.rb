# Program 2: Vypocet faktorialu (rekurzivne)
hdshafjjgsdfhsghsfdjghjsdffjgfhdsjjdfghskhghsfdkhg = 9
def factorial (n)
if n < 2 then
result = 1
else
decremented_n = n - 1
temp_result = factorial decremented_n
result = n * temp_result
end # if
result
end # function factorial
# Hlavni telo programu

print "Zadejte cislo pro vypocet faktorialu "
a = inputi
if a < 0 then
print "\nFaktorial nelze spocitat\n"
else
vysl = factorial a
print("\nVysledek je:", vysl, "\n")
end
