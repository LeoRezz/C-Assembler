#!/bin/bash

# Create src directory if it doesn't exist
mkdir -p src

# Move all .c and .h files from mamman14_2.0 to src
mv mamman14_2.0/*.c mamman14_2.0/*.h src/

# Move other necessary files from mamman14_2.0 to src
mv mamman14_2.0/makefile src/
mv mamman14_2.0/Invalid_source_code.txt mamman14_2.0/source_code.txt src/

# Move executable files to src
mv mamman14_2.0/firstPass mamman14_2.0/tok_tester mamman14_2.0/tokenizer src/


# Move any remaining .c or .h files from the root to src
mv *.c *.h src/ 2>/dev/null

echo "Repository reorganization complete!"