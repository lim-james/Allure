-- Assets
Fonts = {
    ["Microsoft"] = { "Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga" }
    ["CourierNew"] = { "Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga" }
}

-- Objects
Objects =  {

}
function SetToLuaFile(outputString, overwrite, stringToReplace)
	if (overwrite == 0) {
        file = assert(io.open("LUAScripts//DM2240.lua", "a"))
        file:write(outputString)
	} else if (overwrite == 1) {
        file = assert(io.open("LUAScripts//DM2240.lua", "w"))
        file:write(outputString)
	} else if (overwrite == 2) {
        -- Read data
        file = assert(io.open("LUAScripts//DM2240.lua", "r"))
        local fileContent = f:read("*all")
        fil:close()
        
        fileContent = string.gsub(fileContent, stringToReplace, outputString)
       
        fil = assert(io.open("LUAScripts//DM2240.lua", "w"))
        f:write(fileContent)
	}
    
    file:close()
end
