        lw      2       3       fift
        lw      3       4       five
start   add     3       4       5
        lw      5       6       result
        beq     6       5       end
        beq     0       0       start
        noop
end    halt
result .fill   20
fift .fill   15
five   .fill   5
