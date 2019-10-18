#ifndef TGRIFFIN_H
#define TGRIFFIN_H

/** \addtogroup Detectors
 *  @{
 */

#include <utility>
#include <vector>
#include <cstdio>
#include <functional>
//#include <tuple>

#include "TBits.h"
#include "TVector3.h"

#include "Globals.h"
#include "TGriffinHit.h"
#include "TSuppressed.h"
#include "TTransientBits.h"
#include "TSpline.h"

class TGriffin : public TSuppressed {
public:
   enum class EGriffinBits {
      kIsLowGainAddbackSet            = 1<<0,
      kIsHighGainAddbackSet           = 1<<1,
      kIsLowGainCrossTalkSet          = 1<<2,
      kIsHighGainCrossTalkSet         = 1<<3,
      kIsLowGainSuppressed            = 1<<4,
      kIsHighGainSuppressed           = 1<<5,
      kIsLowGainSuppressedAddbackSet  = 1<<6,
      kIsHighGainSuppressedAddbackSet = 1<<7
   };
   enum class EGainBits { kLowGain, kHighGain };

   TGriffin();
   TGriffin(const TGriffin&);
   ~TGriffin() override;

public:
   TGriffinHit* GetGriffinLowGainHit(const int& i);                                              //!<!
   TGriffinHit* GetGriffinHighGainHit(const int& i);                                             //!<!
   TGriffinHit* GetGriffinHit(const Int_t& i) { return GetGriffinHit(i, GetDefaultGainType()); } //!<!
   using TDetector::GetHit;
   TDetectorHit* GetHit(const int& idx);
   Short_t GetLowGainMultiplicity() const { return fGriffinLowGainHits.size(); }
   Short_t GetHighGainMultiplicity() const { return fGriffinHighGainHits.size(); }
   Short_t GetMultiplicity() const override { return GetMultiplicity(GetDefaultGainType()); }

   static TVector3 GetPosition(int DetNbr, int CryNbr = 5, double dist = 110.0); //!<!
   static TVector3 GetDetectorPosition(int DetNbr); //!<!
   static const char* GetColorFromNumber(Int_t number);
#ifndef __CINT__
   void AddFragment(const std::shared_ptr<const TFragment>&, TChannel*) override; //!<!
#endif
	void BuildHits() override {} // no need to build any hits, everything already done in AddFragment

   void ClearTransients() override
   {
		TDetector::ClearTransients();
      fGriffinBits = 0;
      for(const auto& hit : fGriffinHighGainHits) {
         hit->ClearTransients();
      }
   }
   void ResetFlags() const;

   TGriffin& operator=(const TGriffin&); //!<!

#if !defined(__CINT__) && !defined(__CLING__)
   void SetAddbackCriterion(std::function<bool(const TDetectorHit*, const TDetectorHit*)> criterion)
   {
      fAddbackCriterion = std::move(criterion);
   }
   std::function<bool(const TDetectorHit*, const TDetectorHit*)> GetAddbackCriterion() const { return fAddbackCriterion; }

   bool AddbackCriterion(const TDetectorHit* hit1, const TDetectorHit* hit2) override { return fAddbackCriterion(hit1, hit2); }
#endif

   Short_t      GetAddbackLowGainMultiplicity();
   Short_t      GetAddbackHighGainMultiplicity();
   Short_t      GetAddbackMultiplicity() { return GetAddbackMultiplicity(GetDefaultGainType()); }
   TGriffinHit* GetAddbackLowGainHit(const int& i);
   TGriffinHit* GetAddbackHighGainHit(const int& i);
   TGriffinHit* GetAddbackHit(const int& i) { return GetAddbackHit(i, GetDefaultGainType()); }
   bool IsAddbackSet(const EGainBits& gain_type) const;
   void     ResetLowGainAddback();                                 //!<!
   void     ResetHighGainAddback();                                //!<!
   void     ResetAddback() { ResetAddback(GetDefaultGainType()); } //!<!
   UShort_t GetNHighGainAddbackFrags(const size_t& idx);
   UShort_t GetNLowGainAddbackFrags(const size_t& idx);
   UShort_t GetNAddbackFrags(const size_t& idx) { return GetNAddbackFrags(idx, GetDefaultGainType()); }

#if !defined(__CINT__) && !defined(__CLING__)
   void SetSuppressionCriterion(std::function<bool(const TDetectorHit*, const TDetectorHit*)> criterion)
   {
      fSuppressionCriterion = std::move(criterion);
   }
   std::function<bool(const TDetectorHit*, const TDetectorHit*)> GetSuppressionCriterion() const { return fSuppressionCriterion; }

