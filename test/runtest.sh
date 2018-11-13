#!/bin/sh

########
# Test for lexical analysis
########
#factorial.rb
./lexical-test < test/factorial.rb > factorial.lexical.tmp
DIFF=$(diff factorial.lexical.tmp test/factorial.lexical.output)
if [ "$DIFF" != "" ] ; then
  echo "ERROR, factorial.rb lexical analysis not passed!"
  echo $DIFF
else
  echo "Lexical analysis of factorial.rb passed"
fi

#comments.rb
./lexical-test < test/comments.rb > comments.lexical.tmp
DIFF=$(diff comments.lexical.tmp test/comments.lexical.output)
if [ "$DIFF" != "" ] ; then
  echo "ERROR, comments.rb lexical analysis not passed!"
  echo $DIFF
else
  echo "Lexical analysis of factorial.rb passed"
fi
