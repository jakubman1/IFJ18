#!/usr/bin/env bash

################
# Define colors
################
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo -e "\n\n"
echo "Running valgrind"
echo "----------------------------------"
valgrind ./ifj18 < test/srcs/factorial.rb

###########################
# Tests for lexical analysis
###########################
echo -e "\n\n"
echo "Running tests for lexical analysis"
echo "----------------------------------"
for FILENAME in 'comments' 'factorial' 'factorial2' 'floats' 'lexerr' 'multiparam' 'strings' 'undef-var' # List of all test files
do
  ./lexical-test < test/srcs/$FILENAME.rb > test/tmp/$FILENAME.lexical.tmp 2> test/tmp/$FILENAME.lexical.error.tmp
  LEXOUT=$? # Output from lexical analysis
  DIFF=$(diff test/tmp/$FILENAME.lexical.tmp test/lexical/$FILENAME.lexical.output)
  if [ "$DIFF" != "" ] ; then
    echo -e "${RED}ERROR, ${FILENAME}.rb: lexical analysis not passed!${NC}"
    echo -e $DIFF
  else
    echo -e "${GREEN}Lexical analysis of ${FILENAME}.rb passed!${NC}"
  fi
  diff test/tmp/$FILENAME.lexical.error.tmp test/common/empty &> /dev/null
  ERRTEST=$? # 0 -> no difference, stderr was empty, LEXOUT should be 0. 1 -> there was a difference, LEXOUT should be 1 (lexical error)
  if [ $ERRTEST == $LEXOUT ]; then
    echo -e "${YELLOW}Return code correct! ($LEXOUT)${NC}\n"
  else
      echo -e "${RED}ERROR, ${FILENAME}.rb: lexical analysis did not return correct exit code! ($LEXOUT returned)${NC}\n"
  fi

  rm test/tmp/$FILENAME.lexical.tmp
  rm test/tmp/$FILENAME.lexical.error.tmp
done

###########################
# Tests for syntactic analysis
###########################
echo -e "\n\n"
echo "Running tests for syntactic analysis"
echo "----------------------------------"
#NOTE: Only checking for return codes. We could not figure out a simple way to test output from syntactic analysis.
for FILENAME in 'comments' 'factorial' 'factorial2' 'multiparam' 'strings' 'undef-var' # List of all correct files
do
  ./ifj18 < test/srcs/$FILENAME.rb > /dev/null
  SYNOUT=$?
  if [ $SYNOUT == 0 ]; then
    echo -e "${GREEN}Syntactic analysis of ${FILENAME}.rb returned $SYNOUT (passed)!${NC}"
  else
    echo -e "${RED}ERROR: Syntactic analysis of ${FILENAME}.rb returned $SYNOUT (should be 0)!${NC}"
  fi
done
for FILENAME in 'synerr1' 'synerr2' 'synerr3' 'synerr4' 'synerr5' 'synerr6' # List of all incorrect files
do
  ./ifj18 < test/srcs/$FILENAME.rb > /dev/null
  SYNOUT=$?
  if [ $SYNOUT == 2 ]; then
    echo -e "${GREEN}Syntactic analysis of ${FILENAME}.rb returned $SYNOUT (passed)!${NC}"
  else
    echo -e "${RED}ERROR: Syntactic analysis of ${FILENAME}.rb returned $SYNOUT (should be 2)!${NC}"
  fi
done
