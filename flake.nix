{
  description = "A very basic flake";

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
  in {
    packages.${system} = {
      hello = nixpkgs.legacyPackages.${system}.hello;
      default = self.packages.${system}.hello;
    };

    devShells.${system} = {
      default = with pkgs;
        mkShell {
          packages = [
            meson
            ninja
            clang-tools
            cppcheck
            lldb
            pkg-config
            SDL2
            SDL2_mixer
          ];
        };
    };
  };
}
