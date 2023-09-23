set width 0
set height 0
set verbose off

break wrap_malloc
commands 1
  p size
  p trace_idx
  backtrace
  continue
end

run
