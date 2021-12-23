Memory Corruption

#include "object.h"
#include "tacticalsituation.h"

using namespace std;

int main()
{
  auto enemyBoat1  = new LocationObject ("Barbuda", 1000, 1000,
                                                  20.5, 270);
  enemyBoat1->setIdentity(Identifcat::FOE);
  enemyBoat1->setClassif(Classificat::SURFACE);
  enemyBoat1->setCommander("Cpt.Big");
  enemyBoat1->setWeaponQuantity(0, 100);
  enemyBoat1->setWeaponQuantity(1, 2);
  enemyBoat1->setWeaponQuantity(3, 1500);
  enemyBoat1->setWeaponQuantity(4, 300000);
  auto enemyJam1  = make_unique<LocationObject> ("Jam1", -1000, -1000,
                                                 0.0, 0);
  enemyJam1->setIdentity(Identifcat::FOE);
  enemyJam1->setClassif(Classificat::UNKNOWN);
  auto ownAircraft  = make_unique<LocationObject> ("Eagle", 0, 10,
                                                   400.0, 325, 3000);
  ownAircraft->setIdentity(Identifcat::FRIEND);
  ownAircraft->setClassif(Classificat::AIR);
  ownAircraft->setCommander("Cpt.Fast");
  ownAircraft->setWeaponQuantity(0, 4);
  ownAircraft->setWeaponQuantity(1, 4);
  ownAircraft->setWeaponQuantity(3, 12);
  TacticalSituation situation1;
  situation1.addTrack(*enemyBoat1);
  delete enemyBoat1;
  situation1.addTrack(*enemyJam1.get());
  situation1.addTrack(*ownAircraft.get());
  situation1.saveToFile("todaySituation");

  auto enemyBoat2  = make_unique<LocationObject> ("Nightmare", -120, -600,
                                                  12.0, 125);
  enemyBoat2->setIdentity(Identifcat::FOE);
  enemyBoat2->setClassif(Classificat::SURFACE);
  enemyBoat2->setCommander("Cpt.Big");
  enemyBoat2->setWeaponQuantity(0, 100);
  enemyBoat2->setWeaponQuantity(1, 2);
  enemyBoat2->setWeaponQuantity(3, 1500);
  enemyBoat2->setWeaponQuantity(4, 300000);
  auto ownSubmarine  = make_unique<LocationObject> ("Shark", -100, -10,
                                                    10.0, 325, -50);
  ownSubmarine->setIdentity(Identifcat::FRIEND);
  ownSubmarine->setCommander("Cmndr.Concretcior");
  ownSubmarine->setWeaponQuantity(0,16);
  ownSubmarine->setWeaponQuantity(1,2);
  auto params = make_unique<ObjectParameters> (850,-250,325.0,45,1000);
  LocationObject enemyAircraft1("Brolemas125", *params.get());
  LocationObject enemyAircraft2("Brolemas315", *params.get());
  TacticalSituation situation2;
  situation2.addTrack(*enemyBoat1);
  situation2.addTrack(*enemyBoat2.get());
  situation2.addTrack(*ownSubmarine.get());
  situation2.addTrack(enemyAircraft1);
  situation2.addTrack(enemyAircraft2);
  situation2.addFromFile("todaySituation");

  LocationObject trackNM;
  list<ObjectParameters> trackTrajectory;
  if (situation2.getTrack("Nightmare",trackNM))
  {
    trackNM.createTrajectory(trackTrajectory, 600);
    for (const auto & el: trackTrajectory)
    {
      cout << "x = " << el.xMeters << "m";
      cout << " y = " << el.yMeters << "m";
      cout << " Co = " << el.courseDegrees << "deg";
      cout << " Sp = " << el.speedMetersSec << "m/s" << endl;
    }
  }
  std::cout << "==========================" << endl;
  situation1.addTrack(*ownSubmarine.get());
  auto tracksInRegion = situation1.getTracksInRegion(500);
  for (const auto & el: tracksInRegion)
  {
    cout << el << " in area." <<endl;
  }
  LocationObject trackSK;
  situation1.getTrack("Shark",trackSK);
  cout << "Shark range = " << trackSK.getRange() << "m"<< endl;
  situation1.advanceSituation(600);
  situation1.getTrack("Shark",trackSK);
  cout << "Shark range = " << trackSK.getRange() << "m, weapon = " <<
          trackSK.getWeaponQuantity() << endl;
  std::cout << "==========================" << endl;
  std::pair<int,int> coor1{-1000,-1000};
  std::pair<int,int> coor2{1000,1000};
  tracksInRegion = situation2.getTracksInRegion(coor1, coor2);
      cout << "Situation2: tracks in the area: " << endl;
  for (const auto & el: tracksInRegion)
  {
    cout << el << endl;
  }

  clearAllSituationFiles("my1PAssWD_"); //example.
                                      //The password must be entered via the GUI
  return 0;
}


