set(test_target valid_parenthesis_cli)
set(target_relative_dir valid_parenthesis)
configure_file(lit.site.cfg.py.in lit.site.cfg.py @ONLY)

add_test(NAME ${test_target}_lit_tests COMMAND lit --verbose .)
add_test(NAME ${test_target}_lit_tests_valgrind COMMAND lit --verbose --vg .)
