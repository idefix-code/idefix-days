. /applis/site/nix.sh
NIX_PATH="nixpkgs=channel:nixpkgs-unstable"
export IDEFIX_DIR=/bettik/PROJECTS/pr-idefix-23/COMMON/src/idefix
export KOKKOS_TOOLS_DIR=/bettik/PROJECTS/pr-idefix-23/COMMON/bin/amd/kokkos-tools
alias amd_shell='nix-shell  -p rocm-smi -p hip -p cmake -p cmakeCurses'
export AMD_FLAGS="-DKokkos_ENABLE_HIP=ON -DKokkos_ARCH_VEGA90A=ON -DCMAKE_CXX_COMPILER=hipcc"
git config --global --add safe.directory /bettik/PROJECTS/pr-idefix-23/COMMON/src/idefix/src/kokkos
