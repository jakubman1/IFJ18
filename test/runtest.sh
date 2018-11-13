#!/bin/sh
../debug < test.rb > test.rb.lexical.tmp
DIFF=$(diff test.rb.lexical.tmp test.rb.lexical.output)
if [ "$DIFF" != "" ] ; then
  echo "ERROR, test.rb lexical analysis not passed!"
  echo $DIFF
else
  echo "Lexical analysis passed"
fi
