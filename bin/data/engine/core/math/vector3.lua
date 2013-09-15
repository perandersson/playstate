local Vector3 = {}
function Vector3.__call(x, y, z)
	return {x, y, z}
end

-- Adds two tables containing three indexed elements at pos 1, 2 and 3.
-- @lhs
-- @rhs
function Vector3.Add(lhs, rhs)
	return { lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] }
end

function Vector3.Sub(lhs, rhs)
	return { lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] }
end

function Vector3.Length(vec)
	return math.sqrt(vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3])
end

function Vector3.SquaredLength(vec)
	return vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]
end

return Vector3