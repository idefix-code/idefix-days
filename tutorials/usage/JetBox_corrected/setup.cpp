#include "idefix.hpp"
#include "setup.hpp"


// Default constructor



void JetBoundary(Hydro* hydro, int dir, BoundarySide side, const real t)  {
  IdefixArray4D<real> Vc = hydro->Vc;
  IdefixArray1D<real> x1 = hydro->data->x[IDIR];

  if(dir==JDIR) { // BC in radial direction (at r_0 and R)


    if(side == left) {  // au début du domaine : r0
      int ighost = hydro->data->nghost[IDIR];

      hydro->boundary->BoundaryFor("UserDefBoundaryX2Beg", dir, side,
                        KOKKOS_LAMBDA (int k, int j, int i) {

              real x=x1(i);

              if (fabs(x-0.5) < 0.05){
                Vc(RHO,k,j,i) = 10.;
                Vc(VX2,k,j,i) = 10;
                Vc(VX1,k,j,i) = Vc(VX1,k,j,ighost);
              } else {
                Vc(RHO,k,j,i) = Vc(RHO,k,j,ighost);
                Vc(VX2,k,j,i) = Vc(VX2,k,j,ighost);
                Vc(VX1,k,j,i) = Vc(VX1,k,j,ighost);
              }

    });
    
    }
  }
}
// Initialisation routine. Can be used to allocate
// Arrays or variables which are used later on
Setup::Setup(Input &input, Grid &grid, DataBlock &data, Output &output) {
    data.hydro->EnrollUserDefBoundary(&JetBoundary);
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
                d.Vc(VX1,k,j,i) = ZERO_F;
                d.Vc(VX2,k,j,i) = ZERO_F;
            }
        }
    }

    // Send it all, if needed
    d.SyncToDevice();
}
