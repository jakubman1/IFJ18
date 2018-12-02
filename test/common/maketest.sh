for FILENAME in 'comments' 'factorial' 'factorial2' 'floats' 'lexerr' 'multiparam' 'strings' 'undef-var' # List of all test files
do
  ./lexical-test.exe < test/srcs/$FILENAME.rb > test/lexical/$FILENAME.lexical.output
done
