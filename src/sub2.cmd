# sub2.cmd - Cursed Menu definition file
#
# Lines beginning with a pound sign (#) are comments.
# --------------------------------------------------------
#
debug = yes;
pause_after_execution = no;

   # Each menu configuration contains one "MainMenu" which is the
   # first menu displayed.
   # ---------------------------------------------------
   MenuBegin submenu2
    
      MenuTitle = Sub Menu Two
      MenuFore  = WHITE
      MenuBack  = BLUE
    
      ItemName  = Item #1
      ItemDesc  = This is a very nice 1st item
      ItemExec  = echo "Item #1"; sleep 1
      ItemEnd
    
      ItemName  = Item #2
      ItemDesc  = This is a very nice 2nd item
      ItemExec  = echo "Item #2"; sleep 1
      ItemEnd
    
      ItemName  = Exit
      ItemDesc  = Exit this menu
      ItemExec  = MenuExit
      ItemEnd
   
   MenuEnd
