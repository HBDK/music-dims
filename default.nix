with import <nixpkgs> {config.allowUnfree = true;};
pkgs.mkShell rec {
  dotnetPkg = 
    (with dotnetCorePackages; combinePackages [
      sdk_10_0
    ]);

  deps = [
    zlib
    zlib.dev
    openssl
    dotnetPkg
  ];

  NIX_LD_LIBRARY_PATH = lib.makeLibraryPath ([
    stdenv.cc.cc
  ] ++ deps);
  NIX_LD = "${pkgs.stdenv.cc.libc_bin}/bin/ld.so";
  nativeBuildInputs = [ 
  ] ++ deps;
  packages = with pkgs; [
    jetbrains.rider
  ];
  shellHook = ''
    DOTNET_ROOT="${dotnetPkg}";
  '';
}
