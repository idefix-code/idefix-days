# Using the GPU environement in bigfoot
## Connecting to bigfoot
Let's first connect to bigfoot via the frontend

```shell
ssh <yourlogin>@rotule.univ-grenoble-alpes.fr
```
Then
```shell
ssh <yourlogin>@bigfoot
```

## First connection: clone what you need
During the workshop, we will need the idefix days repository on the cluster, so let's clone it in your home:
```shell
git clone https://github.com/idefix-code/idefix-days.git ~/idefix-days

```

## Connect to a compute node, load the environement, compile and run

First, note that the compute nodes are shared between all of us during the workshop. Hence, we limit each reservation to 30 minutes so that everybody can experiment both architectures (we have booked 16 Nvidia GPUs and 8 AMD GPUs).

[Gricad](https://gricad.univ-grenoble-alpes.fr), the infrastructure hosting Grenoble GPU compute nodes, uses a homemade job scheduler called OAR. Follow the instructions below to request an interactive node with OAR and then activate the compilation environement.

<details><summary>I want to play in the green team (Nvidia)</summary>
If a reservation is available (wednesday afternoon only):

```shell
oarsub -t inner=618673 -l nodes=1/gpu=1,walltime=0:30:0 -p "gpumodel='V100'" --project idefix-23 -I
```

... or without reservation (every other day):

```shell
oarsub -l nodes=1/gpu=1,walltime=0:30:0 -p "gpumodel='V100'" --project idefix-23 -I
```

You must then source the Nvidia environement

```shell
source ~/idefix-days/env/nvidia-gpu.sh
```

Then, you can compile the setup you want. Simply go to the setup directory of your choice (follow the tutorial), then configure the setup for your target:
```shell
cmake $IDEFIX_DIR $NVIDIA_FLAGS
```
and compile:
```shell
make -j 4
```
... and run!

```shell
./idefix
```
</p>
</details>
<details><summary>I want to play in the red team (AMD)</summary>
If a reservation is available (wednesday only):

```shell
oarsub -t inner=618674 -l nodes=1/gpu=1,walltime=0:30:0 -t amd --project idefix-23 -I
```

... or without reservation (every other day):

```shell
oarsub -l nodes=1/gpu=1,walltime=0:30:0 -t amd --project idefix-23 -I
```

You must then source the AMD environement

```shell
source ~/idefix-days/env/amd-gpu.sh
```

Then, in order to configure, compile or run the code, you need to use a particular environement with specific glibc and compiler. This is all done for you with the simple command

```shell
amd_shell
```

Note that you now enter a nix shell, with the proper AMD environement. From this point, you can configure and compile the setup of your choice. Simply go to the setup directory of your choice (follow the tutorial), then configure the setup for your target:

```shell
cmake $IDEFIX_DIR $AMD_FLAGS
```
and compile:
```shell
make -j 4
```
... and run!

```shell
./idefix
```
</p>
