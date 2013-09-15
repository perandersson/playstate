--[[
	Function for generating a class and class metatable based on the supplied parameters. 
	The reason for supplying the class name as a strings as in-parameter is because it's not
	allowed for the game engine to create a new class definition everytime this function is called.
	Features such as script hotswapping is dependant on this.
	
	Usage:
	
	-- Does not create a new class-table definition if one already exists
	MyClass = class("MyClass")
	
	-- Constructor
	function MyClass:__init(x)
	
	end
	
	-- You can also use the Class container for information about the class itself.
	-- This is needed if you want the item to be usable by the in-game editor:
	MyClass.Class
	MyClass.Class.Description
	MyClass.Class.Args[n].Name
	MyClass.Class.Args[n].Type
]]--
function class(className, inheritsClass, classDef)
	-- If the class definition already exists.
	local c = _G[className]
	if c then
		return c
	end
	
	-- Create the classDef table if no value was supplied to the function
	if not classDef then
		classDef = {}
	end
	
	classDef.ClassName = className
	c = { Class = classDef }
	
	-- Inherit class if needed
	if inheritsClass then
		if inheritsClass then
			for i, v in pairs(inheritsClass) do
				c[i] = v
			end
			c.__base = inheritsClass
		end
	end
	
	-- the class will be the metatable for all its objects,
	-- and they will look up their methods in it.
	c.__index = c
	
	-- Expose a constructor which can be called by <classname>(<args>) 
	local mt = {}
	mt.__call = function(class_tbl, ...)
		local new_instance = {}
		setmetatable(new_instance, c)
		if c.__init then
			c.__init(new_instance, ...)
		elseif c.__base and c.__base.__init then
			c.__base.__init(new_instance, ...)
		end	
		new_instance.Class = c.Class
		return new_instance
	end
	setmetatable(c, mt)
	_G[className] = c
	return c
end

return class