local vector3 = {}

-- Adds two tables containing three indexed elements at pos 1, 2 and 3.
-- @lhs
-- @rhs
function vector3.add(lhs, rhs)
	return { lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] }
end

return vector3