   bool SuppressionCriterion(const TDetectorHit* hit, const TDetectorHit* bgoHit) override { return fSuppressionCriterion(hit, bgoHit); }
#endif

   TGriffinHit* GetSuppressedLowGainHit(const int& i);                                              //!<!
   TGriffinHit* GetSuppressedHighGainHit(const int& i);                                             //!<!
   TGriffinHit* GetSuppressedHit(const Int_t& i) { return GetSuppressedHit(i, GetDefaultGainType()); } //!<!
   Short_t GetSuppressedLowGainMultiplicity(const TBgo* bgo);
   Short_t GetSuppressedHighGainMultiplicity(const TBgo* bgo);
   Short_t GetSuppressedMultiplicity(const TBgo* bgo) { return GetSuppressedMultiplicity(bgo, GetDefaultGainType()); }
   bool IsSuppressed(const EGainBits& gain_type) const;
   void     ResetLowGainSuppressed();                                 //!<!
   void     ResetHighGainSuppressed();                                //!<!
   void     ResetSuppressed() { ResetSuppressed(GetDefaultGainType()); } //!<!

   Short_t      GetSuppressedAddbackLowGainMultiplicity(const TBgo* bgo);
   Short_t      GetSuppressedAddbackHighGainMultiplicity(const TBgo* bgo);
   Short_t      GetSuppressedAddbackMultiplicity(const TBgo* bgo) { return GetSuppressedAddbackMultiplicity(bgo, GetDefaultGainType()); }
   TGriffinHit* GetSuppressedAddbackLowGainHit(const int& i);
   TGriffinHit* GetSuppressedAddbackHighGainHit(const int& i);
   TGriffinHit* GetSuppressedAddbackHit(const int& i) { return GetSuppressedAddbackHit(i, GetDefaultGainType()); }
   bool IsSuppressedAddbackSet(const EGainBits& gain_type) const;
   void     ResetLowGainSuppressedAddback();                                 //!<!
   void     ResetHighGainSuppressedAddback();                                //!<!
   void     ResetSuppressedAddback() { ResetSuppressedAddback(GetDefaultGainType()); } //!<!
   UShort_t GetNHighGainSuppressedAddbackFrags(const size_t& idx);
   UShort_t GetNLowGainSuppressedAddbackFrags(const size_t& idx);
   UShort_t GetNSuppressedAddbackFrags(const size_t& idx) { return GetNSuppressedAddbackFrags(idx, GetDefaultGainType()); }

private:
#if !defined(__CINT__) && !defined(__CLING__)
   static std::function<bool(const TDetectorHit*, const TDetectorHit*)> fAddbackCriterion;
   static std::function<bool(const TDetectorHit*, const TDetectorHit*)> fSuppressionCriterion;
#endif

	std::vector<std::shared_ptr<TDetectorHit> >& fGriffinLowGainHits = fHits; //!<! Reference to default hit vector (needs to be transient)
   std::vector<std::shared_ptr<TDetectorHit> >  fGriffinHighGainHits; //  The set of crystal hits

   // static bool fSetBGOHits;                //!<!  Flag that determines if BGOHits are being measured

   static bool fSetCoreWave; //!<!  Flag for Waveforms ON/OFF
   // static bool fSetBGOWave;                //!<!  Flag for BGO Waveforms ON/OFF

   long                            fCycleStart; //!<!  The start of the cycle
   mutable TTransientBits<UChar_t> fGriffinBits;  // Transient member flags

   mutable std::vector<std::shared_ptr<TDetectorHit> > fAddbackLowGainHits;  //!<! Used to create addback hits on the fly
   mutable std::vector<std::shared_ptr<TDetectorHit> > fAddbackHighGainHits; //!<! Used to create addback hits on the fly
   mutable std::vector<UShort_t> fAddbackLowGainFrags;  //!<! Number of crystals involved in creating in the addback hit
   mutable std::vector<UShort_t> fAddbackHighGainFrags; //!<! Number of crystals involved in creating in the addback hit

   std::vector<std::shared_ptr<TDetectorHit> > fSuppressedLowGainHits;  //!<!  The set of suppressed crystal hits
   std::vector<std::shared_ptr<TDetectorHit> > fSuppressedHighGainHits; //!<!  The set of suppressed crystal hits