Solution

#include "object.h"
#include "tacticalsituation.h"

using namespace std;

int main()
{
  auto enemyBoat1  = make_unique<LocationObject> ("Barbuda", 1000, 1000,
                                                  20.5, 270);
  enemyBoat1->setIdentity(Identifcat::FOE);
  enemyBoat1->setClassif(Classificat::SURFACE);
  enemyBoat1->setCommander("Cpt.Big");
  enemyBoat1->setWeaponQuantity(0, 100);
  enemyBoat1->setWeaponQuantity(1, 2);
  enemyBoat1->setWeaponQuantity(3, 1500);
  enemyBoat1->setWeaponQuantity(4, 300000);
  auto enemyJam1  = make_unique<LocationObject> ("Jam1", -1000, -1000,
                                                 0.0, 0);
  enemyJam1->setIdentity(Identifcat::FOE);
  enemyJam1->setClassif(Classificat::UNKNOWN);
  auto ownAircraft  = make_unique<LocationObject> ("Eagle", 0, 10,
                                                   400.0, 325, 3000);
  ownAircraft->setIdentity(Identifcat::FRIEND);
  ownAircraft->setClassif(Classificat::AIR);
  ownAircraft->setCommander("Cpt.Fast");
  ownAircraft->setWeaponQuantity(0, 4);
  ownAircraft->setWeaponQuantity(1, 4);
  ownAircraft->setWeaponQuantity(3, 12);
  TacticalSituation situation1;
  situation1.addTrack(*enemyBoat1.get());
  situation1.addTrack(*enemyJam1.get());
  situation1.addTrack(*ownAircraft.get());
  situation1.saveToFile("todaySituation");

  auto enemyBoat2  = make_unique<LocationObject> ("Nightmare", -120, -600,
                                                  12.0, 125);
  enemyBoat2->setIdentity(Identifcat::FOE);
  enemyBoat2->setClassif(Classificat::SURFACE);
  enemyBoat2->setCommander("Cpt.Big");
  enemyBoat2->setWeaponQuantity(0, 100);
  enemyBoat2->setWeaponQuantity(1, 2);
  enemyBoat2->setWeaponQuantity(3, 1500);
  enemyBoat2->setWeaponQuantity(4, 300000);
  auto ownSubmarine  = make_unique<LocationObject> ("Shark", -100, -10,
                                                    10.0, 325, -50);
  ownSubmarine->setIdentity(Identifcat::FRIEND);
  ownSubmarine->setCommander("Cmndr.Concretcior");
  ownSubmarine->setWeaponQuantity(0,16);
  ownSubmarine->setWeaponQuantity(1,2);
  auto params = make_unique<ObjectParameters> (850,-250,325.0,45,1000);
  LocationObject enemyAircraft1("Brolemas125", *params.get());
  LocationObject enemyAircraft2("Brolemas315", *params.get());
  TacticalSituation situation2;
  situation2.addTrack(*enemyBoat1.get());
  situation2.addTrack(*enemyBoat2.get());
  situation2.addTrack(*ownSubmarine.get());
  situation2.addTrack(enemyAircraft1);
  situation2.addTrack(enemyAircraft2);
  situation2.addFromFile("todaySituation");

  LocationObject trackNM;
  list<ObjectParameters> trackTrajectory;
  if (situation2.getTrack("Nightmare",trackNM))
  {
    trackNM.createTrajectory(trackTrajectory, 600);
    for (const auto & el: trackTrajectory)
    {
      cout << "x = " << el.xMeters << "m";
      cout << " y = " << el.yMeters << "m";
      cout << " Co = " << el.courseDegrees << "deg";
      cout << " Sp = " << el.speedMetersSec << "m/s" << endl;
    }
  }
  std::cout << "==========================" << endl;
  situation1.addTrack(*ownSubmarine.get());
  auto tracksInRegion = situation1.getTracksInRegion(500);
  for (const auto & el: tracksInRegion)
  {
    cout << el << " in area." <<endl;
  }
