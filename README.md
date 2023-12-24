# CLI Search
This program is a command line tool written in C that allows users to search for a word within an input file and replace all occurrences of that word with another specified word in an output file.

## Features

- **Search and Replace**: Efficiently finds and replaces a specified word within user-defined lines of an input file.
- **Line-Based Filter**: Allows users to specify line ranges for search and replace operations of the input file.
- **Command Line Interface**: Operates through a command line interface, enabling easy input of parameters.
- **File Handling**: Reads from an input file, performs search and replace operations based on specified criteria, and writes the modified content to an output file.

## Command Line Prompts
-s: Search word
-r: Replacement word
-l: Optional - specify line range for search/replace operation  
-w: Optional - wildcard search
`*Order of these prompts do not matter, but input.txt and output.txt **must** be the second to last and last arguments, respectively*`

### Examples
- -s the* -w -r end -l 10,15 input.txt output.txt
- -s the -r end -l 10,15 input.txt output.txt
