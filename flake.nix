{
  inputs = {
    nixpkgs = {
      url = "github:nixos/nixpkgs/nixos-unstable";
    };
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };

      # The gcc whose libstdc++ the nixpkgs clang wrapper compiles and links against. Its `libstdc++.modules.json`
      # manifest and `backward/` header directory are what CMake needs to build the `import std` module under Clang.
      # `clang++ -print-file-name=...` cannot discover them because the wrapper injects libstdc++ through wrapper flags,
      # so we surface the paths explicitly through the environment (consumed in the root and test_install CMakeLists).
      gccForClang = pkgs.stdenv.cc.cc;
      importStdEnv = {
        IMPORT_STD_MANIFEST = "${gccForClang}/lib/libstdc++.modules.json";
        IMPORT_STD_BACKWARD_INC = "${gccForClang}/include/c++/${gccForClang.version}/backward";
      };

      # `import std` under Clang fails to build with fortification enabled: glibc's clang-fortify `printf`-family
      # overloads have internal linkage and cannot be re-exported by `export module std`. nixpkgs turns on
      # `_FORTIFY_SOURCE` by default, and a plain `-U_FORTIFY_SOURCE` does not help because the cc-wrapper appends its
      # define after user flags, so the fortify feature must be disabled at the toolchain level instead.
      importStdHardeningDisable = [
        "fortify"
        "fortify3"
      ];

      nativeBuildInputs = with pkgs; [
        clang
        gcc15

        cmake
        ninja
      ];

      buildInputs = with pkgs; [
        boost
      ];

      devTools = with pkgs; [
        ccache
        gcovr
        llvm
        valgrind
      ];

      linters = with pkgs; [
        clang-tools
        cmake-format
        cmake-lint
        nixfmt
        prettier
        typos
        yamllint
      ];

    in
    {
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        pname = "cpp_contests";
        version = "0.1.0";

        src = ./.;

        inherit nativeBuildInputs buildInputs;

        # See `importStdHardeningDisable` above. The package builds with gcc, which tolerates fortify, but disabling it
        # keeps the package consistent with the devshell and future-proofs a switch to a Clang-based stdenv.
        hardeningDisable = importStdHardeningDisable;

        env = importStdEnv;

        cmakeFlags = [
        ];

        doCheck = true;

        meta = with pkgs.lib; {
          description = "Repository with solutions for different C++ educational problems.";
          homepage = "https://github.com/rudenkornk/cpp_contests";
          license = licenses.mit;
          platforms = platforms.linux;
        };
      };

      devShells.${system}.default = pkgs.mkShell (
        importStdEnv
        // {
          packages = nativeBuildInputs ++ buildInputs ++ devTools ++ linters;
          # See `importStdHardeningDisable` above; the cc-wrapper reads this via NIX_HARDENING_ENABLE in the devshell.
          hardeningDisable = importStdHardeningDisable;
          shellHook = ''
            echo "Welcome to the project devshell!"
          '';
        }
      );

      checks.${system}.default = self.packages.${system}.default;
    };
}
