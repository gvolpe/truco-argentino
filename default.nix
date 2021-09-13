let
  pkgs = import (
    builtins.fetchTarball {
      name   = "NixOS-unstable-30-12-2020";
      url    = "https://github.com/NixOS/nixpkgs/archive/733e537a8ad7.tar.gz";
      sha256 = "1rjvbycd8dkkflal8qysi9d571xmgqq46py3nx0wvbzwbkvzf7aw";
    }
  ) {};
in
  with pkgs;

  stdenv.mkDerivation rec {
    name = "truco";

    src = ./source;

    buildPhase = ''
      g++ main.cpp -o truco-exe `${SDL_sixel}/bin/sdl-config --libs --cflags` -lSDLmain -lSDL_gfx -lSDL_image -lSDL_ttf
    '';

    nativeBuildInputs = [
      gpp
    ];

    buildInputs = [
      freetype
      libpng12
      SDL
      SDL_gfx
      SDL_image
      SDL_ttf
      zlib
    ];

    libpath = lib.makeLibraryPath [
      stdenv.cc.cc
      stdenv.cc.libc
      SDL
      SDL_gfx
      SDL_image
      SDL_ttf
    ];

    installPhase = ''
      mkdir -p $out/bin/img
      cp -r img/* $out/bin/img
      cp -r ./*.doc $out/bin/
      cp -r ./*.ttf $out/bin/
      patchelf \
        --set-interpreter $(cat ${stdenv.cc}/nix-support/dynamic-linker) \
        --set-rpath "${libpath}" \
        truco-exe
      cp truco-exe $out/bin/truco
    '';

    meta = with stdenv.lib; {
      description = "Truco Argentino";
      homepage = "https://github.com/gvolpe/truco-argentino";
      license = licenses.gpl3;
      platforms = platforms.linux;
      maintainers = [ maintainers.gvolpe ];
    };
  }
