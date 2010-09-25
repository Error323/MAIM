function GetName()
	return "AttackerModule"
end

function GetClassMask()
	local typeMasksTbl = AIModuleClassMasks.TypeMasks
	local terrMasksTbl = AIModuleClassMasks.TerrainMasks
	local weapMasksTbl = AIModuleClassMasks.WeaponMasks
	local roleMasksTbl = AIModuleClassMasks.RoleMasks

	-- NOTE: '+' because we do not have bit-ops
	local terrMask = terrMasksTbl.MASK_LAND + terrMasksTbl.MASK_WATER_SURFACE + terrMasksTbl.MASK_WATER_SUBMERGED
	local typeMask = typeMasksTbl.MASK_OFFENSE_MOBILE
	local weapMask = weapMasksTbl.MASK_ARMED
	local roleMask = roleMasksTbl.MASK_ASSAULT

	return typeMask, terrMask, weapMask, roleMask
end

function GetPriority()
	return AIModulePriorities.LUAMODULE_PRIORITY_PROACTIVE
end

function CanRun()
	return true
end

function Update()
	return false
end
