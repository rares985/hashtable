# Hashtable

A C-implementation of a hashtable that contains strings. The program can take commands
from stdin or from files given to it as command-line parameters.
The program must take a mandatory argument, a positive integer which is the size of the hashtable(can be changed later),
and can take a variable amount of files which may contain commands as input. Examples of running:
./hashtable 20 - taking commands from stdin
./hashtable 10 file1 file2 - reading commands from file1 and file2.

## Permitted commands:
  * _help_ - Prints a list of accepted commands
  * _add <word>_ - Adds a word to the hashtable
  * _remove <word>_ - Removes a word from the hashtable
  * _print_bucket <index> [file]+ - Prints the bucket with the corresponding index
  * _print [file]+ - Prints the whole hashtable
  * _resize <halve/double>+ - Resizes the hashtable and moves all entries
  <arg> means mandatory argument, whereas [arg] means optional argument
