flite_hts_engine \
     -td tree-dur.inf -tf tree-lf0.inf -tm tree-mgc.inf \
     -md dur.pdf         -mf lf0.pdf       -mm mgc.pdf \
     -df lf0.win1        -df lf0.win2      -df lf0.win3 \
     -dm mgc.win1        -dm mgc.win2      -dm mgc.win3 \
     -cf gv-lf0.pdf      -cm gv-mgc.pdf    -ef tree-gv-lf0.inf \
     -em tree-gv-mgc.inf -k  gv-switch.inf -ow output.wav \
     input.txt
