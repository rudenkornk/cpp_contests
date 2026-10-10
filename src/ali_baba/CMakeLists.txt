add_executable(ali_baba_cli)
target_sources(
  ali_baba_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(ali_baba_cli PUBLIC cxx_std_23)
set_target_properties(ali_baba_cli PROPERTIES OUTPUT_NAME ali_baba)
target_setup_for_install(ali_baba_cli)
target_allow_instrumentation(ali_baba_cli)
target_allow_coding_standards(ali_baba_cli)

add_subdirectory(tests)
