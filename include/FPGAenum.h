//FPGAenum.h

enum class HitType { unmapped, mapped, clustered, wildcard, guessed, undefined, extrapolated, spacepoint };
//To label the origin of FPGATrackSimHits
enum class SiliconTech { strip, pixel, nTechs, undefined };

//The detector zone that the FPGATrackSimHit was in
enum class DetectorZone { barrel, posEndcap, negEndcap, nZones, undefined };

