# sqrtf, game_libs copy -- libultra handwritten assembly (gu/sqrtf.s:
# `sqrt.s $f0,$f12; jr ra` assembled with reorder, so the sqrt.s lands in the
# jr delay slot). Reference: references/libreultra/src/gu/sqrtf.s.
# The two zero words before it (0x6E20C/0x6E210) are the previous object's
# tail pad (gl_func_0006E1A4 all-zero SUFFIX_BYTES_FORCE); the two after it
# are this object's pad (_pad_gl_func_0006E1A4 sidecar in the host unit).

glabel game_libs_func_0006E214
    jr      $ra
     sqrt.s  $f0, $f12
endlabel game_libs_func_0006E214
