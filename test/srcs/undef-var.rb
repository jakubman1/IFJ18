#!/bin/ruby -r ./ifj18.rb
# Ukazka definice promenne bez provedeni jeji inicializace v IFJ18/Ruby

# Nasleduje mrtvy kod, ktery ma behem staticke analyzy vliv na tabulku symbolu a vytvori promennou x s hodnotou nil
# Takto zvlastne lze definovat promennou, aniz bychom ji inicializovali
if (1 == 2) then
  x = 42
else
end

# Kdybychom vypustil predchozi if-else-end, tak by pokud o pristup k x zpusobil chybu; zde ale bude nil
if x == nil then
  print "x je nil\n"
else
  print "x neni nil\n"
end
