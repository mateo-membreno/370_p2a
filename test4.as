        lw      0       1       five
        lw      1       2       ten
start   add     1       2       3
        nor     0       3       4
        beq     0       4       done
        beq     0       0       start
        noop
done    halt
five   .fill   5
ten    .fill   10
