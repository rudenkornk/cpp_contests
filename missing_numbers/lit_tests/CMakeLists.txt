set(target_name missing_numbers_cli)
set(target_relative_dir missing_numbers)
configure_file(lit.site.cfg.py.in lit.site.cfg.py @ONLY)

add_test(NAME ${target_name}_lit_tests COMMAND lit --verbose .)
add_test(NAME ${target_name}_lit_tests_valgrind COMMAND lit --verbose --vg .)

