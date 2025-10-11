{ pkgs ? import <nixpkgs> {} }:

(pkgs.buildFHSEnv {
  name = "rider-env";
  targetPkgs = pkgs: (with pkgs; [
    dotnetCorePackages.dotnet_10.sdk
    dotnetCorePackages.dotnet_10.aspnetcore
    powershell
    jetbrains.rider
  ]);
  multiPkgs = pkgs: (with pkgs; [
  ]);
  runScript = "nohup rider &";
}).env
