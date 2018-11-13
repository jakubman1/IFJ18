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
echo -e "\n\n"
echo "Running tests for lexical analysis"
echo -e "\n"
# List of all files
for FILENAME in 'factorial' 'comments'
do
  ./lexical-test < test/$FILENAME.rb > $FILENAME.lexical.tmp
  DIFF=$(diff $FILENAME.lexical.tmp test/$FILENAME.lexical.output)
  if [ "$DIFF" != "" ] ; then
    echo -e "${RED}ERROR, ${FILENAME}.rb lexical analysis not passed!${NC}"
    echo $DIFF
  else
    echo -e "${GREEN}Lexical analysis of ${FILENAME}.rb passed${NC}"
  fi
done
