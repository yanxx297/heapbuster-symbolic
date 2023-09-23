# Tools

## FuzzBALL log to csv
Before running parser.py, preprocess the log so that it only contains necessary data. 
```bash
cat xmllint-sym.log |grep -e "Iteration\|Achieved score\|Input"| grep -B 2 Iteration|tee output
```
Use parser.py to convert processed log to a csv file.
```bash
python3 parser.py output |tee log.csv
```

## Split log by Iteration
FuzzBALL log can be huge when running a large number of paths.
To make it a bit easier to read, use csplit to split the log file by Iteration(path).

```bash
csplit -f xmllint-sym -b -%05d.log xmllint-sym.log -z '/Iteration/' '{*}'
```

## malloc.gdb
A simple gdb script that print stack backtrace whenever malloc is hit.

To use it, load with `-x` while running gdb.
```bash
gdb -x malloc.gdb --args ./xmllint c01_id0051.xml
```