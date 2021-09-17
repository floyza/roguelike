{
  description = "Haskell Nix-Shell";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        overlays = [
          (final: prev: {
            libtcod = prev.libtcod.overrideAttrs (oldAttrs: rec {
              version = "1.18.1";
              # version = "1.16.2";
              src = prev.fetchFromGitHub {
                owner = "libtcod";
                repo = "libtcod";
                rev = "40cfd7c47bbd12b5f0845befbe9a156c5514e233";
                sha256 = "sha256-JiG89rMgxxj+FpulTGx5iXIlIheGde5CtwmTiP2Qtl4=";
                # rev = "fa08b426d73e78f31a9f14ee42c88aa9ea7d6675";
                # sha256 = "sha256-e5VD4Ta5uah3V3D0aGG+S7YgUHlXo5pS+SOIWprVUZk=";
              };
              prePatch = "";
              buildInputs = with prev; [ SDL2 libGLU libGL upx zlib ];
            });
          })
        ];
        pkgs = import nixpkgs { inherit system overlays; };
      in {
        devShell = pkgs.mkShell {
          buildInputs =
            [ pkgs.libtcod pkgs.lua pkgs.boost pkgs.nlohmann_json pkgs.SDL2 ];
          LD_LIBRARY_PATH = "${pkgs.libtcod}/lib";
        };
      });
}
