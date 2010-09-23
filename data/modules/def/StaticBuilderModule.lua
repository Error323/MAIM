function GetName()
	return "StaticBuilderModule"
end

function CanRun()
	print("[DefStaticBuilderModule.lua::CanRun]")
	return true
end

function Update()
	print("[DefStaticBuilderModule.lua::Update]")
	print(AI)
	print(AI["Avg1"])
	print(AI["Avg2"])
	print(AI.Avg1)
	print(AI.Avg2)
	print(AI.Avg1(1, 2, 3))
	print(AI.Avg2(4, 5, 6))
	return false
end
