-- "global" units table; _ALL_ LuaModule instances
-- sharing this Lua state also share the same data
local luaStateUnits = {}


function GetName()
	return "AttackerModule"
end


function GetMinGroupSize(groupID) return    1 end
function GetMaxGroupSize(groupID) return 9001 end


function GetClass()
	-- called at AI load-time only (class-mask is fixed)
	local typeMasksTbl = AIModuleClassConstsTbl.TypeMasksTbl
	local terrMasksTbl = AIModuleClassConstsTbl.TerrainMasksTbl
	local weapMasksTbl = AIModuleClassConstsTbl.WeaponMasksTbl
	local roleMasksTbl = AIModuleClassConstsTbl.RoleMasksTbl

	-- NOTE: '+' because we do not have bit-ops
	local terrMask = terrMasksTbl.MASK_LAND + terrMasksTbl.MASK_WATER_SURFACE + terrMasksTbl.MASK_WATER_SUBMERGED
	local typeMask = typeMasksTbl.MASK_OFFENSE_MOBILE
	local weapMask = weapMasksTbl.MASK_ARMED
	local roleMask = roleMasksTbl.MASK_ASSAULT

	return typeMask, terrMask, weapMask, roleMask
end

function GetPriority()
	-- called at AI load-time only (priority is fixed)
	--[[
	local cmdTypesTbl = AICommandConstsTbl.TypesTbl
	local cmdOptionsTbl = AICommandConstsTbl.OptionsTbl
	local cmdsTbl = AICallOutsTbl.CommandCallOutsTbl
	local commandTbl = {type = cmdTypesTbl.CMD_MOVE, opts = cmdOptionsTbl.SHIFT_KEY, args = {[0] = 123, [1] = 456, [2] = 789}}

	cmdsTbl.GiveCommand(12345, commandTbl)
	--]]

	return AIModulePriorityConstsTbl.LUAMODULE_PRIORITY_PROACTIVE
end



function CanRun(groupID)
	-- called every frame; should return true
	-- when this module should become active
	-- (and always false if done)
	return true
end

function Update(groupID)
	-- called every frame; should return true
	-- when the group managing this module is
	-- done with its assigned [priority] task
	local groupUnits = luaStateUnits[groupID]

	if (groupUnits ~= nil) then
		for unitID, _ in pairs(groupUnits) do
			groupUnits[unitID] = groupUnits[unitID] + 1
		end
	end

	return false
end



function CanAddUnit(groupID, unitID)
	-- called whenever a unit is being considered for
	-- addition to the group managing this LuaModule
	-- instance
	-- TODO: incorporate distance checks, etc.
	if (luaStateUnits[groupID] ~= nil) then
		return (#luaStateUnits[groupID] < GetMaxGroupSize(groupID))
	end

	return true
end

function AddUnit(groupID, unitID)
	-- called whenever a unit is added to the
	-- group managing this LuaModule instance
	if (luaStateUnits[groupID] == nil) then
		luaStateUnits[groupID] = {}
	end

	luaStateUnits[groupID][unitID] = AICallOutsTbl.SimStateCallOutsTbl.GetCurrSimFrame()
end

function DelUnit(groupID, unitID)
	-- called whenever a unit is removed from
	-- the group that contains this LuaModule
	-- instance
	luaStateUnits[groupID][unitID] = nil

	if (#luaStateUnits[groupID] == 0) then
		luaStateUnits[groupID] = nil
	end
end
