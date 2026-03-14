{
  inputs = {
    nixpkgs = {
      url = "github:nixos/nixpkgs/nixos-25.11";
    };
    nur = {
      url = "github:nix-community/nur";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };

      nativeBuildInputs = with pkgs; [
        clang
        gcc15

        cmake
        ninja
      ];

      buildInputs = with pkgs; [
        boost
        fmt
      ];

      devTools = with pkgs; [
        ccache
        gcovr
        llvm
        valgrind
      ];

      linters = with pkgs; [
        cmake-format
        cmake-lint
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

      devShells.${system}.default = pkgs.mkShell {
        packages = nativeBuildInputs ++ buildInputs ++ devTools ++ linters;
        shellHook = ''
          echo "Welcome to the project devshell!"
        '';
      };
    };
}
