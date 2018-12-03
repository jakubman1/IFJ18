for FILENAME in 'comments' 'factorial' 'factorial2' 'floats' 'lexerr' 'multiparam' 'strings' 'undef-var' # List of all test files
do
  ./lexical-test.exe < tests/srcs/$FILENAME.rb > tests/lexical/$FILENAME.lexical.output
done
