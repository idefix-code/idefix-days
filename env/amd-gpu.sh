. /applis/site/nix.sh
NIX_PATH="nixpkgs=channel:nixpkgs-unstable"
export IDEFIX_DIR=~/src/idefix
alias idefix_shell='nix-shell  -p rocm-smi -p hip -p cmake -p cmakeCurses'
export AMD_FLAGS="-DKokkos_ENABLE_HIP=ON -DKokkos_ARCH_VEGA90A=ON -DCMAKE_CXX_COMPILER=hipcc"
