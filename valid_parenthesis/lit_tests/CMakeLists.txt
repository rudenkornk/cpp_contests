configure_file(lit.site.cfg.py.in lit.site.cfg.py @ONLY)

add_test(NAME valid_parenthesis_lit_tests COMMAND lit --verbose .)
add_test(NAME valid_parenthesis_lit_tests_valgrind COMMAND lit --verbose --vg .)
