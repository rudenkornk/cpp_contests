configure_file(lit.site.cfg.py.in lit.site.cfg.py @ONLY)

add_test(NAME missing_numbers_lit_tests COMMAND lit --verbose .)
add_test(NAME missing_numbers_lit_tests_valgrind COMMAND lit --verbose --vg .)

