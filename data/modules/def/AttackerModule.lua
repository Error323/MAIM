function GetName()
	return "AttackerModule"
end

function GetClassMask()
	local typeMasksTbl = AIModuleClassMaskConstsTbl.TypeMasksTbl
	local terrMasksTbl = AIModuleClassMaskConstsTbl.TerrainMasksTbl
	local weapMasksTbl = AIModuleClassMaskConstsTbl.WeaponMasksTbl
	local roleMasksTbl = AIModuleClassMaskConstsTbl.RoleMasksTbl

	-- NOTE: '+' because we do not have bit-ops
	local terrMask = terrMasksTbl.MASK_LAND + terrMasksTbl.MASK_WATER_SURFACE + terrMasksTbl.MASK_WATER_SUBMERGED
	local typeMask = typeMasksTbl.MASK_OFFENSE_MOBILE
	local weapMask = weapMasksTbl.MASK_ARMED
	local roleMask = roleMasksTbl.MASK_ASSAULT

	return typeMask, terrMask, weapMask, roleMask
end

function GetPriority()
	-- [[
	local cmdTypesTbl = AICommandConstsTbl.TypesTbl
	local cmdOptionsTbl = AICommandConstsTbl.OptionsTbl
	local commandTbl = {type = cmdTypesTbl.CMD_MOVE, opts = cmdOptionsTbl.SHIFT_KEY, args = {[0] = 123, [1] = 456}}
	AICallOutsTbl.CommandsTbl.GiveCommand(12345, commandTbl)
	-- ]]

	return AIModulePriorityConstsTbl.LUAMODULE_PRIORITY_PROACTIVE
end

function CanRun()
	return true
end

function Update()
	return false
end
