local vector3 = {}

-- Adds two tables containing three indexed elements at pos 1, 2 and 3.
-- @lhs
-- @rhs
function vector3.add(lhs, rhs)
	return { lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] }
end

function vector3.sub(lhs, rhs)
	return { lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] }
end

function vector3.length(vec)
	return math.sqrt(vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3])
end

function vector3.squaredLength(vec)
	return vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]
end

return vector3