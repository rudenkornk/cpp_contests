add_executable(football_team_cli)
target_sources(
  football_team_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(football_team_cli PUBLIC cxx_std_23)
set_target_properties(football_team_cli PROPERTIES OUTPUT_NAME football_team)
target_setup_for_install(football_team_cli)
target_allow_instrumentation(football_team_cli)
target_allow_coding_standards(football_team_cli)

add_subdirectory(tests)
