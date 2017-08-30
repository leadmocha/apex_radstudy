#ifndef B1TrackInformation_h
#define B1TrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class B1TrackInformation : public G4VUserTrackInformation 
{
  public:
    B1TrackInformation();
    B1TrackInformation(const G4Track* aTrack);
    B1TrackInformation(const B1TrackInformation* aTrackInfo);
    virtual ~B1TrackInformation();
   
    inline void *operator new(size_t);
    inline void operator delete(void *aTrackInfo);
    inline int operator ==(const B1TrackInformation& right) const
    {return (this==&right);}

    void Print() const;

  private:
    G4int                 originalTrackID;
    G4ParticleDefinition* particleDefinition;
    G4ThreeVector         originalPosition;
    G4ThreeVector         originalMomentum;
    G4double              originalEnergy;
    G4double              originalTime;

  public:
    inline G4int GetOriginalTrackID() const {return originalTrackID;}
    inline G4ParticleDefinition* GetOriginalParticle() const {return particleDefinition;}
    inline G4ThreeVector GetOriginalPosition() const {return originalPosition;}
    inline G4ThreeVector GetOriginalMomentum() const {return originalMomentum;}
    inline G4double GetOriginalEnergy() const {return originalEnergy;}
    inline G4double GetOriginalTime() const {return originalTime;}
};

extern G4Allocator<B1TrackInformation> aTrackInformationAllocator;

inline void* B1TrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void B1TrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator.FreeSingle((B1TrackInformation*)aTrackInfo);}

#endif
