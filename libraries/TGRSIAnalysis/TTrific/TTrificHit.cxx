#include "TTrific.h"
#include "TTrificHit.h"
#include "Globals.h"

/// \cond CLASSIMP
ClassImp(TTrificHit)
/// \endcond

TTrificHit::TTrificHit() : TDetectorHit()
{
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
}

TTrificHit::TTrificHit(const TTrificHit& rhs) : TDetectorHit()
{
#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
	Class()->IgnoreTObjectStreamer(kTRUE);
#endif
	Clear();
	rhs.Copy(*this);
}

TTrificHit::~TTrificHit() = default;

void TTrificHit::Copy(TObject& rhs) const
{
	TDetectorHit::Copy(rhs);
	static_cast<TTrificHit&>(rhs).fFilter = fFilter;
}

void TTrificHit::Clear(Option_t* opt)
{
	TDetectorHit::Clear(opt); // clears the base (address, position and waveform)
}

void TTrificHit::Print(Option_t*) const
{
	printf("TRIFIC Detector: %i\n", GetDetector());
	printf("TRIFIC Segment:    %i\n", GetSegment());
	printf("TRIFIC Energy:   %lf\n", GetEnergy());
	printf("TRIFIC Hit Time:   %f\n", GetTime());
}

TVector3 TTrificHit::GetPosition() const
//TVector3 TTrificHit::GetPosition()
{
	//calling GetPosition() on a TRIFIC hit will return the position vector to the centre of the grid
	//calling TTrific::GetPosition(det) will give the vector to the position itself.

	return TVector3(0,0,TTrific::fTargetToWindowCart+TTrific::fInitialSpacingCart+TTrific::fSpacingCart*GetDetector());
}