   mutable std::vector<std::shared_ptr<TDetectorHit> > fSuppressedAddbackLowGainHits;  //!<! Used to create suppressed addback hits on the fly
   mutable std::vector<std::shared_ptr<TDetectorHit> > fSuppressedAddbackHighGainHits; //!<! Used to create suppressed addback hits on the fly
   mutable std::vector<UShort_t> fSuppressedAddbackLowGainFrags;  //!<! Number of crystals involved in creating in the suppressed addback hit
   mutable std::vector<UShort_t> fSuppressedAddbackHighGainFrags; //!<! Number of crystals involved in creating in the suppressed addback hit

   static EGainBits fDefaultGainType;

public:
   static bool SetCoreWave() { return fSetCoreWave; } //!<!
   static void SetDefaultGainType(const EGainBits& gain_type);
   static EGainBits GetDefaultGainType() { return fDefaultGainType; }

private:
   static TVector3 gCloverPosition[17];                      //!<! Position of each HPGe Clover
   void            ClearStatus() const { fGriffinBits = 0; } //!<!
   void SetBitNumber(EGriffinBits bit, Bool_t set) const;
   Bool_t TestBitNumber(EGriffinBits bit) const { return fGriffinBits.TestBit(bit); }

   // Cross-Talk stuff
public:
   static Double_t CTCorrectedEnergy(const TGriffinHit* const hit_to_correct, const TGriffinHit* const other_hit,
                                     Bool_t time_constraint = true);
   Bool_t IsCrossTalkSet(const EGainBits& gain_type) const;
   void FixLowGainCrossTalk();
   void FixHighGainCrossTalk();

private:
   // This is where the general untouchable functions live.
   const std::vector<std::shared_ptr<TDetectorHit> >& GetHitVector() const override { return GetHitVector(fDefaultGainType); }      //!<!
   std::vector<std::shared_ptr<TDetectorHit> >& GetHitVector(const EGainBits& gain_type);      //!<!
   const std::vector<std::shared_ptr<TDetectorHit> >& GetHitVector(const EGainBits& gain_type) const;      //!<!
   std::vector<std::shared_ptr<TDetectorHit> >& GetAddbackVector(const EGainBits& gain_type);  //!<!
   std::vector<UShort_t>& GetAddbackFragVector(const EGainBits& gain_type); //!<!
   TGriffinHit* GetGriffinHit(const Int_t& i, const EGainBits& gain_type);  //!<!
   Short_t GetMultiplicity(const EGainBits& gain_type) const;
   TGriffinHit* GetAddbackHit(const int& i, const EGainBits& gain_type);
   Short_t GetAddbackMultiplicity(const EGainBits& gain_type);
   void SetAddback(const EGainBits& gain_type, bool flag = true) const;
   void ResetAddback(const EGainBits& gain_type); //!<!
   UShort_t GetNAddbackFrags(const size_t& idx, const EGainBits& gain_type);

   std::vector<std::shared_ptr<TDetectorHit> >& GetSuppressedVector(const EGainBits& gain_type);      //!<!
   std::vector<std::shared_ptr<TDetectorHit> >& GetSuppressedAddbackVector(const EGainBits& gain_type);  //!<!
   std::vector<UShort_t>& GetSuppressedAddbackFragVector(const EGainBits& gain_type); //!<!
   TGriffinHit* GetSuppressedHit(const Int_t& i, const EGainBits& gain_type);  //!<!
   Short_t GetSuppressedMultiplicity(const TBgo* bgo, const EGainBits& gain_type);
   void SetSuppressed(const EGainBits& gain_type, bool flag = true) const;
   void ResetSuppressed(const EGainBits& gain_type); //!<!
   TGriffinHit* GetSuppressedAddbackHit(const int& i, const EGainBits& gain_type);
   Short_t GetSuppressedAddbackMultiplicity(const TBgo* bgo, const EGainBits& gain_type);
   void SetSuppressedAddback(const EGainBits& gain_type, bool flag = true) const;
   void ResetSuppressedAddback(const EGainBits& gain_type); //!<!
   UShort_t GetNSuppressedAddbackFrags(const size_t& idx, const EGainBits& gain_type);

   void FixCrossTalk(const EGainBits& gain_type);
   void SetCrossTalk(const EGainBits& gain_type, bool flag = true) const;

public:
   void Copy(TObject&) const override;            //!<!
   void Clear(Option_t* opt = "all") override;    //!<!
   void Print(Option_t* opt = "") const override; //!<!

   /// \cond CLASSIMP
   ClassDefOverride(TGriffin, 6) // Griffin Physics structure
   /// \endcond
};
/*! @} */
#endif
