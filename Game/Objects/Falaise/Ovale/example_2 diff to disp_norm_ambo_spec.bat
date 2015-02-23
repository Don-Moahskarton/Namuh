REM Convert diffuse map (cdiff) to displacement, normal, and specular 
REM Texture is seamless tileable so change the wrap parameters to xy
REM * parameters are default -v is verbose

REM START /WAIT shadermap.exe cdiff "%cd%\RockLayered0034_1_L.jpg" -disp (0,*,8,xy) -ambo (50,10,0,50,0,0,0) -norm (-100,400,xy,*) -spec (50,-10,60,0) -v
START /WAIT shadermap.exe cdiff "%cd%\RockLayered0034_1_L.jpg" -norm (-20,500,xy,*) -v












REM 
REM ShaderMap CL Arguments
REM 
REM Types of Arguments
REM There are two types of arguments in ShaderMap CL commands, those with parameters and those without.
REM -arg1 (param,param,param) // With parameters
REM -arg2 // Without parameters
REM It should be remembered that any argument parameter can be replaced with a * character. This will tell ShaderMap CL to use a default value for the parameter value.
REM 
REM 
REM Universal Arguments
REM A universal argument can be used with every ShaderMap CL command. There is only one such command -v. -v (verbose) instructs ShaderMap CL to output progress to the console window. It has no parameters. An example usage:
REM cdiff "source.jpg" -disp (*,*,*,*) -v
REM 
REM CDIFF, CDISP, and CNORM Shared Arguments
REM There are a number of arguments that the conversion commands share. Each has to do with the type of map to be created from the source image. They are listed with their parameter names below.
REM -disp ([smooth],[level],[postblur],[wrap]) // Create displacement
REM 
    REM [smooth] is a value between 0 - 100.
    REM [level] is a value between -100 - 100.
    REM [postblur] is a value between 0-100.
    REM [wrap] is one of the following values: 0 = no wrap, x = wrap horizontally, y = wrap vertically, xy = wrap both directions.
REM 
REM 
REM -norm ([level],[intensity],[wrap],[invert]) // Create normal
REM 
    REM [level] is a value between -100 - 100.
    REM [intensity] is a value between 0 - 500.
    REM [wrap] is one of the following values: 0 = no wrap, x = wrap horizontally, y = wrap vertically, xy = wrap both directions.
REM     [invert] is one of the following values: 0 = no inversion, x = invert x component, y = invert y component, xy = invert both x and y.
REM 
REM 
REM -ambo ([pixelbase],[samples],[heightbias],[shadowintensity],[postblur],[normalize],[wrap]) // Create ambient occlusion
REM 
    REM [pixelbase] is a number from 0 - 100 and is the starting brightness of each pixel.
    REM [samples] is a number greater than 0 and is the number of surrounding pixels sampled divided by 8.
REM     [heightbias] is a number between 0 - 100 where 0 is all height displacements are used and 100 is no height displacements are used.
    REM [shadowintensity] is a value between 0 - 100.
    REM [postblur] is a value between 0 - 100.
    REM [normalize] is either 0 or 1.
    REM [wrap] is one of the following values: 0 = no wrap, x = wrap horizontally, y = wrap vertically, xy = wrap both directions.
REM 
REM 
REM -spec ([level],[brightness],[contrast],[wrap]) // Create specular
REM 
    REM [level] is a value between -100 - 100.
    REM [brightness] is a value between -100 - 100.
    REM [contrast] is a value between 0 - 100.
    REM [wrap] is one of the following values: 0 = no wrap, x = wrap horizontally, y = wrap vertically, xy = wrap both directions.
REM 
REM 
REM -dudv ([level],[wrap]) // Create dudv
REM 
    REM [level] is a value between -100 - 100.
    REM [wrap] is one of the following values: 0 = no wrap, x = wrap horizontally, y = wrap vertically, xy = wrap both directions.
REM 
REM 
REM -pv ([bounding_box_size]) // Create fast preview maps
REM 
REM     [bounding_box_size] is a positive number which represents a bounding box (n x n) in pixels. This bounding box defines the maximum preview image size.
REM 
REM 
REM -od ([output_directory]) // Set output directory for maps.
REM 
    REM [output_directory] is a string with the path to the output directory.
REM 
REM 
REM CNORM Specific Arguments
REM There are two arguments that are specific to the cnorm command. They are arguments without parameters and used to control the conversion of the normal map to a displacement map.
REM -i[x][y][xy] // -ix, -iy, or -ixy
REM -m[1][2] // -m1 or -m2
REM The -i [] argument tells ShaderMap CL to pre invert the normals before conversion to displacement. It is sometimes necessary to use this argument if the normal map was created with either the x or y values inverted.
REM 
REM The -m [] argument controlls the conversion method used to convert the normals to displacement.
REM 
    REM -m1: Method 1 of converting norm to disp. This is the preferred method and is the default when no method argument is set.
REM     -m2: An experimental method for converting "hard-to-convert" normal maps. The displacement map will not be exact but can still be useful.
REM 
REM 
REM Here's an example conversion from normal to displacement using method 1 and a preflip on the x axis:
REM cnorm "c:/norm.tga" -disp (*,*,*,*) -m1 -ix
REM 
REM FPROP Specific Arguments
REM The fprop command can have the following arguments. Each argument sets the file type and file suffix for a map that ShaderMap CL generates.
REM -disp ([filetype],[suffix]) // Displacement output
REM -norm ([filetype],[suffix]) // Normal output
REM -ambo ([filetype],[suffix]) // Ambient Occlusion output
REM -spec ([filetype],[suffix]) // Specular output
REM -dudv ([filetype],[suffix]) // Dudv output
REM For more on the fprop command, and to see a list of filetypes, read Output Settings. Remember that any filetype or suffix can be reset to default by placing the * character for its corresponding property value.
REM 
REM 
REM PCHAN Specific Arguments
REM pchan can have up to four -src arguments. The -src argument controls what image the color channels are taken from, which channels are taken, and where channels are placed in the destination image. The -src argument is shown below:
REM -src ([source filepath],[source channels],[destination channels],[is_delete])
REM The source filepath is a string (ex. "C:\source.bmp"). The source channels are any combination of the letters r g b or a (ex. abg). The destination channels are again any combination of the letters r g b or a (ex. arb). Take a look at the following command:
REM pchan "output.bmp" -src ("input.bmp",bga,rgb,*)
REM Here the blue channel is being copied to the red channel of the output.bmp. The green is copied to the green channel and the alpha channel to the blue channel. The number of channels must be the same in source and destination properties.
REM 
REM The [is_delete] parameter can be set to 1 to delete the source image after packing the channels. Set [is_delete] to 0 or * to leave source image on the disk after packing.
