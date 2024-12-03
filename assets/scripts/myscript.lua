-- This is a lua global variable
some_variable = 3 ^ (5 * 2)

-- This is a lua table that contain values for the initialization of our game engine
config = {
    title = "My Game Engine",
    fullscreen = false,
    resolution = {
        width = 800,
        height = 600
    }
}

-- This is a Lua function that calculates the factorial of a given number
function factorial(n)
    if n == 1 then
        return 1
    end
    return n * factorial(n - 1)
end

print("The cube of the number 3 is " .. cube(3))