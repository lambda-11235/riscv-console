
with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "devel";
  buildInputs = [
    binutils clang cmake gnumake ninja pkgconfig
    gtk3 xorg.libX11 freetype zlib
    curl ed
  ];
}


