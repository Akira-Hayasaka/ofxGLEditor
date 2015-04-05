-- a single line comment
--[[
	a multi line comment
                         ]]

-- syntax highlighting (in this case lua)
function setup
	print("hello world")
	print("123string456")
end

function draw
	of.setColor(200.1)
	of.line(10, 10, 100, 100)
end

-- mix of numbers and text
1ab23cd45

-- a hex literal number
0x0123456789abcdefABCDEF

-- a very long line that should scroll or wrap
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
