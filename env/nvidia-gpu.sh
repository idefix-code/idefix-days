#install guix environement for cuda
source /applis/site/guix-start.sh
guix install cmake
source /applis/environments/cuda_env.sh bigfoot 11.2
export IDEFIX_DIR=/bettik/PROJECTS/pr-idefix-23/COMMON/src/idefix
export KOKKOS_TOOLS_DIR=/bettik/PROJECTS/pr-idefix-23/COMMON/bin/nvidia/kokkos-tools
export NVIDIA_FLAGS="-DKokkos_ENABLE_CUDA=ON -DKokkos_ARCH_VOLTA70=ON"
