# sub.cmd - The default sub Cursed Menu definition file
#
# Lines beginning with a pound sign (#) are comments.
# --------------------------------------------------------
#
debug = yes;
pause_after_execution = no;

   # Each menu file contain one "MainMenu" which is the
   # first menu displayed.
   # ---------------------------------------------------
   MenuBegin SubMenu
    
      MenuTitle=Sub Menu
      MenuFore=white
      MenuBack=blue
    
      ItemName=Item #1
      ItemDesc=This is a very nice 1st item
      ItemExec=echo "Item #1"; sleep 5
      ItemEnd
    
      ItemName=Item #2
      ItemDesc=This is a very nice 2nd item
      ItemExec=echo "Item #2"; sleep 5
      ItemEnd
    
      ItemName=Exit
      ItemDesc=Exit this menu
      ItemExec=MenuExit
      ItemEnd
   
   MenuEnd SubMenu
