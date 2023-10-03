#install guix environment for cuda
# source /applis/site/guix-start.sh
export PATH=$PATH:/gnu/store/1w01prf09gi6d9i976n1r95lj70yq4sd-cmake-3.21.4/bin
source /applis/environments/cuda_env.sh bigfoot 11.2
export IDEFIX_DIR=/bettik/PROJECTS/pr-idefix-23/COMMON/src/idefix
export KOKKOS_TOOLS_DIR=/bettik/PROJECTS/pr-idefix-23/COMMON/bin/nvidia/kokkos-tools
export NVIDIA_FLAGS="-DKokkos_ENABLE_CUDA=ON -DKokkos_ARCH_VOLTA70=ON"
git config --global --add safe.directory /bettik/PROJECTS/pr-idefix-23/COMMON/src/idefix/src/kokkos
