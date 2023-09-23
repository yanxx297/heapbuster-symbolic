# Heapbuster-symbolic
Symbolic execution directed by a heap operation sequence.

This repo is for documentation and extra tools. 
Below are the main work and related repos.
- [FuzzBALL](https://github.com/yanxx297/fuzzball/tree/hb-branch) heapbuster branch
- A modified version of [libxml2-2.93](https://github.com/yanxx297/libxml2). Changes have been made to generate heap operation trace and to simplify bug finding.

## TODO
- Figure out why op_trace[60] is unreachable even if ELEMENT check has been made easy
    - xmlParseInternalSubset() after DOCTYPE check and DocType name parsed
    - Reproduce: run xmllint.sh, keep running for at least 20,000 Iterations


## Reference

### Breakfast example
A simple example modified from the breakfast example in [heap-challenges](https://github.com/soez/heap-challenges)
The example is in FuzzBALL repo with a running script.

To run this example
```bash
cd examples/breakfast
./breakfast-bp-loop.sh
```

### Reproduce AAH041 by symbolic execution

AAH041(or XML017 in the [magma paper](https://hexhive.epfl.ch/publications/files/21SIGMETRICS.pdf)) is a known bug found in libxml2-2.93. 

At the end of a DOCTYPE DTD element, if there is no '>', `xmlParseDocTypeDecl` will run `NEXT`(xmlNextChar) again and cause heap overread.
To fix this bug, `xmlParseDocTypeDecl` should return in this case.

Find more details at [CVE-2016-1762](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2016-1762) and the [fix](https://gitlab.gnome.org/GNOME/libxml2/-/commit/a7a94612aa3b16779e2c74e1fa353b5d9786c602).

#### Reproduce the bug with given exploit
```bash
valgrind ./xmllint c01_id0051.xml
```

#### Dump the trace

in gdb, break at xmlNextChar and keep c until message `DOCTYPE improperly terminated
` appear
```bash
dump binary memory xmllint.trace 0x1e34b00 0x1e35340

# 0x1e34b00 is the location of op_trace
# get it by ``print op_trace``
# 0x1e35340 = 0x1e34b00 + 16*132, where 12 is the size of memory_op and 132 is the real length of op_trace
```

#### Run xmllint under FuzzBALL
There are several FuzzBALL running scripts under `examples/breakfast`
- xmllint.sh: symbolic execution
- xmllint-concrete.sh: concrete execution, can be used as a sanity check
- xmllint-conc.sh: concolic execution
