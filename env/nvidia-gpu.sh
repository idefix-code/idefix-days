#install guix environement for cuda
source /applis/site/guix-start.sh
guix install cmake
source /applis/environments/cuda_env.sh bigfoot 11.2
export IDEFIX_DIR=~/src/idefix
export NVIDIA_FLAGS="-DKokkos_ENABLE_CUDA=ON -DKokkos_ARCH_AMPERE80=ON"
