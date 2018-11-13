#!/bin/sh
########
# Define colors
########
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

########
# Test for lexical analysis
########
#factorial.rb
./lexical-test < test/factorial.rb > factorial.lexical.tmp
DIFF=$(diff factorial.lexical.tmp test/factorial.lexical.output)
if [ "$DIFF" != "" ] ; then
  echo "${RED}ERROR, factorial.rb lexical analysis not passed!${NC}"
  echo $DIFF
else
  echo "${GREEN}Lexical analysis of factorial.rb passed${NC}"
fi

#comments.rb
./lexical-test < test/comments.rb > comments.lexical.tmp
DIFF=$(diff comments.lexical.tmp test/comments.lexical.output)
if [ "$DIFF" != "" ] ; then
  echo "${RED}ERROR, comments.rb lexical analysis not passed!${NC}"
  echo $DIFF
else
  echo "${GREEN}Lexical analysis of factorial.rb passed${NC}"
fi
