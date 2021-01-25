
with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "devel";
  buildInputs = [
    binutils clang cmake gnumake ninja pkgconfig
    gtk3 xorg.libX11 freetype zlib
    curl ed
  ];

  shellHook = ''
    export XDG_DATA_DIRS=$XDG_ICON_DIRS:$GSETTINGS_SCHEMAS_PATH
  '';
}


