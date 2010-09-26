#include "./AIUnitDef.hpp"
#include "../utils/Util.hpp"

#include <sstream>

float AIUnitDef::FrameCost(char resCode, float buildSpeed) const {
	switch (resCode) {
		case 'M': {
			cFloat bt = GetBuildTimeFrames(buildSpeed);
			cFloat mc = GetDef()->metalCost / bt;
			return mc;
		} break;
		case 'E': {
			cFloat bt = GetBuildTimeFrames(buildSpeed);
			cFloat ec = GetDef()->energyCost / bt;
			return ec;
		} break;
		default: {
			return 1.0f;
		} break;
	}
}

cString AIUnitDef::GetTypeMaskName(cUint32 mask) {
	switch (mask) {
		case MASK_BUILDER_MOBILE: return "BUILDER_MOBILE";
		case MASK_BUILDER_STATIC: return "BUILDER_STATIC";
		case MASK_ASSISTER_MOBILE: return "ASSISTER_MOBILE";
		case MASK_ASSISTER_STATIC: return "ASSISTER_STATIC";
		case MASK_E_PRODUCER_MOBILE: return "E_PRODUCER_MOBILE";
		case MASK_E_PRODUCER_STATIC: return "E_PRODUCER_STATIC";
		case MASK_M_PRODUCER_MOBILE: return "M_PRODUCER_MOBILE";
		case MASK_M_PRODUCER_STATIC: return "M_PRODUCER_STATIC";
		case MASK_E_STORAGE_MOBILE: return "E_STORAGE_MOBILE";
		case MASK_E_STORAGE_STATIC: return "E_STORAGE_STATIC";
		case MASK_M_STORAGE_MOBILE: return "M_STORAGE_MOBILE";
		case MASK_M_STORAGE_STATIC: return "M_STORAGE_STATIC";
		case MASK_DEFENSE_STATIC: return "DEFENSE_STATIC";
		case MASK_DEFENSE_MOBILE: return "DEFENSE_MOBILE";
		case MASK_OFFENSE_STATIC: return "OFFENSE_STATIC";
		case MASK_OFFENSE_MOBILE: return "OFFENSE_MOBILE";
		case MASK_INTEL_MOBILE: return "INTEL_MOBILE";
		case MASK_INTEL_STATIC: return "INTEL_STATIC";
		case MASK_KAMIKAZE: return "KAMIKAZE";
		default: return "INVALID_TYPE_ENUM";
	};
	return "INVALID_TYPE_ENUM";
}

cString AIUnitDef::GetTerrainMaskName(cUint32 mask) {
	switch (mask) {
		case MASK_LAND: return "LAND";
		case MASK_WATER_SURFACE: return "WATER_SURFACE";
		case MASK_WATER_SUBMERGED: return "WATER_SUBMERGED";
		case MASK_AIR: return "AIR";
		default: return "INVALID_TERRAIN_ENUM";
	}
	return "INVALID_TERRAIN_ENUM";
}

cString AIUnitDef::GetWeaponMaskName(cUint32 mask) {
	switch(mask) {
		case MASK_ARMED: return "ARMED";
		case MASK_NUKE: return "NUKE";
		case MASK_ANTINUKE: return "ANTINUKE";
		case MASK_SHIELD: return "SHIELD";
		case MASK_STOCKPILE: return "STOCKPILE";
		case MASK_MANUALFIRE: return "MANUALFIRE";
		default: return "INVALID_WEAPON_ENUM";
	}
	return "INVALID_WEAPON_ENUM";
}

cString AIUnitDef::GetRoleMaskName(cUint32 mask) {
	switch(mask) {
		case MASK_SCOUT: return "SCOUT";
		case MASK_RAIDER: return "RAIDER";
		case MASK_ASSAULT: return "ASSAULT";
		case MASK_ARTILLERY: return "ARTILLERY";
		case MASK_ANTIAIR: return "ANTIAIR";
		case MASK_STRIKER: return "STRIKER";
		default: return "INVALID_ROLE_ENUM";
	}
	return "INVALID_ROLE_ENUM";
}

bool AIUnitDef::AIUnitDefClass::operator < (rcAIUnitDefClass udc) const {
	// sort by summation of all the 1-bit masks, note it doesn't matter which
	// bits are set...
	cUint32 lhs = 
		util::CountOneBits(typeMask) +
		util::CountOneBits(terrMask) +
		util::CountOneBits(weapMask) +
		util::CountOneBits(roleMask);

	cUint32 rhs = 
		util::CountOneBits(udc.typeMask) +
		util::CountOneBits(udc.terrMask) +
		util::CountOneBits(udc.weapMask) +
		util::CountOneBits(udc.roleMask);

	return (lhs < rhs);
}

std::ostream& operator << (std::ostream& out, const AIUnitDef::AIUnitDefClass& unitDefClass) {
	std::stringstream type_mask_ss; type_mask_ss << "typeMasks[";
	std::stringstream terr_mask_ss; terr_mask_ss << "terrMasks[";
	std::stringstream weap_mask_ss; weap_mask_ss << "weapMasks[";
	std::stringstream role_mask_ss; role_mask_ss << "roleMasks[";

	static cUint32 max_bits = 
		std::max<Uint32>(AIUnitDef::NUM_TYPE_MASKS,
		std::max<Uint32>(AIUnitDef::NUM_TERRAIN_MASKS,
		std::max<Uint32>(AIUnitDef::NUM_WEAPON_MASKS, 
		AIUnitDef::NUM_ROLE_MASKS))
	);

	vUint32 indent(4, 0);

	for (Uint32 i = 0; i < max_bits; i++)
	{
		cUint32 mask = (1 << i);

		if (mask & unitDefClass.typeMask)
		{
			type_mask_ss << AIUnitDef::GetTypeMaskName(mask) << "|";
			indent[0] = 1;
		}

		if (mask & unitDefClass.terrMask)
		{
			terr_mask_ss << AIUnitDef::GetTerrainMaskName(mask) << "|";
			indent[1] = 1;
		}

		if (mask & unitDefClass.weapMask)
		{
			weap_mask_ss << AIUnitDef::GetWeaponMaskName(mask) << "|";
			indent[2] = 1;
		}

		if (mask & unitDefClass.roleMask)
		{
			role_mask_ss << AIUnitDef::GetRoleMaskName(mask) << "|";
			indent[3] = 1;
		}
	}

	out << (type_mask_ss.str()).substr(0, type_mask_ss.str().length() - indent[0]) << "], ";
	out << (terr_mask_ss.str()).substr(0, terr_mask_ss.str().length() - indent[1]) << "], ";
	out << (weap_mask_ss.str()).substr(0, weap_mask_ss.str().length() - indent[2]) << "], ";
	out << (role_mask_ss.str()).substr(0, role_mask_ss.str().length() - indent[3]) << "]";

	return out;
}
