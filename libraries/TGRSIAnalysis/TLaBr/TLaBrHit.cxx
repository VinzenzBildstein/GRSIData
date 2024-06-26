#include "TLaBrHit.h"

#include <iostream>
#include <algorithm>
#include <climits>

#include "Globals.h"
#include "TLaBr.h"

/// \cond CLASSIMP
ClassImp(TLaBrHit)
/// \endcond

TLaBrHit::TLaBrHit()
{
// Default Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
}

TLaBrHit::~TLaBrHit() = default;

TLaBrHit::TLaBrHit(const TLaBrHit& rhs) : TDetectorHit()
{
// Copy Constructor
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
   Class()->IgnoreTObjectStreamer(kTRUE);
#endif
   Clear();
   rhs.Copy(*this);
}

void TLaBrHit::Copy(TObject& rhs) const
{
   // Copies a TLaBrHit
   TDetectorHit::Copy(rhs);
   static_cast<TLaBrHit&>(rhs).fFilter = fFilter;
}

TVector3 TLaBrHit::GetPosition(Double_t) const
{
   // Gets the position of the current TLaBrHit
   return TLaBr::GetPosition(GetDetector());
}

TVector3 TLaBrHit::GetPosition() const
{
   // Gets the position of the current TLaBrHit
   return GetPosition(GetDefaultDistance());
}

bool TLaBrHit::InFilter(Int_t)
{
   // check if the desired filter is in wanted filter;
   // return the answer;
   // currently does nothing
   return true;
}

void TLaBrHit::Clear(Option_t*)
{
   // Clears the LaBrHit
   fFilter = 0;
   TDetectorHit::Clear();
}

void TLaBrHit::Print(Option_t*) const
{
   // Prints the LaBrHit. Returns:
   // Detector
   // Energy
   // Time
   printf("LaBr Detector: %i\n", GetDetector());
   printf("LaBr hit energy: %.2f\n", GetEnergy());
   printf("LaBr hit time:   %.lf\n", GetTime());
}
