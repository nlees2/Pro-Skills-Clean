#pragma once
#include "CWeapon.h"


CWeapon::CWeapon()
{

}


CWeapon::~CWeapon()
{

}
void CWeapon::createWeapon(I3DEngine* myEngine, int maxAmmoValue, bool autofireValue, float fireRateValue, weaponState currentWeaponStateValue, string meshName, CPlayer myPlayer, float weaponStartingY)
{
	maxAmmo = maxAmmoValue;				// sie of an M4Colt ammo clip
	autofireEnabled = autofireValue;		// enables the auto fire option on the M4Colt
	fireRate = fireRateValue;			// sets the fire rate of the M4Colt (interval between shots in seconds)
	ammo = maxAmmo;		// sets the current ammo to the max clip size
	currentWeaponState = currentWeaponStateValue; // sets the M4Colt as the default starting weapon

	weaponMesh = myEngine->LoadMesh(meshName);					// M4 weapon
	weaponModel = weaponMesh->CreateModel(0.0f, 0.0f, 0.0f);			// creates the M4Colt	

	weaponModel->SetPosition(2.0f, weaponStartingY, 7.0f); // Positions the M4Colt in the correct position next to the player
	weaponModel->AttachToParent(myPlayer.myCamera->cameraDummy);	  // attatches the players weapon to the camera dummy
	weaponModel->RotateLocalY(-5.0f);			  // Rotates the M4Colt left slightly for asthetic reasons
	weaponModel->Scale(10.0f);
	reloadTimer = 0.0f;
}
void CWeapon::fireWeapon()
{

}
void CWeapon::reloadWeapon(CWeapon &currentWeapon, float frameTime, CAmmoClip &mapAmmoClip, int& currentAmmoClip)
{
	currentWeapon.reloadTimer += frameTime;  // runs the reload timer

	if (currentWeapon.reloadTimer >= 1.0f)   // if the reload timer has ended
	{
		currentWeapon.ammo = currentWeapon.maxAmmo;  // sets the players current ammo to the max size of the ammo clip
		currentWeapon.currentWeaponState = active;		   // sets the players current weapon to active
		mapAmmoClip.XPos = currentWeapon.weaponModel->GetX(); // sets the X Position of the current ammo clip
		mapAmmoClip.YPos = currentWeapon.weaponModel->GetY(); // sets the Y Position of the current ammo clip
		mapAmmoClip.ZPos = currentWeapon.weaponModel->GetZ(); // sets the Z Position of the current ammo clip
		mapAmmoClip.rotation = 0.0f;							// sets the rotation variable of the ammo clip to upright
		mapAmmoClip.worldModel->ResetOrientation();			// resets teh ammo clips orientation
		currentAmmoClip = nextInArray(currentAmmoClip, kNumAmmoClips);			// sets the current ammo clip to the next in the array, ready for the next reload
	}
}
void CWeapon::selectWeapon()
{

}