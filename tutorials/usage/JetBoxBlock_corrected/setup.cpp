#include "idefix.hpp"
#include "setup.hpp"


void BlockInternal(Hydro *hydro, const real t) {
  auto *data = hydro->data;
  IdefixArray4D<real> Vc = hydro->Vc;
  IdefixArray1D<real> x1=data->x[IDIR];
  IdefixArray1D<real> x2=data->x[JDIR];

  real x_c,y_c,size;

  x_c = 0.5;
  y_c = 2;
  size = 0.1;

  idefix_for("InternalBoundary",
    0, data->np_tot[KDIR],
    0, data->np_tot[JDIR],
    0, data->np_tot[IDIR],
              KOKKOS_LAMBDA (int k, int j, int i) {

                if(fabs(x1(i)-x_c )< size && fabs(x2(j)- y_c ) < size) {
                  Vc(RHO,k,j,i) = 1.0;
                  Vc(VX1,k,j,i) = 0.0;
                  Vc(VX2,k,j,i) = 0.0;
                }

              });

}



void JetBoundary(Hydro* hydro, int dir, BoundarySide side, const real t)  {
  IdefixArray4D<real> Vc = hydro->Vc;
  IdefixArray1D<real> x1 = hydro->data->x[IDIR];

  if(dir==JDIR) {

    if(side == left) {
      int jghost = hydro->data->nghost[JDIR]; // first active cell of the domain


      hydro->boundary->BoundaryFor("UserDefBoundaryX2Beg", dir, side,
                        KOKKOS_LAMBDA (int k, int j, int i) {

              real x=x1(i);

              if (fabs(x-0.5) < 0.05){ // inside the jet
                Vc(RHO,k,j,i) = 10.;
                Vc(VX2,k,j,i) = 10;
                Vc(VX1,k,j,i) = Vc(VX1,k,jghost,i);
              } else { // otherwise we copy the state of the first active cell
                Vc(RHO,k,j,i) = Vc(RHO,k,jghost,i);
                Vc(VX2,k,j,i) = Vc(VX2,k,jghost,i);
                Vc(VX1,k,j,i) = Vc(VX1,k,jghost,i);
              }

    });

    }
  }
}

// Initialisation routine. Can be used to allocate
// Arrays or variables which are used later on
Setup::Setup(Input &input, Grid &grid, DataBlock &data, Output &output) {
    data.hydro->EnrollUserDefBoundary(&JetBoundary);
    data.hydro->EnrollInternalBoundary(&BlockInternal);
}




// This routine initialize the flow
// Note that data is on the device.
// One can therefore define locally
// a datahost and sync it, if needed
void Setup::InitFlow(DataBlock &data) {
    // Create a host copy
    DataBlockHost d(data);


    for(int k = 0; k < d.np_tot[KDIR] ; k++) {
        for(int j = 0; j < d.np_tot[JDIR] ; j++) {
            for(int i = 0; i < d.np_tot[IDIR] ; i++) {
                d.Vc(RHO,k,j,i) = ONE_F;
                d.Vc(VX1,k,j,i) = (0.5-idfx::randm())*0.2;
                d.Vc(VX2,k,j,i) = (0.5-idfx::randm())*0.2;
            }
        }
    }

    // Send it all, if needed
    d.SyncToDevice();
}
