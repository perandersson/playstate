-- Function for generating a class and class metatable based on the supplied parameters

-- Usage: MyClass = class("MyClass", "ParentClass")
-- The reason for using strings as in-parameters is because we MUST NOT create a new table for a class
-- when the file is reloaded. This will enable us to alter existing classes in runtime.
function class(className, inheritsClassName)
	-- If the class definition already exists.
	local c = _G[className]
	if c then
		print("Found metatable for class " .. className)
		return c
	end
	
	c = {}
	
	-- Inherit class if needed
	local inheritsClass = _G[inheritsClassName]
	if inheritsClass then
		for i, v in pairs(inheritsClass) do
			c[i] = v
		end
		c.__base = inheritsClass
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
			print("calling base constructor")
			c.__base.__init(new_instance, ...)
		end		
		return new_instance
	end
	setmetatable(c, mt)
	_G[className] = c
	return c
end

return class