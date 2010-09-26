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
	local cmdsTbl = AICallOutsTbl.CommandsTbl
	local commandTbl = {type = cmdTypesTbl.CMD_MOVE, opts = cmdOptionsTbl.SHIFT_KEY, args = {[0] = 123, [1] = 456, [2] = 789}}

	cmdsTbl.GiveCommand(12345, commandTbl)
	-- ]]

	return AIModulePriorityConstsTbl.LUAMODULE_PRIORITY_PROACTIVE
end



function CanRun()
	-- called every frame; should return true
	-- when this module should become active
	-- (and always false if done)
	return true
end

function Update()
	-- called every frame; should return true
	-- when the group managing this module is
	-- done with its assigned [priority] task
	return false
end



function AddUnit(unitID)
	-- called whenever a unit is added to the
	-- group managing this module
end

function DelUnit(unitID)
	-- called whenever a unit is removed from
	-- the group managing this module
end