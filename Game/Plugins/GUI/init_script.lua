-- get CEGUI singletons
local logger = CEGUI.Logger:getSingleton()
logger:logEvent( ">>> Init script says hello" )
logger:setLoggingLevel( CEGUI.Informative )

-- get a local reference to the singletons we use (not required)
 local winMgr = CEGUI.WindowManager:getSingleton()
 local logger = CEGUI.Logger:getSingleton()
 local imageman  = CEGUI.ImagesetManager:getSingleton()
 local schememan = CEGUI.SchemeManager:getSingleton()
 local fontman   = CEGUI.FontManager:getSingleton()
 local system    = CEGUI.System:getSingleton()

-- load a default font
-- fontman:createFont( "fonts/georgia.ttf" )

-- set default mouse cursor
system:setDefaultMouseCursor( "TaharezLook","MouseArrow" )


-- 	local rootWindow = winMgr:getWindow("root")
 
-- 	w = winMgr:loadWindowLayout("layouts/TreeDemoTaharez.layout")
-- 	w:setVisible(true)
-- 	rootWindow:addChildWindow(w)
 
-- 	w = winMgr:loadWindowLayout("layouts/TextDemo.layout")
-- 	w:setVisible(false)
-- 	rootWindow:addChildWindow(w)



logger:logEvent( "<<< Init script says goodbye" )
