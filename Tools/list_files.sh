# Filepath Creator
# Lists all important filepaths to a single text file.
# Recommended for LLMs to find files this way.

#!/bin/bash
find . -type f \
  -not -path './bazel-bin/*' \
  -not -path './bazel-out/*' \
  -not -path './bazel-RatLab/*' \
  -not -path './bazel-testlogs/*' \
  -not -path './.git/*' \
  > ./Misc/filelist.txt # Starting path is from the terminal's path.
