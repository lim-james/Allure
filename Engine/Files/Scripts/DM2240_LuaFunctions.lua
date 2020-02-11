-- SetToLuaFile
function SetToLuaFile(outputString, overwrite, stringToReplace)
   --File
   local f;
   --Overwrite old data with new data.
   if overwrite == 2 then
      --Open the Lua file in Read mode
      f = assert(io.open("Image//DM2240.lua", "r"))
      --Read in the entire content
      local fileContent = f:read("*all")
      --Close the Lua file which was open in Read mode
      f:close()

      --Replace the content
      fileContent = string.gsub(fileContent, stringToReplace, outputString)

      --Open the Lua file in Write mode
      f = assert(io.open("Image//DM2240.lua", "w"))
      --Write the new content into the file
      f:write(fileContent)
   --Wipe the contents with new data.
   elseif overwrite == 1 then
      f = assert(io.open("Image//DM2240.lua", "w"))
      --Write to the file.
      f:write(outputString)
   --Append with new data.
   elseif overwrite == 0 then
      f = assert(io.open("Image//DM2240.lua", "a"))
      --Write to the file.
      f:write(outputString)
   end
   --Close the file.
   f:close()
end

-- Calculate Distance Square
function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
  --Calculate distance squared
  local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
  --Print the result
  print(distanceSquare)
  -- Return the result
  return distanceSquare
end

-- Get the Min, Max, Average and number of values for a set of arguments
function GetMinMax(...)
  local minValue = -1
  local maxValue = -1
  local avgValue = -1
  local numValues = 0
  for i,v in ipairs(arg) do
    if minValue == -1 then
      minValue = v
    elseif v < minValue then
      minValue = v
    end

    if maxValue == -1 then
      maxValue = v
    elseif v > maxValue then
      maxValue = v
    end
    
    avgValue = avgValue + v
    numValues = numValues + 1
    end
  avgValue = avgValue / numValues
  return minValue, maxValue, avgValue, numValues
end
