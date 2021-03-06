/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2014 Laurent Thiebaut & Olivia Merle
 *
 * Blackvoxel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Blackvoxel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 * ZVoxelType.h
 *
 *  Created on: 16 févr. 2011
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_H
#define Z_ZVOXELTYPE_H

// #ifndef Z_ZVOXELTYPE_H
// #  include "ZVoxelType.h"
// #endif
#include "z/ZType_ZVoxelRef.h"

#ifndef Z_BITMAP_IMAGE_H
#  include "z/ZBitmapImage.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

#ifndef Z_VAR_H
#  include "z/ZVar.h"
#endif


#define ZVOXEL_DRAWINFO_VOID 0
#define ZVOXEL_DRAWINFO_NOTVOID 1
#define ZVOXEL_DRAWINFO_DRAWFULLVOXELOPACITY 2
#define ZVOXEL_DRAWINFO_DRAWTRANSPARENTRENDERING 4
#define ZVOXEL_DRAWINFO_SPECIALRENDERING 8

#define ZVOXEL_DRAWINFO_CULLINGBITS (ZVOXEL_DRAWINFO_NOTVOID | ZVOXEL_DRAWINFO_DRAWFULLVOXELOPACITY | ZVOXEL_DRAWINFO_DRAWTRANSPARENTRENDERING)

class ZGame;
class ZStream_SpecialRamStream;
class ZFabInfos;
class ZVoxelTypeManager;
class ZActiveVoxelInterface;
class ZVoxelSector;
struct VoxelLocation
{
  ZVoxelSector * Sector;
  ULong          Offset;
};


class ZVoxelType
{
  protected:
    ZGame * GameEnv;
  public:
    ZVoxelTypeManager * VoxelTypeManager;
    ZString VoxelTypeName;
    //UByte CanPassThrough;
// Obsoleted
    bool  Draw_TransparentRendering;
    bool  Draw_FullVoxelOpacity;
// New
    UByte DrawInfo;
//
    UShort VoxelType;
    ZBitmapImage * MainTexture;
    unsigned int   OpenGl_TextureRef[6];

    bool Is_NoType; // Defined if this is a default "No type" Entry;
    bool Is_UserTypeTransformable;     // Can be used to make user blocks. Very rare blocs must avoid that.
    bool Is_PlayerCanPassThrough;      // The player can pass through it without colision.
    bool Is_CanBeReplacedBy_Water;     // Does water can flow and replace it.
    bool Is_CanBeReplacedBy_GreenAcid; // Does green acid can flow and replace it.
    bool Is_CanBeReplacedBy_MustardGaz;// The mustard gaz can flow and replace it.
    bool Is_CanTriggerBomb;            // Vincinity of this voxel can trigger nearby bomb
    bool Is_Pumpable_ByPump_T1;        // Can be pumped by pump T1.
    bool Is_Pumpable_ByPump_T2;        // Can be pumped by pump T1.
    bool Is_Loadable_ByLoader_L1;      // Can be loaded by loader L1.
    bool BvProp_MoveableByTreadmill;   // Can be moved by threadmill.
    bool BvProp_CanBePickedUpByRobot;  // User programmable robot can pick it up.
    bool BvProp_AtomicFireResistant;   // Z Fire
    bool BvProp_EgmyT1Resistant;
    UByte BvProp_XrRobotPickMinLevel;       // xr1 extraction robot can pick it up.
    UByte BvProp_PrRobotReplaceMinLevel;       // Pr  programmable robot level to destroy it.
    UByte BvProp_PrRobotPickMinLevel;          // Pr  programmable robot level to pick it up.
    UByte BvProp_PrRobotMoveMinLevel;          // Pr  programmable robot level to pick it up.

    Long BlastResistance;              // Blast resistance. 1 = air or no modification. 2 = Very soft solid block. -1..-X = Blast amplification.

    // Control and

    bool Is_SpaceGripType;
    bool Is_KeepControlOnJumping;

    //

    bool Is_Harming;
    bool Is_Active; // Active voxels trigger special functions and transformations.
    bool Is_CombinableWith_GreenAcid;


// Type d'extension.

    ULong ExtensionType;
    bool Is_VoxelExtension;              // Does this voxeltype has specific extension to init.
    bool Is_HasAllocatedMemoryExtension; // Has allocated memory extension

// Interfaces logicielles

    bool Is_Interface_StoreBlock;
    bool Is_Interface_PushBlock;
    bool Is_Interface_PullBlock;
    bool Is_Interface_GetInfo;
    bool Is_Interface_SetInfo;

// Material Caracteristics

    double MiningHardness;
    ULong  MiningType;
    double FrictionCoef;
    double HarmingLifePointsPerSecond;
    double LiquidDensity;

    double Grip_Horizontal;
    double Grip_Vertical;
    bool   Is_Liquid;
    bool   Is_Gaz;

// Autres flags

    bool   Is_Rideable; // This voxel is a vehicle where player can board in.
    bool   Is_HasHelpingMessage;
    ZString HelpingMessage;
    bool   BvProp_FastMoving; // Fast moving voxels can override the modification tracker system in some cases.
    ZFabInfos * FabInfos;
    ULong  Documentation_PageNum;

    ZVoxelType();
    ZVoxelType(UShort VoxelType);
    void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}
    void SetManager(ZVoxelTypeManager * VoxelTypeManager) { this->VoxelTypeManager = VoxelTypeManager; }
    virtual ~ZVoxelType();
    virtual void SpecialRender(float x, float y, float z) {};

    virtual Bool LoadTexture();
    virtual Bool LoadVoxelInformations();

    // Voxel Extensions;
    virtual ZVoxelExtension * CreateVoxelExtension(bool IsLoadingPhase = false) { return(0); }
    virtual void  DeleteVoxelExtension(ZMemSize VoxelExtension, bool IsUnloadingPhase = false);

    //

    virtual void  UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z) {}

    virtual bool  Interface_StoreBlock_Store( UShort VoxelType, ULong Count ) { return(false); }
    virtual ULong Interface_PushBlock_Push( VoxelLocation * DestLocation,  UShort  VoxelType, ULong Count ) { return(0); }
    virtual ULong Interface_PushBlock_PushTest( VoxelLocation * DestLocation,  UShort  VoxelType, ULong Count ) {return(Count);}
    virtual ULong Interface_PushBlock_Pull( VoxelLocation * DestLocation,  UShort * VoxelType, ULong Count ) { return(0);}
    virtual ULong Interface_PushBlock_PullTest( VoxelLocation * DestLocation,  UShort * VoxelType, ULong Count ) { return(0);}

    virtual bool  Interface_GetInfo(VoxelLocation * VLoc, ULong InfoNum, ZVar * Out) { return(false); }
    virtual bool  Interface_GetInfoDoc(ULong InfoNum, ULong DocType, ZVar * Out) { return(false); }
    virtual bool  Interface_SetInfo(VoxelLocation * VLoc, ULong InfoNum, ZVar * In)  { return(false); }
    virtual void  GetBlockInformations(VoxelLocation * DestLocation, ZString & Infos) { return; }

    // When an active voxel should be processed. Note some voxels use "direct" faster way.

    virtual void  ActiveProcess( ZActiveVoxelInterface * AvData) {};

	virtual bool  React( const ZVoxelRef &self, double tick ) { return false; }
};


#endif
