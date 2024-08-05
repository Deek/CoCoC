# translate name: into name EXPORT followed by name
#s/^\([a-zA-Z_][a-zA-Z0-9$_]\+\):/\1 EXPORT\n\1/
s/^\([a-zA-Z_][a-zA-Z0-9$_]\+\)/_\1 EXPORT\n\1 EXPORT\n_\1\n\1/
# for macOS:
s/^\([a-zA-Z_][a-zA-Z0-9$_]*\):/\1 EXPORT\
_\1 EXPORT\n_\1\n\1/
# translate sections
s/^\([[:space:]]*\)psect.*/ SECTION code/
s/^\([[:space:]]*\)vsect.*/ SECTION bss/
s/^\([[:space:]]*\)csect[[[:space:]]]\+\(.*\)/ SECTION _constant\n org \1/
s/^\([[:space:]]*\)csect/ SECTION _constant/
s/^\([[:space:]]*\)endsect.*/ ENDSECT\n SECTION code/
s/^\([[:space:]]*\)ends.*/ ENDSECT\n SECTION code/
# convert fail / warn directives
s/^\([[:space:]]*\)fail\(.*\)/ ERROR\1/
s/^\([[:space:]]*\)warn\(.*\)/ WARNING\1/