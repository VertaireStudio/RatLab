# Filepath Creator
# Lists all important filepaths to a single text file.
# Recommended for LLMs to find files this way.
# Note: Only treat the output as up-to-date when the script has recently ran.

#!/bin/bash
find . -type f \
  -not -path './.git/*' \
  > ./Misc/filelist.txt # Starting path is from the terminal's path.
